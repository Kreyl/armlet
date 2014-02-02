/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    STM32L1xx/hal_lld.h
 * @brief   STM32L1xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32L1XX_MD for Ultra Low Power Medium-density devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "stm32.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @name    Platform identification
 * @{
 */
#define PLATFORM_NAME           "STM32L1xx Ultra Low Power Medium Density"
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
// @KL
//#define STM32_HSICLK            16000000    /**< High speed internal clock. */
//#define STM32_LSICLK            38000       /**< Low speed internal clock.  */
/** @} */

/**
 * @name    PWR_CR register bits definitions
 * @{
 */
#define STM32_VOS_MASK          (3 << 11)   /**< Core voltage mask.         */
#define STM32_VOS_1P8           (1 << 11)   /**< Core voltage 1.8 Volts.    */
#define STM32_VOS_1P5           (2 << 11)   /**< Core voltage 1.5 Volts.    */
#define STM32_VOS_1P2           (3 << 11)   /**< Core voltage 1.2 Volts.    */

#define STM32_PLS_MASK          (7 << 5)    /**< PLS bits mask.             */
#define STM32_PLS_LEV0          (0 << 5)    /**< PVD level 0.               */
#define STM32_PLS_LEV1          (1 << 5)    /**< PVD level 1.               */
#define STM32_PLS_LEV2          (2 << 5)    /**< PVD level 2.               */
#define STM32_PLS_LEV3          (3 << 5)    /**< PVD level 3.               */
#define STM32_PLS_LEV4          (4 << 5)    /**< PVD level 4.               */
#define STM32_PLS_LEV5          (5 << 5)    /**< PVD level 5.               */
#define STM32_PLS_LEV6          (6 << 5)    /**< PVD level 6.               */
#define STM32_PLS_LEV7          (7 << 5)    /**< PVD level 7.               */
/** @} */

/**
 * @name    RCC_CR register bits definitions
 * @{
 */
#define STM32_RTCPRE_MASK       (3 << 29)   /**< RTCPRE mask.               */
#define STM32_RTCPRE_DIV2       (0 << 29)   /**< HSE divided by 2.          */
#define STM32_RTCPRE_DIV4       (1 << 29)   /**< HSE divided by 4.          */
#define STM32_RTCPRE_DIV8       (2 << 29)   /**< HSE divided by 2.          */
#define STM32_RTCPRE_DIV16      (3 << 29)   /**< HSE divided by 16.         */
/** @} */

/**
 * @name    RCC_CFGR register bits definitions
 * @{
 */
#define STM32_SW_MSI            (0 << 0)    /**< SYSCLK source is MSI.      */
#define STM32_SW_HSI            (1 << 0)    /**< SYSCLK source is HSI.      */
#define STM32_SW_HSE            (2 << 0)    /**< SYSCLK source is HSE.      */
#define STM32_SW_PLL            (3 << 0)    /**< SYSCLK source is PLL.      */

#define STM32_HPRE_DIV1         (0 << 4)    /**< SYSCLK divided by 1.       */
#define STM32_HPRE_DIV2         (8 << 4)    /**< SYSCLK divided by 2.       */
#define STM32_HPRE_DIV4         (9 << 4)    /**< SYSCLK divided by 4.       */
#define STM32_HPRE_DIV8         (10 << 4)   /**< SYSCLK divided by 8.       */
#define STM32_HPRE_DIV16        (11 << 4)   /**< SYSCLK divided by 16.      */
#define STM32_HPRE_DIV64        (12 << 4)   /**< SYSCLK divided by 64.      */
#define STM32_HPRE_DIV128       (13 << 4)   /**< SYSCLK divided by 128.     */
#define STM32_HPRE_DIV256       (14 << 4)   /**< SYSCLK divided by 256.     */
#define STM32_HPRE_DIV512       (15 << 4)   /**< SYSCLK divided by 512.     */

#define STM32_PPRE1_DIV1        (0 << 8)    /**< HCLK divided by 1.         */
#define STM32_PPRE1_DIV2        (4 << 8)    /**< HCLK divided by 2.         */
#define STM32_PPRE1_DIV4        (5 << 8)    /**< HCLK divided by 4.         */
#define STM32_PPRE1_DIV8        (6 << 8)    /**< HCLK divided by 8.         */
#define STM32_PPRE1_DIV16       (7 << 8)    /**< HCLK divided by 16.        */

#define STM32_PPRE2_DIV1        (0 << 11)   /**< HCLK divided by 1.         */
#define STM32_PPRE2_DIV2        (4 << 11)   /**< HCLK divided by 2.         */
#define STM32_PPRE2_DIV4        (5 << 11)   /**< HCLK divided by 4.         */
#define STM32_PPRE2_DIV8        (6 << 11)   /**< HCLK divided by 8.         */
#define STM32_PPRE2_DIV16       (7 << 11)   /**< HCLK divided by 16.        */

#define STM32_PLLSRC_HSI        (0 << 16)   /**< PLL clock source is HSI.   */
#define STM32_PLLSRC_HSE        (1 << 16)   /**< PLL clock source is HSE.   */

#define STM32_MCOSEL_NOCLOCK    (0 << 24)   /**< No clock on MCO pin.       */
#define STM32_MCOSEL_SYSCLK     (1 << 24)   /**< SYSCLK on MCO pin.         */
#define STM32_MCOSEL_HSI        (2 << 24)   /**< HSI clock on MCO pin.      */
#define STM32_MCOSEL_MSI        (3 << 24)   /**< MSI clock on MCO pin.      */
#define STM32_MCOSEL_HSE        (4 << 24)   /**< HSE clock on MCO pin.      */
#define STM32_MCOSEL_PLL        (5 << 24)   /**< PLL clock on MCO pin.      */
#define STM32_MCOSEL_LSI        (6 << 24)   /**< LSI clock on MCO pin.      */
#define STM32_MCOSEL_LSE        (7 << 24)   /**< LSE clock on MCO pin.      */

#define STM32_MCOPRE_DIV1       (0 << 28)   /**< MCO divided by 1.          */
#define STM32_MCOPRE_DIV2       (1 << 28)   /**< MCO divided by 1.          */
#define STM32_MCOPRE_DIV4       (2 << 28)   /**< MCO divided by 1.          */
#define STM32_MCOPRE_DIV8       (3 << 28)   /**< MCO divided by 1.          */
#define STM32_MCOPRE_DIV16      (4 << 28)   /**< MCO divided by 1.          */
/** @} */

/**
 * @name    RCC_ICSCR register bits definitions
 * @{
 */
#define STM32_MSIRANGE_MASK     (7 << 13)   /**< MSIRANGE field mask.       */
#define STM32_MSIRANGE_64K      (0 << 13)   /**< 64kHz nominal.             */
#define STM32_MSIRANGE_128K     (1 << 13)   /**< 128kHz nominal.            */
#define STM32_MSIRANGE_256K     (2 << 13)   /**< 256kHz nominal.            */
#define STM32_MSIRANGE_512K     (3 << 13)   /**< 512kHz nominal.            */
#define STM32_MSIRANGE_1M       (4 << 13)   /**< 1MHz nominal.              */
#define STM32_MSIRANGE_2M       (5 << 13)   /**< 2MHz nominal.              */
#define STM32_MSIRANGE_4M       (6 << 13)   /**< 4MHz nominal               */
/** @} */

/**
 * @name    RCC_CSR register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK       (3 << 16)   /**< RTC source mask.           */
#define STM32_RTCSEL_NOCLOCK    (0 << 16)   /**< No RTC source.             */
#define STM32_RTCSEL_LSE        (1 << 16)   /**< RTC source is LSE.         */
#define STM32_RTCSEL_LSI        (2 << 16)   /**< RTC source is LSI.         */
#define STM32_RTCSEL_HSEDIV     (3 << 16)   /**< RTC source is HSE divided. */
/** @} */

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    STM32L1xx capabilities
 * @{
 */
/* ADC attributes.*/
#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          FALSE
#define STM32_HAS_ADC3          FALSE
#define STM32_HAS_ADC4          FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1          FALSE
#define STM32_HAS_CAN2          FALSE
#define STM32_CAN_MAX_FILTERS   0

/* DAC attributes.*/
#define STM32_HAS_DAC           TRUE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA      FALSE
#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          FALSE

/* ETH attributes.*/
#define STM32_HAS_ETH           FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS 23

/* GPIO attributes.*/
#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         FALSE
#define STM32_HAS_GPIOE         FALSE
#define STM32_HAS_GPIOF         FALSE
#define STM32_HAS_GPIOG         FALSE
#define STM32_HAS_GPIOH         TRUE
#define STM32_HAS_GPIOI         FALSE

/* I2C attributes.*/
#define STM32_HAS_I2C1          TRUE
#define STM32_I2C1_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 7))
#define STM32_I2C1_RX_DMA_CHN   0x00000000
#define STM32_I2C1_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 6))
#define STM32_I2C1_TX_DMA_CHN   0x00000000

#define STM32_HAS_I2C2          TRUE
#define STM32_I2C2_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 5))
#define STM32_I2C2_RX_DMA_CHN   0x00000000
#define STM32_I2C2_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_I2C2_TX_DMA_CHN   0x00000000

#define STM32_HAS_I2C3          FALSE
#define STM32_I2C3_RX_DMA_MSK   0
#define STM32_I2C3_RX_DMA_CHN   0x00000000
#define STM32_I2C3_TX_DMA_MSK   0
#define STM32_I2C3_TX_DMA_CHN   0x00000000

/* RTC attributes.*/
#define STM32_HAS_RTC           TRUE
#define STM32_RTC_HAS_SUBSECONDS FALSE
#define STM32_RTC_IS_CALENDAR   TRUE

/* SDIO attributes.*/
#define STM32_HAS_SDIO          FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1          TRUE
#define STM32_SPI1_RX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 2)
#define STM32_SPI1_RX_DMA_CHN   0x00000000
#define STM32_SPI1_TX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 3)
#define STM32_SPI1_TX_DMA_CHN   0x00000000

#define STM32_HAS_SPI2          TRUE
#define STM32_SPI2_RX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 4)
#define STM32_SPI2_RX_DMA_CHN   0x00000000
#define STM32_SPI2_TX_DMA_MSK   STM32_DMA_STREAM_ID_MSK(1, 5)
#define STM32_SPI2_TX_DMA_CHN   0x00000000

#define STM32_HAS_SPI3          FALSE
#define STM32_SPI3_RX_DMA_MSK   0
#define STM32_SPI3_RX_DMA_CHN   0x00000000
#define STM32_SPI3_TX_DMA_MSK   0
#define STM32_SPI3_TX_DMA_CHN   0x00000000

/* TIM attributes.*/
#define STM32_HAS_TIM1          FALSE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          FALSE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          TRUE
#define STM32_HAS_TIM10         TRUE
#define STM32_HAS_TIM11         TRUE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE
#define STM32_HAS_TIM18         FALSE
#define STM32_HAS_TIM19         FALSE

/* USART attributes.*/
#define STM32_HAS_USART1        TRUE
#define STM32_USART1_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 5))
#define STM32_USART1_RX_DMA_CHN 0x00000000
#define STM32_USART1_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_USART1_TX_DMA_CHN 0x00000000

#define STM32_HAS_USART2        TRUE
#define STM32_USART2_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 6))
#define STM32_USART2_RX_DMA_CHN 0x00000000
#define STM32_USART2_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 7))
#define STM32_USART2_TX_DMA_CHN 0x00000000

#define STM32_HAS_USART3        TRUE
#define STM32_USART3_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 3))
#define STM32_USART3_RX_DMA_CHN 0x00000000
#define STM32_USART3_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_USART3_TX_DMA_CHN 0x00000000

#define STM32_HAS_UART4         FALSE
#define STM32_UART4_RX_DMA_MSK  0
#define STM32_UART4_RX_DMA_CHN  0x00000000
#define STM32_UART4_TX_DMA_MSK  0
#define STM32_UART4_TX_DMA_CHN  0x00000000

#define STM32_HAS_UART5         FALSE
#define STM32_UART5_RX_DMA_MSK  0
#define STM32_UART5_RX_DMA_CHN  0x00000000
#define STM32_UART5_TX_DMA_MSK  0
#define STM32_UART5_TX_DMA_CHN  0x00000000

#define STM32_HAS_USART6        FALSE
#define STM32_USART6_RX_DMA_MSK 0
#define STM32_USART6_RX_DMA_CHN 0x00000000
#define STM32_USART6_TX_DMA_MSK 0
#define STM32_USART6_TX_DMA_CHN 0x00000000

/* USB attributes.*/
#define STM32_HAS_USB           TRUE
#define STM32_HAS_OTG1          FALSE
#define STM32_HAS_OTG2          FALSE
/** @} */

/*===========================================================================*/
/* Platform specific friendly IRQ names.                                     */
/*===========================================================================*/

/**
 * @name  IRQ VECTOR names
 * @{
 */
#define WWDG_IRQHandler         Vector40    /**< Window Watchdog.           */
#define PVD_IRQHandler          Vector44    /**< PVD through EXTI Line
                                                 detect.                    */
#define TAMPER_STAMP_IRQHandler Vector48    /**< Tamper and Time Stamp
                                                 through EXTI.              */
#define RTC_WKUP_IRQHandler     Vector4C    /**< RTC Wakeup Timer through
                                                 EXTI.                      */
#define FLASH_IRQHandler        Vector50    /**< Flash.                     */
#define RCC_IRQHandler          Vector54    /**< RCC.                       */
#define EXTI0_IRQHandler        Vector58    /**< EXTI Line 0.               */
#define EXTI1_IRQHandler        Vector5C    /**< EXTI Line 1.               */
#define EXTI2_IRQHandler        Vector60    /**< EXTI Line 2.               */
#define EXTI3_IRQHandler        Vector64    /**< EXTI Line 3.               */
#define EXTI4_IRQHandler        Vector68    /**< EXTI Line 4.               */
#define DMA1_Ch1_IRQHandler     Vector6C    /**< DMA1 Channel 1.            */
#define DMA1_Ch2_IRQHandler     Vector70    /**< DMA1 Channel 2.            */
#define DMA1_Ch3_IRQHandler     Vector74    /**< DMA1 Channel 3.            */
#define DMA1_Ch4_IRQHandler     Vector78    /**< DMA1 Channel 4.            */
#define DMA1_Ch5_IRQHandler     Vector7C    /**< DMA1 Channel 5.            */
#define DMA1_Ch6_IRQHandler     Vector80    /**< DMA1 Channel 6.            */
#define DMA1_Ch7_IRQHandler     Vector84    /**< DMA1 Channel 7.            */
#define ADC1_IRQHandler         Vector88    /**< ADC1.                      */
#define USB_HP_IRQHandler       Vector8C    /**< USB High Priority.         */
#define USB_LP_IRQHandler       Vector90    /**< USB Low Priority.          */
#define DAC_IRQHandler          Vector94    /**< DAC.                       */
#define COMP_IRQHandler         Vector98    /**< Comparator through EXTI.   */
#define EXTI9_5_IRQHandler      Vector9C    /**< EXTI Line 9..5.            */
#define TIM9_IRQHandler         VectorA0    /**< TIM9.                      */
#define TIM10_IRQHandler        VectorA4    /**< TIM10.                     */
#define TIM11_IRQHandler        VectorA8    /**< TIM11.                     */
#define LCD_IRQHandler          VectorAC    /**< LCD.                       */
#define TIM2_IRQHandler         VectorB0    /**< TIM2.                      */
#define TIM3_IRQHandler         VectorB4    /**< TIM3.                      */
#define TIM4_IRQHandler         VectorB8    /**< TIM4.                      */
#define I2C1_EV_IRQHandler      VectorBC    /**< I2C1 Event.                */
#define I2C1_ER_IRQHandler      VectorC0    /**< I2C1 Error.                */
#define I2C2_EV_IRQHandler      VectorC4    /**< I2C2 Event.                */
#define I2C2_ER_IRQHandler      VectorC8    /**< I2C2 Error.                */
#define SPI1_IRQHandler         VectorCC    /**< SPI1.                      */
#define SPI2_IRQHandler         VectorD0    /**< SPI2.                      */
#define USART1_IRQHandler       VectorD4    /**< USART1.                    */
#define USART2_IRQHandler       VectorD8    /**< USART2.                    */
#define USART3_IRQHandler       VectorDC    /**< USART3.                    */
#define EXTI15_10_IRQHandler    VectorE0    /**< EXTI Line 15..10.          */
#define RTC_Alarm_IRQHandler    VectorE4    /**< RTC Alarm through EXTI.    */
#define USB_FS_WKUP_IRQHandler  VectorE8    /**< USB Wakeup from suspend.   */
#define TIM6_IRQHandler         VectorEC    /**< TIM6.                      */
#define TIM7_IRQHandler         VectorF0    /**< TIM7.                      */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the PWR/RCC initialization in the HAL.
 */
#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT               FALSE
#endif

/**
 * @brief   Core voltage selection.
 * @note    This setting affects all the performance and clock related
 *          settings, the maximum performance is only obtainable selecting
 *          the maximum voltage.
 */
#if !defined(STM32_VOS) || defined(__DOXYGEN__)
#define STM32_VOS                   STM32_VOS_1P8
#endif

/**
 * @brief   Enables or disables the programmable voltage detector.
 */
#if !defined(STM32_PVD_ENABLE) || defined(__DOXYGEN__)
#define STM32_PVD_ENABLE            FALSE
#endif

/**
 * @brief   Sets voltage level for programmable voltage detector.
 */
#if !defined(STM32_PLS) || defined(__DOXYGEN__)
#define STM32_PLS                   STM32_PLS_LEV0
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(STM32L1xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32L1xx_MCUCONF not defined"
#endif

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the current value of the system free running counter.
 * @note    This service is implemented by returning the content of the
 *          DWT_CYCCNT register.
 *
 * @return              The value of the system free running counter of
 *                      type halrtcnt_t.
 *
 * @notapi
 */
//#define hal_lld_get_counter_value()         DWT_CYCCNT

/**
 * @brief   Realtime counter frequency.
 * @note    The DWT_CYCCNT register is incremented directly by the system
 *          clock so this function returns STM32_HCLK.
 *
 * @return              The realtime counter frequency of type halclock_t.
 *
 * @notapi
 */
//#define hal_lld_get_counter_frequency()     STM32_HCLK

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* STM32 ISR, DMA and RCC helpers.*/
#include "stm32_isr.h"
#include "stm32_dma.h"
#include "stm32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
