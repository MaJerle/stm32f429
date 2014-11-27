/**
 *	Timer properties for all STM32F4xx timers
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
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
 * This library allows you to enable/disable clock for specific timer,
 * and returns you settings for timer, ex. max period, max prescaler and timer's clock
 *
 * Also, it is used in every library where timers are used. 
 */
#ifndef TM_TIMER_PROPERTIES_H
#define TM_TIMER_PROPERTIES_H	100
/**
 * Dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx TIM
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "defines.h"

/**
 * Result enumeration
 *
 * Parameters:
 * 	- TM_TIMER_PROPERTIES_Result_Ok:
 * 		Everything OK
 * 	- TM_TIMER_PROPERTIES_Result_Error:
 * 		An error occured
 * 	- TM_TIMER_PROPERTIES_Result_TimerNotValid:
 * 		Timer is not valid
 * 	- TM_TIMER_PROPERTIES_Result_FrequencyTooHigh:
 * 		Frequency for timer is too high
 * 	- TM_TIMER_PROPERTIES_Result_FrequencyTooLow:
 * 		Frequency for timer is too low
 */
typedef enum {
	TM_TIMER_PROPERTIES_Result_Ok,
	TM_TIMER_PROPERTIES_Result_Error,
	TM_TIMER_PROPERTIES_Result_TimerNotValid,
	TM_TIMER_PROPERTIES_Result_FrequencyTooHigh,
	TM_TIMER_PROPERTIES_Result_FrequencyTooLow
} TM_TIMER_PROPERTIES_Result_t;

/**
 * Struct for timer data
 * 
 * Parameters:
 * 	- uint32_t TimerFrequency:
 * 		timer's working frequency
 * 	- uint32_t MaxPeriod:
 * 		Max timer period
 * 	- uint32_t MaxPrescaler:
 * 		Max timer prescaler
 * 	- uint32_t Period:
 * 		Timer's working period
 * 	- uint32_t Prescaler:
 * 		Timer's working prescaler
 * 	- uint32_t Frequency:
 * 		Timer's reload frequency
 */
typedef struct {
	uint32_t TimerFrequency;
	uint32_t MaxPeriod;
	uint32_t MaxPrescaler;
	uint32_t Period;
	uint32_t Prescaler;
	uint32_t Frequency;
} TM_TIMER_PROPERTIES_t;

/**
 * Returns you a timer properties
 * 
 * Parameters:
 *	- TIM_TypeDef* TIMx:
 *		Timer used to get settings for
 *	- TM_TIMER_PROPERTIES_t* Timer_Data:
 *		Pointer to TM_TIMER_PROPERTIES_t struct to store data to
 *		
 * Member of TM_TIMER_PROPERTIES_Result_t is returned
 */
extern TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_GetTimerProperties(TIM_TypeDef* TIMx, TM_TIMER_PROPERTIES_t* Timer_Data);

/**
 * Generate period and prescaller for given timer frequency
 * 
 * Parameters:
 *	- TM_TIMER_PROPERTIES_t* Timer_Data:
 *		Pointer for timer data
 *	- uint32_t frequency:
 *		Frequency used
 *		
 * Member of TM_TIMER_PROPERTIES_Result_t is returned
 */
extern TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_GenerateDataForWorkingFrequency(TM_TIMER_PROPERTIES_t* Timer_Data, double frequency);

/**
 * Enable timer's clock
 * 
 * Parameters:
 * 	- TIM_TypeDef* TIMx:
 * 		Timer to enable clock for
 * 		
 * Member of TM_TIMER_PROPERTIES_Result_t is returned
 */
extern TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_EnableClock(TIM_TypeDef* TIMx);

/**
 * Disable timer's clock
 * 
 * Parameters:
 * 	- TIM_TypeDef* TIMx:
 * 		Timer to disable clock for
 * 		
 * Member of TM_TIMER_PROPERTIES_Result_t is returned
 */
extern TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_DisableClock(TIM_TypeDef* TIMx);
 
#endif

