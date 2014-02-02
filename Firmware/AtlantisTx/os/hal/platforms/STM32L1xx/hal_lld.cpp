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
 * @file    STM32L1xx/hal_lld.c
 * @brief   STM32L1xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "clocking_L1xx.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Initializes the backup domain.
 */
//static void hal_lld_backup_domain_init(void) {

  /* Backup domain access enabled and left open.*/
//  PWR->CR |= PWR_CR_DBP;

  /* Reset BKP domain if different clock source selected.*/
//  if ((RCC->CSR & STM32_RTCSEL_MASK) != STM32_RTCSEL){
//    /* Backup domain reset.*/
//    RCC->CSR |= RCC_CSR_RTCRST;
//    RCC->CSR &= ~RCC_CSR_RTCRST;
//  }

  /* If enabled then the LSE is started.*/
//#if STM32_LSE_ENABLED
//  RCC->CSR |= RCC_CSR_LSEON;
//  while ((RCC->CSR & RCC_CSR_LSERDY) == 0)
//    ;                                     /* Waits until LSE is stable.   */
//#endif

//#if STM32_RTCSEL != STM32_RTCSEL_NOCLOCK
//  /* If the backup domain hasn't been initialized yet then proceed with
//     initialization.*/
//  if ((RCC->CSR & RCC_CSR_RTCEN) == 0) {
//    /* Selects clock source.*/
//    RCC->CSR |= STM32_RTCSEL;
//
//    /* RTC clock enabled.*/
//    RCC->CSR |= RCC_CSR_RTCEN;
//  }
//#endif /* STM32_RTCSEL != STM32_RTCSEL_NOCLOCK */
//}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* Reset of all peripherals.*/
  rccResetAHB(~RCC_AHBRSTR_FLITFRST);
  rccResetAPB1(~RCC_APB1RSTR_PWRRST);
  rccResetAPB2(~0);

  rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, TRUE);

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = Clk.AHBFreqHz / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* DWT cycle counter enable.*/
//  SCS_DEMCR |= SCS_DEMCR_TRCENA;
//  DWT_CTRL  |= DWT_CTRL_CYCCNTENA;

  /* PWR clock enabled.*/
  rccEnablePWRInterface(FALSE);

  /* Initializes the backup domain.*/
//  hal_lld_backup_domain_init();

#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif

  /* Programmable voltage detector enable.*/
#if STM32_PVD_ENABLE
  PWR->CR |= PWR_CR_PVDE | (STM32_PLS & STM32_PLS_MASK);
#endif /* STM32_PVD_ENABLE */
}

/** @} */
