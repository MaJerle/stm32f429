/**
 *	Pretty accurate delay functions with SysTick timer
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-03-stm32f429-discovery-system-clock-and-pretty-precise-delay-library/
 *	@version 	v1.2
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 *	
 *	If you are working with Keil uVision, than set KEIL_IDE define in options for project:
 *	- Open "Options for target"
 *	- Tab "C/C++"
 *	- Under "Define" add "KEIL_IDE", without quotes
 */
#ifndef TM_DELAY_
#define TM_DELAY_ 120
/**
 * Library dependencies
 * - STM32F4xx
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
static __IO uint32_t TM_Time2;

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

/**
 * Get the TM_Time variable value
 */
uint32_t TM_DELAY_Time2(void);

/**
 * Set value for TM_Time variable
 */
void TM_DELAY_SetTime2(uint32_t time);

/**
 * Re-enable Systick. It has to be configured before with TM_DELAY_Init();
 */
void TM_DELAY_EnableSystick(void);

/**
 * Disable Systick. Usef when going into sleep mode, so systick will not
 */
void TM_DELAY_DisableSystick(void);

#endif
