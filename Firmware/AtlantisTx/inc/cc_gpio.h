/*
 * cc_gpio.h
 *
 *  Created on: Nov 28, 2013
 *      Author: kreyl
 */

#ifndef CC_GPIO_H_
#define CC_GPIO_H_

// Pins
#define CC_GPIO     GPIOA
#define CC_GDO2     2
#define CC_GDO0     3
#define CC_SCK      5
#define CC_MISO     6
#define CC_MOSI     7
#define CC_CS       4

#if CC_GDO0 == 0
#define GDO0_IRQ_HANLER     EXTI0_IRQHandler
#elif CC_GDO0 == 1
#define GDO0_IRQ_HANLER     EXTI1_IRQHandler
#elif CC_GDO0 == 2
#define GDO0_IRQ_HANLER     EXTI2_IRQHandler
#elif CC_GDO0 == 3
#define GDO0_IRQ_HANLER     EXTI3_IRQHandler
#elif CC_GDO0 == 4
#define GDO0_IRQ_HANLER     EXTI4_IRQHandler
#endif


// SPI
#define CC_SPI      SPI1

#endif /* CC_GPIO_H_ */
