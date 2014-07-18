#include "sound.h"
#include <string.h>
#include "evt_mask.h"

//#if SOUND_ENABLED

Sound_t Sound;

// Mode register
#define VS_MODE_REG_VALUE   0x0802  // Native SDI mode, Layer I + II enabled

// After file end, send several zeroes
#define ZERO_SEQ_LEN        128
static const uint8_t SZero = 0;

static uint8_t ReadWriteByte(uint8_t AByte);

// ================================= IRQ =======================================
extern "C" {
// Dreq IRQ
CH_IRQ_HANDLER(EXTI0_IRQHandler) {
    CH_IRQ_PROLOGUE();
   // Uart.PrintNow("I ");
    EXTI->PR = (1 << 0);  // Clean irq flag
//    Uart.Printf("Irq ");
    Sound.ISendNextData();
  //  Uart.PrintNow("i ");
    CH_IRQ_EPILOGUE();
}
// DMA irq
void SIrqDmaHandler(void *p, uint32_t flags) { Sound.IrqDmaHandler(); }
} // extern c

void Sound_t::IrqDmaHandler() {
   // Uart.PrintNow("D ");
    ISpi.WaitBsyLo();               // Wait SPI transaction end
    XCS_Hi();                       // }
    XDCS_Hi();                      // } Stop SPI
    if(IDreq.IsHi()) ISendNextData();   // More data allowed, send it now
    else IDreq.EnableIrq(IRQ_PRIO_MEDIUM); // Enable dreq irq
   // Uart.PrintNow("d ");
}

// =========================== Implementation ==================================
static WORKING_AREA(waSoundThread, 512);
__attribute__((noreturn))
static void SoundThread(void *arg) {
    chRegSetThreadName("Sound");
    while(1) Sound.ITask();
}

void Sound_t::ITask() {
    eventmask_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    // Play new request
    if(EvtMsk & VS_EVT_COMPLETED) {
//        Uart.Printf("\rcmp");
        AddCmd(VS_REG_MODE, 0x0004);    // Soft reset
        if(IFilename != NULL) IPlayNew();
        else if(IPThd != nullptr) chEvtSignal(IPThd, EVTMASK_PLAY_ENDS);  // Raise event if nothing to play
    }
    // Stop request
    else if(EvtMsk & VS_EVT_STOP) {
//        Uart.Printf("\rStop");
        PrepareToStop();
    }
    // Data read request
    else if(EvtMsk & VS_EVT_READ_NEXT) {
        FRESULT rslt = FR_NO_FILE;
        bool EofAtStart = f_eof(&IFile);
        // Read next if not EOF
        if(!EofAtStart) {
            if     (Buf1.DataSz == 0) { /*Uart.Printf("1"); */rslt = Buf1.ReadFromFile(&IFile); }
            else if(Buf2.DataSz == 0) { /*Uart.Printf("2"); */rslt = Buf2.ReadFromFile(&IFile); }
        }
        // Check if was EOF or if error occured during reading. Do not do it if EOF occured during reading.
        if((rslt != FR_OK) or EofAtStart) {
            PrepareToStop();
        }
        else StartTransmissionIfNotBusy();
    }
}

void Sound_t::Init() {
    // ==== GPIO init ====
    PinSetupOut(VS_GPIO, VS_RST, omPushPull);
    PinSetupOut(VS_GPIO, VS_XCS, omPushPull);
    PinSetupOut(VS_GPIO, VS_XDCS, omPushPull);
    Rst_Lo();
    XCS_Hi();
    XDCS_Hi();
    PinSetupIn(VS_GPIO, VS_DREQ, pudPullDown);
    PinSetupAlterFunc(VS_GPIO, VS_XCLK, omPushPull, pudNone, VS_AF);
    PinSetupAlterFunc(VS_GPIO, VS_SO,   omPushPull, pudNone, VS_AF);
    PinSetupAlterFunc(VS_GPIO, VS_SI,   omPushPull, pudNone, VS_AF);

    // ==== SPI init ====
    ISpi.Setup(VS_SPI, boMSB, cpolIdleLow, cphaFirstEdge, sbFdiv4);
    ISpi.Enable();
    ISpi.EnableTxDma();

    // ==== DMA ====
    // Here only unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (VS_DMA, IRQ_PRIO_MEDIUM, SIrqDmaHandler, NULL);
    dmaStreamSetPeripheral(VS_DMA, &VS_SPI->DR);
    dmaStreamSetMode      (VS_DMA, VS_DMA_MODE);

    // ==== Variables ====
    State = sndStopped;
    IDmaIdle = true;
    PBuf = &Buf1;
    IAttenuation = VS_INITIAL_ATTENUATION;
    chMBInit(&CmdBox, CmdBuf, VS_CMD_BUF_SZ);

    // ==== Init VS ====
    Rst_Hi();
    Clk.MCO1Enable(mco1HSE, mcoDiv1);   // Only after reset, as pins are grounded when Rst is Lo
    chThdSleepMicroseconds(45);
    // ==== DREQ IRQ ====
    IDreq.Setup(VS_GPIO, VS_DREQ, ttRising);
    // ==== Thread ====
    PThread = chThdCreateStatic(waSoundThread, sizeof(waSoundThread), NORMALPRIO, (tfunc_t)SoundThread, NULL);
    //StartTransmissionIfNotBusy();   // Send init commands
}

void Sound_t::IPlayNew() {
    AddCmd(VS_REG_MODE, VS_MODE_REG_VALUE);
    AddCmd(VS_REG_CLOCKF, (0x8000 + (12000000/2000)));
    AddCmd(VS_REG_VOL, ((IAttenuation * 256) + IAttenuation));

    FRESULT rslt;
    // Open new file
    Uart.Printf("\rPlay %S", IFilename);
    rslt = f_open(&IFile, IFilename, FA_READ+FA_OPEN_EXISTING);
    if (rslt != FR_OK) {
        if (rslt == FR_NO_FILE) Uart.Printf("\r%S: not found", IFilename);
        else Uart.Printf("\rOpenFile error: %u", rslt);
        IFilename = NULL;
        Stop();
        return;
    }
    IFilename = NULL;
    // Check if zero file
    if (IFile.fsize == 0) {
        f_close(&IFile);
        Uart.Printf("\rEmpty file");
        Stop();
        return;
    }
    // Fast forward to start position if not zero
    if(IStartPosition != 0) {
        if(IStartPosition < IFile.fsize) f_lseek(&IFile, IStartPosition);
    }

    // Initially, fill both buffers
    if(Buf1.ReadFromFile(&IFile) != OK) { Stop(); return; }
    // Fill second buffer if needed
    if(Buf1.DataSz == VS_DATA_BUF_SZ) Buf2.ReadFromFile(&IFile);

    PBuf = &Buf1;
    State = sndPlaying;
    StartTransmissionIfNotBusy();
}

// ================================ Inner use ==================================
void Sound_t::AddCmd(uint8_t AAddr, uint16_t AData) {
    VsCmd_t FCmd;
    FCmd.OpCode = VS_WRITE_OPCODE;
    FCmd.Address = AAddr;
    FCmd.Data = __REV16(AData);
    // Add cmd to queue
    chMBPost(&CmdBox, FCmd.Msg, TIME_INFINITE);
    StartTransmissionIfNotBusy();
}

void Sound_t::ISendNextData() {
//    Uart.Printf("sn\r");
    IDreq.DisableIrq();
    dmaStreamDisable(VS_DMA);
    IDmaIdle = false;
    // If command queue is not empty, send command
    chSysLockFromIsr();
    msg_t msg = chMBFetchI(&CmdBox, &ICmd.Msg);
    chSysUnlockFromIsr();
    if(msg == RDY_OK) {
//        Uart.Printf("vCmd: %A\r", &ICmd, 4, ' ');
        XCS_Lo();   // Start Cmd transmission
        dmaStreamSetMemory0(VS_DMA, &ICmd);
        dmaStreamSetTransactionSize(VS_DMA, sizeof(VsCmd_t));
        dmaStreamSetMode(VS_DMA, VS_DMA_MODE | STM32_DMA_CR_MINC);  // Memory pointer increase
        dmaStreamEnable(VS_DMA);
    }
    // Send next chunk of data if any
    else if(State == sndPlaying) {
        //Uart.Printf("D");
        // Send data if buffer is not empty
        if(PBuf->DataSz != 0) {
            XDCS_Lo();  // Start data transmission
            uint32_t FLength = (PBuf->DataSz > 32)? 32 : PBuf->DataSz;
            dmaStreamSetMemory0(VS_DMA, PBuf->PData);
            dmaStreamSetTransactionSize(VS_DMA, FLength);
            dmaStreamSetMode(VS_DMA, VS_DMA_MODE | STM32_DMA_CR_MINC);  // Memory pointer increase
            dmaStreamEnable(VS_DMA);
            // Process pointers and lengths
            PBuf->DataSz -= FLength;
            PBuf->PData += FLength;
        }
        else IDmaIdle = true;   // Will come true if both buffers are empty

        // Check if buffer is now empty
        if(PBuf->DataSz == 0) {
            // Prepare to read next chunk
//            Uart.Printf("*");
            chSysLockFromIsr();
            chEvtSignalI(PThread, VS_EVT_READ_NEXT);
            chSysUnlockFromIsr();
            // Switch to next buf
            PBuf = (PBuf == &Buf1)? &Buf2 : &Buf1;
        }
    }
    else if(State == sndWritingZeroes) {
//        Uart.Printf("Z");
        if(ZeroesCount == 0) { // Was writing zeroes, now all over
            State = sndStopped;
            IDmaIdle = true;
//            Uart.Printf("vEnd\r");
            chSysLockFromIsr();
            chEvtSignalI(PThread, VS_EVT_COMPLETED);
            chSysUnlockFromIsr();
        }
        else SendZeroes();
    }
    else {
//        Uart.Printf("I\r");
        if(!IDreq.IsHi()) IDreq.EnableIrq(IRQ_PRIO_MEDIUM);
        else IDmaIdle = true;
    }
}

void Sound_t::PrepareToStop() {
    State = sndWritingZeroes;
    ZeroesCount = ZERO_SEQ_LEN;
    if(IFile.fs != 0) f_close(&IFile);
    StartTransmissionIfNotBusy();
}

void Sound_t::SendZeroes() {
//    Uart.Printf("sz\r");
    XDCS_Lo();  // Start data transmission
    uint32_t FLength = (ZeroesCount > 32)? 32 : ZeroesCount;
    dmaStreamSetMemory0(VS_DMA, &SZero);
    dmaStreamSetTransactionSize(VS_DMA, FLength);
    dmaStreamSetMode(VS_DMA, VS_DMA_MODE);  // Do not increase memory pointer
    dmaStreamEnable(VS_DMA);
    ZeroesCount -= FLength;
}

uint8_t ReadWriteByte(uint8_t AByte) {
    VS_SPI->DR = AByte;
    while(!(VS_SPI->SR & SPI_SR_RXNE));
    return (uint8_t)(VS_SPI->DR);
}

// ==== Commands ====
uint8_t Sound_t::CmdRead(uint8_t AAddr, uint16_t* AData) {
//    uint8_t IReply;
    uint16_t IData;
    // Wait until ready
    //if ((IReply = BusyWait()) != OK) return IReply; // Get out in case of timeout
    XCS_Lo();   // Start transmission
    ReadWriteByte(VS_READ_OPCODE);  // Send operation code
    ReadWriteByte(AAddr);           // Send addr
    *AData = ReadWriteByte(0);      // Read upper byte
    *AData <<= 8;
    IData = ReadWriteByte(0);       // Read lower byte
    *AData += IData;
    XCS_Hi();   // End transmission
    return OK;
}
uint8_t Sound_t::CmdWrite(uint8_t AAddr, uint16_t AData) {
//    uint8_t IReply;
    // Wait until ready
//    if ((IReply = BusyWait()) != OK) return IReply; // Get out in case of timeout
    XCS_Lo();                       // Start transmission
    ReadWriteByte(VS_WRITE_OPCODE); // Send operation code
    ReadWriteByte(AAddr);           // Send addr
    ReadWriteByte(AData >> 8);      // Send upper byte
    ReadWriteByte(0x00FF & AData);  // Send lower byte
    XCS_Hi();                       // End transmission
    return OK;
}

//#endif // #if SOUND_ENABLED
