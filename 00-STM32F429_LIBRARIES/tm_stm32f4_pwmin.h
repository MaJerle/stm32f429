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
extern "C" {
#endif
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_PWMIN
 * @brief    PWM IN library for STM32F4xx devices - http://stm32f4-discovery.com/2015/01/library-48-measure-pwm-input-signal-with-stm32f4
 * @{
 *
 * This library allows you to measure PWM input signal (Frequency, Duty Cycle) using TIM peripheral.
 *
 * \par Pinout
 *
@verbatim
TIMER   |CHANNEL 1            |CHANNEL 2         
        |PP1    PP2    PP3    |PP1    PP2    PP3
                                                
TIM 1   |PA8    PE9    -      |PA9    PE10   -	
TIM 2   |PA0    PA5    PA15   |PA1    PB3    -	
TIM 3   |PA6    PB4    PC6    |PA7    PB5    PC7 
TIM 4   |PB6    PD12   -      |PB7    PD13   -	
TIM 5   |PA0    PH10   -      |PA1    PH11   -	
TIM 8   |PC6    PI5    -      |PC7    PI6    -	
TIM 9   |PA2    PE5    -      |PA3    PE6    -	
TIM 10  |PB8    PF6    -      |-      -      -	
TIM 11  |PB9    PF7    -      |-      -      -	
TIM 12  |PB14   PH6    -      |PB15   PH9    -	
TIM 13  |PA6    PF8    -      |-      -      -	
TIM 14  |PA7    PF9    -      |-      -      -	
@endverbatim
 *
 * 	- PPx: Pins Pack 1 to 3, for 3 possible channel outputs on timer.
 *
 * Notes on table above
 * 	- Not all timers are available on all STM32F4xx devices
 * 	- All timers have 16-bit prescaler
 * 	- TIM6 and TIM7 don't have PWM input feature, they are only basic timers
 * 	- TIM2 and TIM5 are 32bit timers
 * 	- TIM10, TIM11, TIM13 and TIM14 have only one PWM channel
 * 	- One timer can measure just 1 frequency and duty cycle at a time!
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 20, 2015
  - Support for new GPIO library
  
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx GPIO
 - STM32F4xx TIM
 - defines.h
 - TM TIMER PROPERTIES
 - TM GPIO
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_timer_properties.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_PWMIN_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  PWMIN Timer global interrupt for NVIC
 */
#ifndef PWMIN_PRIORITY
#define PWMIN_PRIORITY		0x01
#endif

/**
 * @}
 */

/**
 * @defgroup TM_PWMIN_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  PWM Input result enumeration
 */
typedef enum {
	TM_PWMIN_Result_Ok = 0,          /*!< Everything OK */
	TM_PWMIN_Result_TimerNotValid,   /*!< Invalid timer selected for PWM input capture */
	TM_PWMIN_Result_ChannelNotValid, /*!< Invalid input channel selected for timer */
	TM_PWMIN_Result_PinNotValid      /*!< Invalid pin selected for timer */
} TM_PWMIN_Result_t;

/**
 * @brief  PWM Input working struct
 */
typedef struct {
	float Frequency;                  /*!< Measured frequency on input pin */
	float DutyCycle;                  /*!< Measured duty cycle on input pin */
	TIM_TypeDef* __TIM;               /*!< Pointer to timer used for measure. For private use only */
	TM_TIMER_PROPERTIES_t __TIM_Data; /*!< Settings about timer. For private use only */
	float __Freq;                     /*!< Temporary frequency value. For private use only */
	float __Duty;                     /*!< Temporary duty cycle value. For private use only */
	uint8_t __SubPriority;            /*!< Subpriority for NVIC. For private use only */
} TM_PWMIN_t;

/**
 * @brief  Input channel for measure PWM selection
 */
typedef enum {
	TM_PWMIN_Channel_1 = 0, /*!< Use Channel 1 for PWMIN measure */
	TM_PWMIN_Channel_2      /*!< Use Channel 2 for PWMIN measure */
} TM_PWMIN_Channel_t;

/**
 * @brief  Pinspack for PWMIN channel pin
 */
typedef enum {
	TM_PWMIN_PinsPack_1 = 0, /*!< Use pinspack1 pin from PWMIN pinout table */
	TM_PWMIN_PinsPack_2,     /*!< Use pinspack2 pin from PWMIN pinout table */
	TM_PWMIN_PinsPack_3      /*!< Use pinspack3 pin from PWMIN pinout table */
} TM_PWMIN_PinsPack_t;

/**
 * @}
 */

/**
 * @defgroup TM_PWMIN_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes and prepares timer for PWM input capture
 * @param  *TIMx: Pointer to TIM you will use for PWM input capture
 * @param  *PWMIN_Data: Pointer to an empty @ref TM_PWMIN_t structure
 * @param  PWMIN_Channel: Channel you will use on timer. This parameter can be a value of @ref TM_PWMIN_Channel_t enumeration
 * @param  PinsPack: Pinspack you will use for your channel. This parameter can be a value of @ref TM_PWMIN_PinsPack_t enumeration
 * @param  MinExpectedFrequency: Type minimal input frequency you expect in input.
 *            If you don't know what to expect, set to 1
 * @param  TIMx_IRQn: IRQ for NVIC settings. For TIM2 it can be TIM2_IRQn, for TIM3,4,5,... it can be TIMx_IRQn, where x is a number of timer.
 *            This parameter can be a value of @ref IRQn_Type enumeration
 * @retval Member of TM_PWMIN_Result_t 
 */
TM_PWMIN_Result_t TM_PWMIN_InitTimer(TIM_TypeDef* TIMx, TM_PWMIN_t* PWMIN_Data, TM_PWMIN_Channel_t PWMIN_Channel, TM_PWMIN_PinsPack_t PinsPack, float MinExpectedFrequency, IRQn_Type TIMx_IRQn);

/**
 * @brief  Reads data from measured input PWM signal
 * @note   Data are valid if frequency is greater than 0
 * @param  *PWMIN_Data: Pointer to TM_PWMIN_t struct.
 *		      Frequency and Duty cycle will be stored in this structure.
 * @retval Member of @ref TM_PWMIN_Result_t
 */
TM_PWMIN_Result_t TM_PWMIN_Get(TM_PWMIN_t* PWMIN_Data);

/**
 * @brief  Most important function. 
 *         This function handles TIMx interrupts to calculate PWM input data.
 * @note   You need to call it in your TIMx handler. 
@verbatim
//For TIM2, you will do:
void TIM2_IRQHandler(void) {
    TM_PWMIN_InterruptHandler(&PWMIN_TIM2_Data);
}
@endverbatim
 * @param  *PWMIN_Data: Pointer to @ref TM_PWMIN_t structure
 * @retval Member of @ref TM_PWMIN_Result_t
 */
TM_PWMIN_Result_t TM_PWMIN_InterruptHandler(TM_PWMIN_t* PWMIN_Data);

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
