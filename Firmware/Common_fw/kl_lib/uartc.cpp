/*
 * uartc.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: g.kruglov
 */

#include "uartc.h"
#include "cmd_uart.h"
#include <stdarg.h>


void UartPrintfStr(const char *format) {
    Uart.Printf(format);
}

void UartPrintfStrUint(const char *format, uint32_t d) {
    Uart.Printf(format, d);
}
