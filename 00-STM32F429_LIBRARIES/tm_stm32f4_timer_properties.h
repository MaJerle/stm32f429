/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Timer properties for all STM32F4xx timers
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
#ifndef TM_TIMER_PROPERTIES_H
#define TM_TIMER_PROPERTIES_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_TIMER_PROPERTIES
 * @brief    Timer properties for all STM32F4xx timers
 * @{
 *
 * This library allows you to enable/disable clock for specific timer,
 * and returns you settings for timer, ex. max period, max prescaler and timer's clock
 *
 * Also, it is used in every library where timers are used, such as PWM, PWMIN and so on.
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 15, 2015
  - Removed unnecessary function calls, used register access

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx TIM
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "defines.h"

/**
 * @defgroup TM_TIMER_PROPERTIES_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Timer result enumeration 		
 */
typedef enum {
	TM_TIMER_PROPERTIES_Result_Ok,               /*!< Everything OK */
	TM_TIMER_PROPERTIES_Result_Error,            /*!< An error occurred */
	TM_TIMER_PROPERTIES_Result_TimerNotValid,    /*!< Timer is not valid */
	TM_TIMER_PROPERTIES_Result_FrequencyTooHigh, /*!< Frequency for timer is too high */
	TM_TIMER_PROPERTIES_Result_FrequencyTooLow   /*!< Frequency for timer is too low */
} TM_TIMER_PROPERTIES_Result_t;

/**
 * @brief  Structure for timer data
 */
typedef struct {
	uint32_t TimerFrequency; /*!< timer's working frequency */
	uint32_t MaxPeriod;      /*!< Max timer period */
	uint32_t MaxPrescaler;   /*!< Max timer prescaler */
	uint32_t Period;         /*!< Timer's working period */
	uint32_t Prescaler;      /*!< Timer's working prescaler */
	uint32_t Frequency;      /*!< Timer's reload frequency */
} TM_TIMER_PROPERTIES_t;

/**
 * @}
 */

/**
 * @defgroup TM_TIMER_PROPERTIES_Functions
 * @brief    Library Functions
 * @{
 */
 
/**
 * @brief  Gets timer properties
 * @param  *TIMx: Timer used to get settings for
 * @param  *Timer_Data: Pointer to @ref TM_TIMER_PROPERTIES_t structure to store data to
 * @retval Member of @ref TM_TIMER_PROPERTIES_Result_t
 */
TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_GetTimerProperties(TIM_TypeDef* TIMx, TM_TIMER_PROPERTIES_t* Timer_Data);

/**
 * @brief  Generates period and prescaler for given timer frequency
 * @param  *Timer_Data: Pointer to @ref TM_TIMER_PROPERTIES_t structure
 * @param  frequency: Frequency used to generate period and prescaler values
 * @retval Member of @ref TM_TIMER_PROPERTIES_Result_t
 */
TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_GenerateDataForWorkingFrequency(TM_TIMER_PROPERTIES_t* Timer_Data, double frequency);

/**
 * @brief  Enables timer clock
 * @param  *TIMx: Timer used to enable clock for it
 * @retval Member of @ref TM_TIMER_PROPERTIES_Result_t
 */
TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_EnableClock(TIM_TypeDef* TIMx);

/**
 * @brief  Disables timer clock
 * @param  *TIMx: Timer used to disable clock for it
 * @retval Member of @ref TM_TIMER_PROPERTIES_Result_t
 */
TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_DisableClock(TIM_TypeDef* TIMx);

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

