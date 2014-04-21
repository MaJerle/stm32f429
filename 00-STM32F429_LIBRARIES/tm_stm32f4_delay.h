/**
 *	Pretty accurate delay functions with SysTick timer
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */

#ifndef TM_DELAY_
#define TM_DELAY_

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

static __IO uint32_t TM_TimingDelay;


/**
 * This variable can be used in main
 * It is automatically increased every time systick make an interrupt
 *
 * //leds are toggled every 50ms.
 * TM_Time = 0;
 * while (1) {
 * 		if (TM_Time >= 50000) { //50ms
 * 			TM_Time = 0;
 * 			//Toogle leds here
 * 		}
 * 		//Do other stuff
 * }
 *
 */
static __IO uint32_t TM_Time;

/**
 *
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in microseconds.
  * @retval None
  */
inline void Delay(__IO uint32_t nTime);

/**
 *
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
inline void Delayms(__IO uint32_t nTime);

/**
 *
 *
 * Initialize timer settings for delay
 */
void TM_DELAY_Init(void);

#endif
