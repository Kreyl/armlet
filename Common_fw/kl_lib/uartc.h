/*
 * uartc.h
 *
 *  Created on: Oct 3, 2013
 *      Author: g.kruglov
 */

#ifndef UARTC_H_
#define UARTC_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void UartPrintfStr(const char *format);
void UartPrintfStrUint(const char *format, uint32_t d);


#ifdef __cplusplus
}
#endif


#endif /* UARTC_H_ */
