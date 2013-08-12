#include "sound.h"
#include <string.h>

Sound_t Sound;

#define VS_READ_NEXT_CHUNK  99
#define VS_PLAY_END_CB      108

// After file end, send several zeroes
#define ZERO_SEQ_LEN  128
static const uint8_t SZero = 0;

static uint8_t ReadWriteByte(uint8_t AByte);

// Pin operations
static inline void Rst_Lo()   { PinClear(VS_GPIO, VS_RST); }
static inline void Rst_Hi()   { PinSet(VS_GPIO, VS_RST); }
static inline void XCS_Lo()   { PinClear(VS_GPIO, VS_XCS); }
static inline void XCS_Hi()   { PinSet(VS_GPIO, VS_XCS); }
static inline void XDCS_Lo()  { PinClear(VS_GPIO, VS_XDCS); }
static inline void XDCS_Hi()  { PinSet(VS_GPIO, VS_XDCS); }

// ================================= IRQ =======================================
extern "C" {
CH_IRQ_HANDLER(EXTI0_IRQHandler) {
    CH_IRQ_PROLOGUE();
    EXTI->PR = (1 << 0);  // Clean irq flag
    Sound.ISendNextData();
    CH_IRQ_EPILOGUE();
}

void SIrqDmaHandler(void *p, uint32_t flags) { Sound.IrqDmaHandler(); }
} // extern c


void Sound_t::IrqDmaHandler() {
//    Uart.Printf("Dma\r");
    Spi_t::WaitBsyHi2Lo(VS_SPI);    // Wait SPI transaction end
    XCS_Hi();                       // }
    XDCS_Hi();                      // } Stop SPI
    if(IDreq.IsHi()) ISendNextData();   // More data allowed, send it now
    else IDreq.Enable(IRQ_PRIO_MEDIUM); // Enable dreq irq
}

// =========================== Implementation ==================================
static WORKING_AREA(waSoundThread, 256);
void SoundThread(void *arg) {
    chRegSetThreadName("Sound");
    while(1) Sound.ISendDataTask();
}

void Sound_t::ISendDataTask() {
    chSysLock();
    chSchGoSleepTimeoutS(THD_STATE_SUSPENDED, TIME_INFINITE);
    chSysUnlock();  // Will be here when need to fill buffers
    if((State != sndPlaying) or (IFile.fs == 0)) return;    // if file is closed
    FRESULT rslt;
    if     (Buf1.DataSz == 0) rslt = Buf1.ReadFromFile(&IFile);
    else if(Buf2.DataSz == 0) rslt = Buf2.ReadFromFile(&IFile);
    if(f_eof(&IFile) or (rslt != FR_OK)) f_close(&IFile);
}
        //Uart.Printf("%u\r", Sound.State);
//        chThdSleepMilliseconds(450);
//        switch (Sound.State) {
//            case sndPlaying:
//                // Check if buf is empty
//                if(BufSz == 0) {
//                    PBuf = Buf;
//                    rslt = f_read(&IFile, Buf, BUF_SZ, &BufSz);
//                    if((rslt != FR_OK) or (BufSz == 0)) StopNow();
//                } // if buf is empty
//                else {
//                    // Upload data
//                    XDCS_Lo();  // Start transmission
//                    while(DreqIsHi() and BufSz) {
//                        ReadWriteByte(*PBuf);
//                        PBuf++;
//                        BufSz--;
//                    }
//                    XDCS_Hi();
//                }
//                break;
//
//            case sndMustStop:
//                if(DreqIsHi()) StopNow();
//                break;
//
//            default:
//                chThdSleepMilliseconds(45);
//                break; // just get out
//        } // switch
//    }
//}

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
    Spi_t::Setup(VS_SPI, boMSB, cpolIdleLow, cphaFirstEdge, sbFdiv4);
    Spi_t::Enable(VS_SPI);
    Spi_t::EnableTxDma(VS_SPI);

    // ==== DMA ====
    // Here only unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (VS_DMA, IRQ_PRIO_MEDIUM, SIrqDmaHandler, NULL);
    dmaStreamSetPeripheral(VS_DMA, &VS_SPI->DR);
    dmaStreamSetMode      (VS_DMA, VS_DMA_MODE);

    // ==== Variables ====
    State = sndStopped;
    IDmaIdle = true;
    PBuf = &Buf1;
    chMBInit(&CmdBox, CmdBuf, VS_CMD_BUF_SZ);

    // ==== Init VS ====
    Rst_Hi();
    Clk.MCO1Enable(mco1HSE, mcoDiv1);   // Only after reset, as pins are grounded when Rst is Lo
    chThdSleepMicroseconds(45);

    // ==== DREQ IRQ ====
    IDreq.Init(VS_GPIO, VS_DREQ, Rising);

// //   CmdWrite(VS_REG_MODE, (VS_SM_SDINEW | VS_SM_RESET));    // Perform software reset
    AddCmd(VS_REG_MODE, 0x0802);  // Native SPI mode, Layer I + II enabled
    AddCmd(VS_REG_CLOCKF, 0x8000 + (12000000/2000));
//    CmdWrite(VS_REG_MIXERVOL, (VS_SMV_ACTIVE | VS_SMV_GAIN2));
//    CmdWrite(VS_REG_RECCTRL, VS_SARC_DREQ512);
//    SetVolume(0);

    // ==== Thread ====
    PThread = chThdCreateStatic(waSoundThread, sizeof(waSoundThread), NORMALPRIO, (tfunc_t)SoundThread, NULL);
}

void Sound_t::Play(const char* AFilename) {
    if (State == sndPlaying) StopNow();
    FRESULT rslt;
    // Open file
    Uart.Printf("Play %S\r", AFilename);
    rslt = f_open(&IFile, AFilename, FA_READ+FA_OPEN_EXISTING);
    if (rslt != FR_OK) {
        if (rslt == FR_NO_FILE) Uart.Printf("%S: file not found\r", AFilename);
        else Uart.Printf("OpenFile error: %u", rslt);
        return;
    }
    // Check if zero file
    if (IFile.fsize == 0) {
        f_close(&IFile);
        Uart.Printf("Empty file\r");
        return;
    }
    // Initially, fill both buffers
    if(Buf1.ReadFromFile(&IFile) != OK) { StopNow(); return; }
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

void Sound_t::StopNow() {
    Uart.Printf("StopNow\r");
    chSysLock();
    Buf1.DataSz = 0;
    Buf2.DataSz = 0;
    chSysUnlock();
    f_close(&IFile);
    //klPrintf("Stopped\r");
}

void Sound_t::ISendNextData() {
//    Uart.Printf("sn\r");
    IDreq.Disable();
    dmaStreamDisable(VS_DMA);
    IDmaIdle = false;
    // If command queue is not empty, send command
    chSysLockFromIsr();
    msg_t msg = chMBFetchI(&CmdBox, &ICmd.Msg);
    chSysUnlockFromIsr();
    if(msg == RDY_OK) {
        Uart.Printf("vCmd: %A\r", &ICmd, 4, ' ');
        XCS_Lo();   // Start Cmd transmission
        dmaStreamSetMemory0(VS_DMA, &ICmd);
        dmaStreamSetTransactionSize(VS_DMA, sizeof(VsCmd_t));
        dmaStreamSetMode(VS_DMA, VS_DMA_MODE | STM32_DMA_CR_MINC);  // Memory pointer increase
        dmaStreamEnable(VS_DMA);
    }
    // Send next chunk of data if any
    else if(PBuf->DataSz != 0) {
//        Uart.Printf("D\r");
        XDCS_Lo();  // Start data transmission
        uint32_t FLength = (PBuf->DataSz > 32)? 32 : PBuf->DataSz;
        dmaStreamSetMemory0(VS_DMA, PBuf->PData);
        dmaStreamSetTransactionSize(VS_DMA, FLength);
        dmaStreamSetMode(VS_DMA, VS_DMA_MODE | STM32_DMA_CR_MINC);  // Memory pointer increase
        dmaStreamEnable(VS_DMA);
        // Process pointers and lengths
        PBuf->DataSz -= FLength;
        PBuf->PData += FLength;
        if(PBuf->DataSz == 0) {
            // Prepare to read next chunk
            chSysLockFromIsr();
            PThread->p_u.rdymsg = VS_READ_NEXT_CHUNK;
            if(PThread->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThread);
            chSysUnlockFromIsr();
            // Switch to next buf
            PBuf = (PBuf == &Buf1)? &Buf2 : &Buf1;
        }
    }
    else {  // Buffers are empty: end of file
        if(State == sndPlaying) {  // Was playing, write zeroes
            Uart.Printf("vZ1\r");
            State = sndWritingZeroes;
            ZeroesCount = ZERO_SEQ_LEN;
            SendZeroes();
        }
        else if(State == sndWritingZeroes) {
            Uart.Printf("vZ2\r");
            if(ZeroesCount == 0) { // Was writing zeroes, now all over
                State = sndStopped;
                IDmaIdle = true;
                chSysLockFromIsr();
                PThread->p_u.rdymsg = VS_PLAY_END_CB;
                if(PThread->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThread);
                chSysUnlockFromIsr();
                Uart.Printf("vEnd\r");
            }
            else SendZeroes();
        }
        else {
//            Uart.Printf("CmdEnd\r");
            IDmaIdle = true;
        }
    }
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
    uint8_t IReply;
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
    uint8_t IReply;
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
