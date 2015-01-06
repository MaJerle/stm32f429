/**
 *	PWM IN library for STM32F4xx devices
 *	It allows you to measure PWM signal with timer
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/01/library-48-measure-pwm-input-signal-with-stm32f4
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
 *
 * PWM input pins are connected to fixed possible pins
 *
 * 	TIMER	|CHANNEL 1				|CHANNEL 2			
 * 			|PP1	PP2		PP3		|PP1	PP2		PP3
 *
 *	TIM 1	|PA8	PE9		-		|PA9	PE10	-
 * 	TIM 2	|PA0	PA5		PA15	|PA1	PB3		-
 * 	TIM 3	|PA6	PB4		PC6		|PA7	PB5		PC7
 * 	TIM 4	|PB6	PD12	-		|PB7	PD13	-
 * 	TIM 5	|PA0	PH10	-		|PA1	PH11	-
 * 	TIM 8	|PC6	PI5		-		|PC7	PI6		-
 * 	TIM 9	|PA2	PE5		-		|PA3	PE6		-
 * 	TIM 10	|PB8	PF6		-		|-		-		-
 * 	TIM 11	|PB9	PF7		-		|-		-		-
 * 	TIM 12	|PB14	PH6		-		|PB15	PH9		-
 * 	TIM 13	|PA6	PF8		-		|-		-		-
 * 	TIM 14	|PA7	PF9		-		|-		-		-
 *
 * 	- PPx: Pins Pack 1 to 3, for 3 possible channel outputs on timer.
 *
 * Notes on table above
 * 	- Not all timers are available on all STM32F4xx devices
 * 	- All timers have 16bit prescaler
 * 	- TIM6 and TIM7 don’t have PWM input feature, they are only basic timers
 * 	- TIM2 and TIM5 are 32bit timers
 * 	- TIM10, TIM11, TIM13 and TIM14 have only one PWM channel
 * 	- One timer can measure just 1 frequency and duty cycle at a time!
 */
#ifndef TM_PWMIN_H
#define TM_PWMIN_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM TIMER PROPERTIES
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_timer_properties.h"
#include "defines.h"

/* PWM IN timer global priority */
#ifndef PWMIN_PRIORITY
#define PWMIN_PRIORITY		0x01
#endif

/**
 * PWM Input result enumeration
 *
 * Parameters:
 *	- TM_PWMIN_Result_Ok:
 *		Everything OK
 *	- TM_PWMIN_Result_TimerNotValid:
 *		Invalid timer selected for PWM input capture
 *	- TM_PWMIN_Result_ChannelNotValid:
 *		Invalid input channel selected for timer
 *	- TM_PWMIN_Result_PinNotValid:
 *		Invalid pin selected for timer
 */
typedef enum {
	TM_PWMIN_Result_Ok = 0,
	TM_PWMIN_Result_TimerNotValid,
	TM_PWMIN_Result_ChannelNotValid,
	TM_PWMIN_Result_PinNotValid
} TM_PWMIN_Result_t;

/**
 * PWM Input working struct
 *
 * Parameters:
 *	- float Frequency:
 *		Public, Measured frequency on input pin
 *	- float DutyCycle:
 *		Public, Measured duty cycle on input pin
 * 	- TIM_TypeDef* __TIM:
 *		Private, Pointer to timer used for measure
 * 	- TM_TIMER_PROPERTIES_t __TIM_Data:
 *		Settings about timer
 * 	- float __Freq:
 *		Private, temporary frequency value
 * 	- float __Duty:
 *		Private, temporary duty cycle value
 * 	- uint8_t __SubPriority:
 *		Private, NVIC subpriority
 */
typedef struct {
	/* Public */
	float Frequency;
	float DutyCycle;
	/* Private */
	TIM_TypeDef* __TIM;
	TM_TIMER_PROPERTIES_t __TIM_Data;
	float __Freq;
	float __Duty;
	uint8_t __SubPriority;
} TM_PWMIN_t;

/**
 * Select input channel for PWM input on specific timer
 *
 */
typedef enum {
	TM_PWMIN_Channel_1 = 0,
	TM_PWMIN_Channel_2
} TM_PWMIN_Channel_t;

/**
 * Select pin for input channel on specific timer
 *
 */
typedef enum {
	TM_PWMIN_PinsPack_1 = 0,
	TM_PWMIN_PinsPack_2,
	TM_PWMIN_PinsPack_3
} TM_PWMIN_PinsPack_t;

/**
 * Initialize and prepare timer for input PWM capture
 *
 * Parameters:
 *	- TIM_TypeDef* TIMx:
 *		Timer you will use for input PWM capture
 *	- TM_PWMIN_t* PWMIN_Data:
 *		Pointer to an empty TM_PWMIN_t struct
 *	- TM_PWMIN_Channel_t PWMIN_Channel:
 *		Channel you will use on timer.
 *	- TM_PWMIN_PinsPack_t PinsPack:
 *		Pinspack you will use for your channel
 *	- float MinExpectedFrequency:
 *		Type minimal input frequency you expect in input. If you don't know what to expect, set to 1
 *	- IRQn_Type TIMx_IRQn:
 *		IRQ for NVIC settings. For TIM2 it can be TIM2_IRQn, for TIM3,4,5,... it can be TIMx_IRQn, where x is a number of timer
 *
 * Member of TM_PWMIN_Result_t is returned
 */
extern TM_PWMIN_Result_t TM_PWMIN_InitTimer(TIM_TypeDef* TIMx, TM_PWMIN_t* PWMIN_Data, TM_PWMIN_Channel_t PWMIN_Channel, TM_PWMIN_PinsPack_t PinsPack, float MinExpectedFrequency, IRQn_Type TIMx_IRQn);

/**
 * Read data from measured input PWM signal
 *
 * Parameters:
 *	- TM_PWMIN_t* PWMIN_Data:
 *		Pointer to TM_PWMIN_t struct
 *		Frequency and Dutycycle will be stored in this structure
 * 
 * Data are valid if frequency is greater than 0
 *
 * Member of TM_PWMIN_Result_t is returned
 */
extern TM_PWMIN_Result_t TM_PWMIN_Get(TM_PWMIN_t* PWMIN_Data);

/**
 * Most important function
 * This function handles TIMx interrupts to calculate PWM input data.
 * You need to call it in your TIMx handler. 
 *
 * For TIM2, you will do:
 *	void TIM2_IRQHandler(void) {
 *		TM_PWMIN_InterruptHandler(&PWMIN_TIM2_Data);
 *	}
 *
 * Parameters:
 *	- TM_PWMIN_t* PWMIN_Data:
 *		Pointer to TM_PWMIN_t struct
 *
 * Member of TM_PWMIN_Result_t is returned
 */
extern TM_PWMIN_Result_t TM_PWMIN_InterruptHandler(TM_PWMIN_t* PWMIN_Data);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
