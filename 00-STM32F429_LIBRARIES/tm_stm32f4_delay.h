/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/04/library-03-stm32f429-discovery-system-clock-and-pretty-precise-delay-library/
 * @version v2.4
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Pretty accurate delay functions with SysTick or any other timer
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_DELAY_H
#define TM_DELAY_H 240

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_DELAY
 * @brief    Pretty accurate delay functions with SysTick or any other timer - http://stm32f4-discovery.com/2014/04/library-03-stm32f429-discovery-system-clock-and-pretty-precise-delay-library/
 * @{
 *
@verbatim
If you are using GCC compiler, then your microseconds delay is probably totally inaccurate.
USE TIMER FOR DELAY, otherwise your delay will not be accurate.

Another way is to use ARM compiler.
@endverbatim
 *
 * As of version 2.0 you have now two possible ways to make a delay.
 * The first (and default) is Systick timer. It makes interrupts every 1ms.
 * If you want delay in "us" accuracy, there is simple pooling (variable) mode.
 *
 *
 * The second (better) options is to use one of timers on F4xx MCU.
 * Timer also makes an interrupts every 1ms (for count time) instead of 1us as it was before.
 * For "us" delay, timer's counter is used to count ticks. It makes a new tick each "us".
 * Not all MCUs have all possible timers, so this lib has been designed that you select your own.
 *
 * \par Select custom TIM for delay functions
 *
 * By default, Systick timer is used for delay. If you want your custom timer,
 * open defines.h file, add lines below and edit for your needs.
 *
\code{.c}
//Select custom timer for delay, here is TIM2 selected.
//If you want custom TIMx, just replace number "2" for your TIM's number.
#define TM_DELAY_TIM				TIM2
#define TM_DELAY_TIM_IRQ			TIM2_IRQn
#define TM_DELAY_TIM_IRQ_HANDLER	TIM2_IRQHandler
\endcode
 *
 *
 * With this setting (using custom timer) you have better accuracy in "us" delay.
 * Also, you have to know, that if you want to use timer for delay, you have to include additional files:
 *
 *	- CMSIS:
 *		- STM32F4xx TIM
 *		- MISC
 *	- TM:
 *		TM TIMER PROPERTIES
 *
 * Delay functions (Delay, Delayms) are now Inline functions.
 * This allows faster execution and more accurate delay.
 *
 * If you are working with Keil uVision and you are using Systick for delay,
 * then set KEIL_IDE define in options for project:
 *	- Open "Options for target"
 *	- Tab "C/C++"
 *	- Under "Define" add "KEIL_IDE", without quotes
 *
 * \par Custom timers
 *
 * Custom timers are a way to make some tasks in a periodic value. 
 * As of version 2.4, delay library allows you to create custom timer which count DOWN and when it reaches zero, callback is called.
 *
 * You can use variable settings for count, reload value and auto reload feature.
 *
 * \par Changelog
 *
@verbatim
 Version 2.4
  - May 26, 2015
  - Added support for custom timers which can be called periodically

 Version 2.3
  - April 18, 2015
  - Fixed support for internal RC clock
  
 Version 2.2
  - January 12, 2015
  - Added support for custom function call each time 1ms interrupt happen
  - Function is called TM_DELAY_1msHandler(void), with __weak parameter
  - attributes.h file needed
  
 Version 2.1
  - GCC compiler fixes
  - Still prefer that you use TIM for delay if you are working with ARM-GCC compiler

 Version 2.0
  - November 28, 2014
  - Delay library has been totally rewritten. Because Systick is designed to be used
       in RTOS, it is not compatible to use it at the 2 places at the same time.
       For that purpose, library has been rewritten.
  - Read full documentation above

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC:       Only if you want to use TIMx for delay instead of Systick
 - STM32F4xx TIM:       Only if you want to use TIMx for delay instead of Systick
 - MISC
 - defines.h
 - TM TIMER PROPERTIES: Only if you want to use TIMx for delay instead of Systick
 - attribute.h
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "attributes.h"
/* If user selectable timer is selected for delay */
#if defined(TM_DELAY_TIM)
#include "misc.h"
#include "stm32f4xx_tim.h"
#include "tm_stm32f4_timer_properties.h"
#endif
#include "stdlib.h"

/**
 * @defgroup TM_DELAY_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Custom timer structure
 */
typedef struct {
	uint32_t ARR;             /*!< Auto reload value */
	uint32_t AutoReload;      /*!< Set to 1 if timer should be auto reloaded when it reaches zero */
	uint32_t CNT;             /*!< Counter value, counter counts down */
	uint8_t Enabled;          /*!< Set to 1 when timer is enabled */
	void (*Callback)(void *); /*!< Callback which will be called when timer reaches zero */
	void* UserParameters;     /*!< Pointer to user parameters used for callback function */
} TM_DELAY_Timer_t;

/**
 * @}
 */

/**
 * @defgroup TM_DELAY_Macros
 * @brief    Library Macros
 * @{
 */

/**
 * @brief  Number of allowed custom timers
 * @note   Should be changes in defines.h file if necessary
 */
#ifndef DELAY_MAX_CUSTOM_TIMERS
#define DELAY_MAX_CUSTOM_TIMERS   5
#endif

/* Memory allocation function */
#ifndef LIB_ALLOC_FUNC
#define LIB_ALLOC_FUNC    malloc
#endif

/* Memory free function */
#ifndef LIB_FREE_FUNC
#define LIB_FREE_FUNC     free
#endif

/**
 * @}
 */

/**
 * @defgroup TM_DELAY_Variables
 * @brief    Library Variables
 * @{
 */

/**
 * This variable can be used in main
 * It is automatically increased every time systick make an interrupt
 */
extern __IO uint32_t TM_Time;
extern __IO uint32_t TM_Time2;
extern __IO uint32_t mult;

/**
 * @}
 */

/**
 * @defgroup TM_DELAY_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @param  Delays for specific amount of microseconds
 * @param  micros: Time in microseconds for delay
 * @retval None
 * @note   Declared as static inline
 */
static __INLINE void Delay(uint32_t micros) {
#if defined(TM_DELAY_TIM)
	volatile uint32_t timer = TM_DELAY_TIM->CNT;

	do {
		/* Count timer ticks */
		while ((TM_DELAY_TIM->CNT - timer) == 0);

		/* Increase timer */
		timer = TM_DELAY_TIM->CNT;

		/* Decrease microseconds */
	} while (--micros);
#else
	uint32_t amicros;

	/* Multiply micro seconds */
	amicros = (micros) * (mult);

	#ifdef __GNUC__
		if (SystemCoreClock == 180000000 || SystemCoreClock == 100000000) {
			amicros -= mult;
		}
	#endif

	/* If clock is 100MHz, then add additional multiplier */
	/* 100/3 = 33.3 = 33 and delay wouldn't be so accurate */
	#if defined(STM32F411xE)
	amicros += mult;
	#endif

	/* While loop */
	while (amicros--);
#endif /* TM_DELAY_TIM */
}

/**
 * @param  Delays for specific amount of milliseconds
 * @param  millis: Time in milliseconds for delay
 * @retval None
 * @note   Declared as static inline
 */
static __INLINE void Delayms(uint32_t millis) {
	volatile uint32_t timer = TM_Time;

	/* Called from thread */
	if (!__get_IPSR()) {
		/* Wait for timer to count milliseconds */
		while ((TM_Time - timer) < millis) {
#ifdef DELAY_SLEEP
			/* Go sleep, wait systick interrupt */
			__WFI();
#endif
		}
	} else {
		/* Called from interrupt */
		while (millis) {
			if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
				millis--;
			}
		}
	}
}

/**
 * @brief  Initializes timer settings for delay
 * @note   This function will initialize Systick or user timer, according to settings
 * @param  None
 * @retval None
 */
void TM_DELAY_Init(void);

/**
 * @brief  Gets the TM_Time variable value
 * @param  None
 * @retval Current time in milliseconds
 */
#define TM_DELAY_Time()					(TM_Time)

/**
 * @brief  Sets value for TM_Time variable
 * @param  time: Time in milliseconds
 * @retval None
 */
#define TM_DELAY_SetTime(time)			(TM_Time = (time))

/**
 * @brief  Re-enables delay timer It has to be configured before with TM_DELAY_Init()
 * @note   This function enables delay timer. It can be systick or user selectable timer.
 * @param  None
 * @retval None
 */
void TM_DELAY_EnableDelayTimer(void);

/**
 * @brief  Disables delay timer
 * @note   This function disables delay timer. It can be systick or user selectable timer.
 * @param  None
 * @retval None
 */
void TM_DELAY_DisableDelayTimer(void);

/**
 * @brief  Gets the TM_Time2 variable value
 * @param  None
 * @retval Current time in milliseconds
 * @note   This is not meant for public use
 */
#define TM_DELAY_Time2()				(TM_Time2)

/**
 * @brief  Sets value for TM_Time variable
 * @param  time: Time in milliseconds
 * @retval None
 * @note   This is not meant for public use
 */
#define TM_DELAY_SetTime2(time)			(TM_Time2 = (time))

/**
 * @brief  Creates a new custom timer which has 1ms resolution
 * @note   It uses @ref malloc for memory allocation for timer structure
 * @param  ReloadValue: Number of milliseconds when timer reaches zero and callback function is called
 * @param  AutoReload: If set to 1, timer will start again when it reaches zero and callback is called
 * @param  StartTimer: If set to 1, timer will start immediately
 * @param  *TM_DELAY_CustomTimerCallback: Pointer to callback function which will be called when timer reaches zero
 * @param  *UserParameters: Pointer to void pointer to user parameters used as first parameter in callback function
 * @retval Pointer to allocated timer structure
 */
TM_DELAY_Timer_t* TM_DELAY_TimerCreate(uint32_t ReloadValue, uint8_t AutoReload, uint8_t StartTimer, void (*TM_DELAY_CustomTimerCallback)(void *), void* UserParameters);

/**
 * @brief  Deletes already allocated timer
 * @param  *Timer: Pointer to @ref TM_DELAY_Timer_t structure
 * @retval None
 */
void TM_DELAY_TimerDelete(TM_DELAY_Timer_t* Timer);

/**
 * @brief  Stops custom timer from counting
 * @param  *Timer: Pointer to @ref TM_DELAY_Timer_t structure
 * @retval Pointer to @ref TM_DELAY_Timer_t structure
 */
TM_DELAY_Timer_t* TM_DELAY_TimerStop(TM_DELAY_Timer_t* Timer);

/**
 * @brief  Starts custom timer counting
 * @param  *Timer: Pointer to @ref TM_DELAY_Timer_t structure
 * @retval Pointer to @ref TM_DELAY_Timer_t structure
 */
TM_DELAY_Timer_t* TM_DELAY_TimerStart(TM_DELAY_Timer_t* Timer);

/**
 * @brief  Resets custom timer counter value
 * @param  *Timer: Pointer to @ref TM_DELAY_Timer_t structure
 * @retval Pointer to @ref TM_DELAY_Timer_t structure
 */
TM_DELAY_Timer_t* TM_DELAY_TimerReset(TM_DELAY_Timer_t* Timer);

/**
 * @brief  Sets auto reload feature for timer
 * @note   Auto reload features is used for timer which starts again when zero is reached if auto reload active
 * @param  *Timer: Pointer to @ref TM_DELAY_Timer_t structure
 * uint8_t AutoReload: Set to 1 if you want to enable AutoReload or 0 to disable
 * @retval Pointer to @ref TM_DELAY_Timer_t structure
 */
TM_DELAY_Timer_t* TM_DELAY_TimerAutoReload(TM_DELAY_Timer_t* Timer, uint8_t AutoReload);

/**
 * @brief  Sets auto reload value for timer
 * @param  *Timer: Pointer to @ref TM_DELAY_Timer_t structure
 * @param  AutoReloadValue: Value for timer to be set when zero is reached and callback is called
 * @note   AutoReload feature must be enabled for timer in order to get this to work properly
 * @retval Pointer to @ref TM_DELAY_Timer_t structure
 */
TM_DELAY_Timer_t* TM_DELAY_TimerAutoReloadValue(TM_DELAY_Timer_t* Timer, uint32_t AutoReloadValue);

/**
 * @brief  User function, called each 1ms when interrupt from timer happen
 * @note   Here user should put things which has to be called periodically
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
__weak void TM_DELAY_1msHandler(void);


/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
