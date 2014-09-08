/*
 * cmd_uart.h
 *
 *  Created on: 15.04.2013
 *      Author: kreyl
 */

#ifndef CMD_UART_H_
#define CMD_UART_H_

#include "stm32f2xx.h"
#include "ch.h"
#include "hal.h"
#include "kl_lib_f2xx.h"
#include "kl_sprintf.h"

#include "main.h"

// Set to true if RX needed
#define UART_RX_ENABLED     TRUE

// UART
#define UART_TXBUF_SIZE     7200
#define UART                USART2
#define UART_GPIO           GPIOD
#define UART_TX_PIN         5
#define UART_AF             AF7  // for all uarts
#define UART_DMA_CHNL       4
#define UART_RCC_ENABLE()   rccEnableUSART2(FALSE)

#define UART_DMA_TX         STM32_DMA1_STREAM6
#define UART_DMA_TX_MODE    STM32_DMA_CR_CHSEL(UART_DMA_CHNL) | \
                            DMA_PRIORITY_LOW | \
                            STM32_DMA_CR_MSIZE_BYTE | \
                            STM32_DMA_CR_PSIZE_BYTE | \
                            STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                            STM32_DMA_CR_DIR_M2P |    /* Direction is memory to peripheral */ \
                            STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */

#if UART_RX_ENABLED
#define UART_RXBUF_SZ       72 // unprocessed bytes
#define UART_CMD_BUF_SZ     54 // payload bytes
#define UART_RX_PIN         6
#define UART_RX_REG         UART->DR

#define UART_RX_POLLING_MS  99
#define UART_DMA_RX         STM32_DMA1_STREAM5
#define UART_DMA_RX_MODE    STM32_DMA_CR_CHSEL(UART_DMA_CHNL) | \
                            DMA_PRIORITY_LOW | \
                            STM32_DMA_CR_MSIZE_BYTE | \
                            STM32_DMA_CR_PSIZE_BYTE | \
                            STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                            STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                            STM32_DMA_CR_CIRC         /* Circular buffer enable */
#define DELIMITERS      " ,"

class Cmd_t {
private:
    void Finalize() {
        for(uint32_t i=Cnt; i < UART_CMD_BUF_SZ; i++) IString[i] = 0;
        Name = strtok(IString, DELIMITERS);
        GetNextToken();
    }
    void Backspace() { if(Cnt > 0) Cnt--; }
    char IString[UART_CMD_BUF_SZ];
    uint32_t Cnt;
public:
    char *Name, *Token;
    void PutChar(char c) { if(Cnt < UART_CMD_BUF_SZ-1) IString[Cnt++] = c; }
    bool IsEmpty() { return (Cnt == 0); }
    uint8_t GetNextToken() {
        Token = strtok(NULL, DELIMITERS);
        return (*Token == '\0')? 1 : 0;
    }
    uint8_t TryConvertTokenToNumber(uint32_t *POutput) { return Convert::TryStrToUInt32(Token, POutput); }
    uint8_t TryConvertTokenToNumber( int32_t *POutput) { return Convert::TryStrToInt32(Token, POutput); }
    bool NameIs(const char *SCmd) { return (strcasecmp(Name, SCmd) == 0); }
    friend class CmdUart_t;
};
#endif

class CmdUart_t {
private:
    char TXBuf[UART_TXBUF_SIZE];
    char *PWrite, *PRead;
    bool IDmaIsIdle;
    uint32_t IFullSlotsCount, ITransSize;
    void ISendViaDMA();
#if UART_RX_ENABLED
    int32_t SzOld=0, RIndx=0;
    uint8_t IRxBuf[UART_RXBUF_SZ];
    Cmd_t ICmd[2], *PCmdWrite = &ICmd[0], *PCmdRead = &ICmd[1];
    void CompleteCmd();
#endif
    uint32_t IBaudrate;
public:
    void Printf(const char *S, ...);
    void PrintfI(const char *S, ...);
    void FlushTx() { while(!IDmaIsIdle); }  // wait DMA
    void PrintNow(const char *S) {
        while(*S != 0) {
            while(!(UART->SR & USART_SR_TXE));
            UART->DR = *S++;
            while(!(UART->SR & USART_SR_TXE));
        }
    }
    void Init(uint32_t ABaudrate);
    void OnAHBFreqChange();
    void DeInit() { UART->CR1 &= ~USART_CR1_UE; /* UART Disable*/  }
    void Cmd(uint8_t CmdCode, uint8_t *PData, uint32_t Length) { Printf("\r\n#%X,%A", CmdCode, PData, Length, 0); }
    // Inner use
    void IPrintf(const char *format, va_list args);
    void IRQDmaTxHandler();
    void IPutChar(char c);
#if UART_RX_ENABLED
    void PollRx();
    // Command and reply
    void Ack(int32_t Result) { Printf("\r\n#Ack %d", Result); }
#endif
};

extern CmdUart_t Uart;

#endif /* CMD_UART_H_ */
