/**
 *	PWM library for STM32F4xx devices, supporting all possible timers with PWM feature
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-33-pwm-for-stm32f4xx/
 *	@version 	v1.1
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
 * Version 1.1
 * 	- 15.09.2014
 * 	- Split timer properties with enable/disable clocks into new library. This library will be used 
 * 	in each project where timers are included.
 *
 * PWM pins are connected to fixed possible pins
 *
 * 	TIMER	|CHANNEL 1				|CHANNEL 2				|CHANNEL 3				|CHANNEL 4
 * 			|PP1	PP2		PP3		|PP1	PP2		PP3		|PP1	PP2		PP3		|PP1	PP2		PP3
 *
 * 	TIM 2	|PA0	PA5		PA15	|PA1	PB3		-		|PA2	PB10	-		|PA3	PB11	-
 * 	TIM 3	|PA6	PB4		PC6		|PA7	PB5		PC7		|PB0	PC8		-		|PB1	PC9		-
 * 	TIM 4	|PB6	PD12	-		|PB7	PD13	-		|PB8	PD14	-		|PB9	PD15	-
 * 	TIM 5	|PA0	PH10	-		|PA1	PH11	-		|PA2	PH12	-		|PA3	PI0		-
 * 	TIM 8	|PC6	PI5		-		|PC7	PI6		-		|PC8	PI7		-		|PC9	PI2		-
 * 	TIM 9	|PA2	PE5		-		|PA3	PE6		-		|-		-		-		|-		-		-
 * 	TIM 10	|PB8	PF6		-		|-		-		-		|-		-		-		|-		-		-
 * 	TIM 11	|PB9	PF7		-		|-		-		-		|-		-		-		|-		-		-
 * 	TIM 12	|PB14	PH6		-		|PB15	PH9		-		|-		-		-		|-		-		-
 * 	TIM 13	|PA6	PF8		-		|-		-		-		|-		-		-		|-		-		-
 * 	TIM 14	|PA7	PF9		-		|-		-		-		|-		-		-		|-		-		-
 *
 * 	- PPx: Pins Pack 1 to 3, for 3 possible channel outputs on timer.
 *
 * Notes on table above
 * 	- Not all timers are available on all STM32F4xx devices
 * 	- All timers have 16bit prescaler
 * 	- TIM6 and TIM7 don’t have PWM feature, they are only basic timers
 * 	- TIM2 and TIM5 are 32bit timers
 * 	- TIM9 and TIM12 have two PWM channels
 * 	- TIM10, TIM11, TIM13 and TIM14 have only one PWM channel
 * 	- All channels at one timer have the same PWM frequency!
 */
#ifndef TM_PWM_H
#define TM_PWM_H 110
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx TIM
 * - defines.h
 * - TM TIMER PROPERTIES
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "tm_stm32f4_timer_properties.h"
#include "defines.h"

/**
 * Result enumeration
 *
 * Parameters
 * 	- TM_PWM_Result_Ok
 * 		Everything OK
 * 	- TM_PWM_Result_FrequencyTooHigh
 * 		You select too high frequency for timer for PWM
 * 	- TM_PWM_Result_FrequencyTooLow
 * 		Prescaler value is too big for selected frequency
 * 	- TM_PWM_Result_PulseTooHigh
 * 		Pulse for Output compare is larger than timer period
 * 	- TM_PWM_Result_TimerNotValid
 * 		Selected timer is not valid. This happens when you select TIM6 or TIM7,
 * 		because they don't have PWM capability.
 * 	- TM_PWM_Result_ChannelNotValid
 * 		Channel is not valid. Some timers don't have all 4 timers available for PWM
 * 	- TM_PWM_Result_PinNotValid
 * 		Selected pin is not valid. Most channels have only 2 possible pins for PWM,
 * 		but some 3. If you select pin 3 on channel that don't have 3rd pin available
 * 		for PWM, this will be returned
 */
typedef enum {
	TM_PWM_Result_Ok = 0,
	TM_PWM_Result_FrequencyTooHigh,
	TM_PWM_Result_FrequencyTooLow,
	TM_PWM_Result_PulseTooHigh,
	TM_PWM_Result_TimerNotValid,
	TM_PWM_Result_ChannelNotValid,
	TM_PWM_Result_PinNotValid
} TM_PWM_Result_t;

/**
 * Timer data
 *
 * Parameters:
 * 	- uint32_t Period
 * 		Period used, set on initialization for PWM
 * 	- uint32_t Prescaler
 * 		Prescaler used for PWM frequency
 * 	- uint32_t Frequency
 * 		PWM frequency used
 * 	- uint32_t Micros
 * 		Microseconds used for one period.
 *		This is not useful in large pwm frequency, but good for controlling servos or similar,
 *		Where you need exact time of pulse high
 */
typedef struct {
	uint32_t Period;
	uint32_t Prescaler;
	uint32_t Frequency;
	uint32_t Micros;
} TM_PWM_TIM_t;

/**
 * Channel selected for PWM on specific timer
 *
 */
typedef enum {
	TM_PWM_Channel_1,
	TM_PWM_Channel_2,
	TM_PWM_Channel_3,
	TM_PWM_Channel_4
} TM_PWM_Channel_t;

/**
 * Pin selected for corresponding channel on specific channel
 *
 */
typedef enum {
	TM_PWM_PinsPack_1,
	TM_PWM_PinsPack_2,
	TM_PWM_PinsPack_3
} TM_PWM_PinsPack_t;

/**
 * Initialize specific timer for PWM capability
 *
 * Parameters:
 * 	- TIM_TypeDef* TIMx
 * 		Pointer to selected timer, you want to use for PWM
 * 	- TM_PWM_TIM_t* TIM_Data
 * 		Pointer to blank TM_PWM_TIM_t structure.
 * 		Here will init function save all data for specific timer
 * 	- uint32_t PWMFrequency
 * 		Select custom frequency for PWM
 */
extern TM_PWM_Result_t TM_PWM_InitTimer(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, double PWMFrequency);

/**
 * Initialize channel used for specific timer
 *
 * Parameters:
 * 	- TIM_TypeDef* TIMx
 * 		Pointer to initialized timer for PWM
 * 	- TM_PWM_Channel_t Channel
 * 		Select channel you will use on specific timer
 * 	- TM_PWM_PinsPack_t PinsPack
 * 		Select which pinspack you will use for pin
 */
extern TM_PWM_Result_t TM_PWM_InitChannel(TIM_TypeDef* TIMx, TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);

/**
 * Set PWM value for specific timer and channel
 *
 * Parameters:
 *	- TIM_TypeDef* TIMx
 *		Pointer to initialized timer for PWM
 *	- TM_PWM_TIM_t* TIM_Data
 *		Pointer to initialized timer data
 *	- TM_PWM_Channel_t Channel
 *		Channel for which you set value
 *	- uint32_t Pulse
 *		Pulse, to be set for compare match
 */
extern TM_PWM_Result_t TM_PWM_SetChannel(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t Pulse);

/**
 * Set PWM value for specific timer and channel with percentage feature
 *
 * Parameters:
 *	- TIM_TypeDef* TIMx
 *		Pointer to initialized timer for PWM
 *	- TM_PWM_TIM_t* TIM_Data
 *		Pointer to initialized timer data
 *	- TM_PWM_Channel_t Channel
 *		Channel for which you set value
 *	- float percent
 *		Percentage from 0 to 100, to set PWM value
 *
 */
extern TM_PWM_Result_t TM_PWM_SetChannelPercent(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, float percent);

/**
 * Set PWM value for specific timer and channel with pulse high time feature.
 * You have to specify amount of micro seconds pulse will be high for specific timer and channel.
 * This method is not so good on PWM large than 100k, because you cannot set nice and correct settings, with microseconds
 * accuracy. It is perfect, and was designed for low frequencies, eg. use with servos, where you have exact amount of time
 * for servo's rotation.
 *
 * Parameters:
 *	- TIM_TypeDef* TIMx
 *		Pointer to initialized timer for PWM
 *	- TM_PWM_TIM_t* TIM_Data
 *		Pointer to initialized timer data
 *	- TM_PWM_Channel_t Channel
 *		Channel for which you set value
 *	- uint32_t micros
 *		Microseconds for pulse high on PWM. Cannot be large than timer period in micros.
 *		PWM 1kHz = Timer period = 1000000 / 1000 = 1000us. This parameter can not be greater than 1000us in this case.
 */
extern TM_PWM_Result_t TM_PWM_SetChannelMicros(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t micros);

#endif

