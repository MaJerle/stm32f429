/**
 *	Pretty accurate delay functions with SysTick timer
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.1
 *	@ide		Keil uVision
 */
/**

If you are working with Keil uVision, than set KEIL_IDE define in options for project:

- Open "Options for target"
- Tab "C/C++"
- Under "Define" type "KEIL_IDE", without quotes

*/
#ifndef TM_DELAY_
#define TM_DELAY_ 110
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"

static __IO uint32_t TM_TimingDelay;

/**
 * This variable can be used in main
 * It is automatically increased every time systick make an interrupt
 */
static __IO uint32_t TM_Time;

/**
 *
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in microseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime);

/**
 *
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delayms(__IO uint32_t nTime);

/**
 *
 *
 * Initialize timer settings for delay
 */
void TM_DELAY_Init(void);

/**
 * Get the TM_Time variable value
 */
uint32_t TM_DELAY_Time(void);

/**
 * Set value for TM_Time variable
 */
void TM_DELAY_SetTime(uint32_t time);

#endif
