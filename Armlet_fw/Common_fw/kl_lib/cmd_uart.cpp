/*
 * cmd_uart.cpp
 *
 *  Created on: 15.04.2013
 *      Author: kreyl
 */

#include "cmd_uart.h"
#include <string.h>

CmdUart_t Uart;

static inline void FPutChar(char c) { Uart.IPutChar(c); }

void CmdUart_t::IPutChar(char c) {
    *PWrite++ = c;
    if(PWrite >= &TXBuf[UART_TXBUF_SIZE]) PWrite = TXBuf;   // Circulate buffer
}

void CmdUart_t::Printf(const char *format, ...) {
    chSysLock();
    va_list args;
    va_start(args, format);
    IPrintf(format, args);
    va_end(args);
    chSysUnlock();
}

void CmdUart_t::IPrintf(const char *format, va_list args) {
    int32_t MaxLength = UART_TXBUF_SIZE - IFullSlotsCount;
    IFullSlotsCount += kl_vsprintf(FPutChar, MaxLength, format, args);
    // Start transmission if Idle
    if(IDmaIsIdle) ISendViaDMA();
}

void CmdUart_t::ISendViaDMA() {
    uint32_t PartSz = (TXBuf + UART_TXBUF_SIZE) - PRead; // Cnt from PRead to end of buf
    ITransSize = MIN(IFullSlotsCount, PartSz);
    if(ITransSize != 0) {
        IDmaIsIdle = false;
        dmaStreamSetMemory0(UART_DMA_TX, PRead);
        dmaStreamSetTransactionSize(UART_DMA_TX, ITransSize);
        dmaStreamSetMode(UART_DMA_TX, UART_DMA_TX_MODE);
        dmaStreamEnable(UART_DMA_TX);
    }
}

// ================================= Thread ====================================
#if UART_RX_ENABLED

static inline bool TryConvertToDigit(uint8_t b, uint8_t *p) {
    if((b >= '0') and (b <= '9')) {
        *p = b - '0';
        return true;
    }
    else if((b >= 'A') and (b <= 'F')) {
        *p = 0x0A + b - 'A';
        return true;
    }
    else return false;
}
static inline bool IsDelimiter(uint8_t b) { return (b == ','); }
static inline bool IsEnd(uint8_t b) { return (b == '\r') or (b == '\n'); }

static WORKING_AREA(waUartRxThread, 512);
static msg_t UartRxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("UartRx");
    msg_t Msg;
    while(1) {
        // Fetch byte from queue
        Msg = chIQGetTimeout(&Uart.iqueue, TIME_INFINITE);
        if(Msg >= Q_OK) Uart.IProcessByte((uint8_t)Msg);
    } // while 1
    return 0;
}

void CmdUart_t::IProcessByte(uint8_t b) {
    uint8_t d=0;
    if(b == '#') RxState = rsCmdCode1; // If # is received anywhere, start again
    else switch(RxState) {
        case rsCmdCode1:
            if(TryConvertToDigit(b, &d)) {
                CmdCode = d << 4;
                RxState = rsCmdCode2;
            }
            else IResetCmd();
            break;

        case rsCmdCode2:
            if(TryConvertToDigit(b, &d)) {
                CmdCode |= d;
                RxState = rsData1;
            }
            else IResetCmd();
            break;

        case rsData1:
            if(TryConvertToDigit(b, &d)) {
                *PCmdWrite = d << 4;
                RxState = rsData2;
            }
            else if(IsDelimiter(b)) return; // skip delimiters
            else if(IsEnd(b)) {
                UartCmdCallback(CmdCode, CmdData, (PCmdWrite - CmdData));
                IResetCmd();
            }
            else IResetCmd();
            break;

        case rsData2:
            if(TryConvertToDigit(b, &d)) {
                *PCmdWrite |= d;
                RxState = rsData1;  // Prepare to rx next byte
                if(PCmdWrite < (CmdData + (UART_CMDDATA_SZ-1))) PCmdWrite++;
            }
            else IResetCmd(); // Delimiters and End symbols are not allowed in the middle of byte
            break;

        default: break;
    } // switch
}
#endif

// ==== Init & DMA ====
// Wrapper for IRQ
extern "C" {
void CmdUartTxIrq(void *p, uint32_t flags) { Uart.IRQDmaTxHandler(); }
}

void CmdUart_t::Init(uint32_t ABaudrate) {
    PWrite = TXBuf;
    PRead = TXBuf;
    IDmaIsIdle = true;
    IFullSlotsCount = 0;
    IBaudrate = ABaudrate;
    PinSetupAlterFunc(UART_GPIO, UART_TX_PIN, omPushPull, pudNone, UART_AF);

    // ==== USART configuration ====
    UART_RCC_ENABLE();
    UART->CR1 = USART_CR1_UE;     // Enable USART
    OnAHBFreqChange();
    UART->CR2 = 0;
    UART->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
#if UART_RX_ENABLED
    UART->CR1 |=
            USART_CR1_TE            // Transmitter enable
            | USART_CR1_RE          // Receiver enable
            | USART_CR1_RXNEIE;     // RX Irq enable
    UART_RX_IRQ_ENABLE();           // Enable irq vector
#else
    UART->CR1 = USART_CR1_TE;     // Transmitter enabled
#endif

    // ==== DMA ====
    dmaStreamAllocate     (UART_DMA_TX, IRQ_PRIO_HIGH, CmdUartTxIrq, NULL);
    dmaStreamSetPeripheral(UART_DMA_TX, &UART->DR);
    dmaStreamSetMode      (UART_DMA_TX, UART_DMA_TX_MODE);

#if UART_RX_ENABLED
    chIQInit(&iqueue, IRxBuf, UART_RXBUF_SZ, NULL, NULL);
    IResetCmd();
    PinSetupAlterFunc(UART_GPIO, UART_RX_PIN,  omOpenDrain, pudPullUp, UART_AF);

    // Create and start thread
    chThdCreateStatic(waUartRxThread, sizeof(waUartRxThread), NORMALPRIO, UartRxThread, NULL);
#endif
    UART->CR1 |= USART_CR1_UE;       // Enable USART
}

void CmdUart_t::OnAHBFreqChange() {
    if(UART == USART1) UART->BRR = Clk.APB2FreqHz / IBaudrate;
    else               UART->BRR = Clk.APB1FreqHz / IBaudrate;
}

// ==== TX DMA IRQ ====
void CmdUart_t::IRQDmaTxHandler() {
    dmaStreamDisable(UART_DMA_TX);    // Registers may be changed only when stream is disabled
    IFullSlotsCount -= ITransSize;
    PRead += ITransSize;
    if(PRead >= (TXBuf + UART_TXBUF_SIZE)) PRead = TXBuf; // Circulate pointer

    if(IFullSlotsCount == 0) IDmaIsIdle = true; // Nothing left to send
    else ISendViaDMA();
}

#if UART_RX_ENABLED
// UART RX irq
extern "C" {
CH_IRQ_HANDLER(UART_RX_IRQ) {
    CH_IRQ_PROLOGUE();
    chSysLockFromIsr();
    uint8_t b = UART_RX_REG;
    // Put byte to queue
    chIQPutI(&Uart.iqueue, b);
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}
} // extern C
#endif
