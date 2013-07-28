#include "sound.h"
#include <string.h>

Sound_t Sound;

FIL IFile;

#define BUF_SZ  4096
static uint8_t Buf[BUF_SZ], *PBuf;
static UINT BufSz=0;

static void StopNow();
static void WriteTrailingZeroes();
static uint8_t BusyWait();
static uint8_t ReadWriteByte(uint8_t AByte);

// Pin operations
static inline void Rst_Lo()   { PinClear(VS_GPIO, VS_RST); }
static inline void Rst_Hi()   { PinSet(VS_GPIO, VS_RST); }
static inline void XCS_Lo()   { PinClear(VS_GPIO, VS_XCS); }
static inline void XCS_Hi()   { PinSet(VS_GPIO, VS_XCS); }
static inline void XDCS_Lo()  { PinClear(VS_GPIO, VS_XDCS); }
static inline void XDCS_Hi()  { PinSet(VS_GPIO, VS_XDCS); }
static inline bool DreqIsHi() { return PinIsSet(VS_GPIO, VS_DREQ); }

// =========================== Implementation ==================================
static WORKING_AREA(waSoundThread, 256);
static msg_t SoundThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Sound");

    //PinSetupOut(GPIOB, 0, omPushPull, pudNone);
    //PinSet(GPIOB, 0);
    FRESULT rslt;

    while(1) {
        //Uart.Printf("%u\r", Sound.State);
        switch (Sound.State) {
            case sndPlaying:
                // Check if buf is empty
                if(BufSz == 0) {
                    PBuf = Buf;
                    rslt = f_read(&IFile, Buf, BUF_SZ, &BufSz);
                    if((rslt != FR_OK) or (BufSz == 0)) StopNow();
                } // if buf is empty
                else {
                    // Upload data
                    XDCS_Lo();  // Start transmission
                    while(DreqIsHi() and BufSz) {
                        ReadWriteByte(*PBuf);
                        PBuf++;
                        BufSz--;
                    }
                    XDCS_Hi();
                }
                break;

            case sndMustStop:
                if(DreqIsHi()) StopNow();
                break;

            default:
                chThdSleepMilliseconds(45);
                break; // just get out
        } // switch
    }
    return 0;
}

void Sound_t::Init() {
    State = sndStopped;
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
    VS_SPI_RCC_EN();
    // NoCRC, FullDuplex, 8bit, MSB, Baudrate, Master, ClkLowIdle(CPOL=0),
    // FirstEdge(CPHA=0), NSS software controlled and is 1
    //VS_SPI->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_BAUDRATE_DIV4;
    VS_SPI->CR2 = 0;
    VS_SPI->I2SCFGR &= ~((uint16_t)SPI_I2SCFGR_I2SMOD);
    VS_SPI->CR1 |= SPI_CR1_SPE; // Enable SPI

    // ==== Init VS ====
    Rst_Hi();
    Clk.MCO1Enable(mco1HSE, mcoDiv1);   // Only after reset, as pins are grounded when Rst is Lo
    chThdSleepMicroseconds(450);
//    CmdWrite(VS_REG_MODE, (VS_SM_SDINEW | VS_SM_RESET));    // Perform software reset
    CmdWrite(VS_REG_MODE, 0x0802);  // Native SPI mode, Layer I + II enabled
    CmdWrite(VS_REG_CLOCKF, 0x8000 + (12000000/2000));    // x4, XTALI = 12.288 MHz
//    CmdWrite(VS_REG_MIXERVOL, (VS_SMV_ACTIVE | VS_SMV_GAIN2));
//    CmdWrite(VS_REG_RECCTRL, VS_SARC_DREQ512);
    SetVolume(0);

    // ==== Thread ====
    chThdCreateStatic(waSoundThread, sizeof(waSoundThread), NORMALPRIO, SoundThread, NULL);
}

void Sound_t::Play(const char* AFilename) {
    if (State == sndPlaying) StopNow();
    FRESULT rslt;
    // Open file
    Uart.Printf("%S\r", AFilename);
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
    BufSz = 0;
    State = sndPlaying;
}

// ================================ Inner use ==================================
void StopNow() {
    Uart.Printf("StopNow\r");
    WriteTrailingZeroes();
    f_close(&IFile);
    Sound.State = sndStopped;
    //klPrintf("Stopped\r");
}

uint8_t BusyWait() {
    uint32_t Timeout = VS_TIMEOUT;
    while(!DreqIsHi()) {
        Timeout--;
        if (Timeout == 0) {
            Uart.Printf("VS timeout\r");
            return VS_TIMEOUT_ER;
        }
    }
    return VS_OK;
}

void WriteTrailingZeroes() {
    if(BusyWait() != VS_OK) return;     // Get out in case of timeout
    XDCS_Lo();                          // Start transmission
    for (uint8_t i=0; i<VS_TRAILING_0_COUNT; i++) ReadWriteByte(0); // Send data
    XDCS_Hi();                          // End transmission
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
    if ((IReply = BusyWait()) != VS_OK) return IReply; // Get out in case of timeout
    XCS_Lo();   // Start transmission
    ReadWriteByte(VS_READ_OPCODE);  // Send operation code
    ReadWriteByte(AAddr);           // Send addr
    *AData = ReadWriteByte(0);      // Read upper byte
    *AData <<= 8;
    IData = ReadWriteByte(0);       // Read lower byte
    *AData += IData;
    XCS_Hi();   // End transmission
    return VS_OK;
}
uint8_t Sound_t::CmdWrite(uint8_t AAddr, uint16_t AData) {
    uint8_t IReply;
    // Wait until ready
    if ((IReply = BusyWait()) != VS_OK) return IReply; // Get out in case of timeout
    XCS_Lo();                       // Start transmission
    ReadWriteByte(VS_WRITE_OPCODE); // Send operation code
    ReadWriteByte(AAddr);           // Send addr
    ReadWriteByte(AData >> 8);      // Send upper byte
    ReadWriteByte(0x00FF & AData);  // Send lower byte
    XCS_Hi();                       // End transmission
    return VS_OK;
}
