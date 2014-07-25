/*
 * cmd_uart.cpp
 *
 *  Created on: 15.04.2013
 *      Author: kreyl
 */


#include "cmd_uart.h"
#include <string.h>
#include "application.h"

CmdUart_t Uart;

extern "C" {
void PrintfC(const char *format, ...) {
    chSysLock();
    va_list args;
    va_start(args, format);
    Uart.IPrintf(format, args);
    va_end(args);
    chSysUnlock();
}
}

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
void CmdUart_t::PollRx() {
    int32_t Sz = UART_RXBUF_SZ - UART_DMA_RX->stream->NDTR;   // Number of bytes copied to buffer since restart
    if(Sz != SzOld) {
        int32_t ByteCnt = Sz - SzOld;
        if(ByteCnt < 0) ByteCnt += UART_RXBUF_SZ;   // Handle buffer circulation
        SzOld = Sz;
        for(int32_t i=0; i<ByteCnt; i++) {          // Iterate received bytes
            char c = IRxBuf[RIndx++];
            if(c == '\b') PCmdWrite->Backspace();
            else if((c == '\r') or (c == '\n')) CompleteCmd();
            else PCmdWrite->PutChar(c);
            if(RIndx >= UART_RXBUF_SZ) RIndx = 0;
        }
    }
}

void CmdUart_t::CompleteCmd() {
    if(PCmdWrite->IsEmpty()) return;
    chSysLock();
    PCmdWrite->Finalize();
    PCmdRead = PCmdWrite;
    PCmdWrite = (PCmdWrite == &ICmd[0])? &ICmd[1] : &ICmd[0];
    PCmdWrite->Cnt = 0;
    chSysUnlock();
    App.OnUartCmd(PCmdRead);
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
    if(UART == USART1) UART->BRR = Clk.APB2FreqHz / ABaudrate;
    else               UART->BRR = Clk.APB1FreqHz / ABaudrate;
    UART->CR2 = 0;
    // ==== DMA ====
    dmaStreamAllocate     (UART_DMA_TX, IRQ_PRIO_HIGH, CmdUartTxIrq, NULL);
    dmaStreamSetPeripheral(UART_DMA_TX, &UART->DR);
    dmaStreamSetMode      (UART_DMA_TX, UART_DMA_TX_MODE);

#if UART_RX_ENABLED
    UART->CR1 = USART_CR1_TE | USART_CR1_RE;        // TX & RX enable
    UART->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;    // Enable DMA at TX & RX

    PinSetupAlterFunc(UART_GPIO, UART_RX_PIN,  omOpenDrain, pudPullUp, UART_AF);

    dmaStreamAllocate     (UART_DMA_RX, IRQ_PRIO_LOW, nullptr, NULL);
    dmaStreamSetPeripheral(UART_DMA_RX, &UART->DR);
    dmaStreamSetMemory0   (UART_DMA_RX, IRxBuf);
    dmaStreamSetTransactionSize(UART_DMA_RX, UART_RXBUF_SZ);
    dmaStreamSetMode      (UART_DMA_RX, UART_DMA_RX_MODE);
    dmaStreamEnable       (UART_DMA_RX);
#else
    UART->CR1 = USART_CR1_TE;     // Transmitter enabled
    UART->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
#endif
    UART->CR1 |= USART_CR1_UE;    // Enable USART
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
