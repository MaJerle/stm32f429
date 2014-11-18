/**
 *	Servo library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-42-control-rc-servo-stm32f4
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
 * This library allows you to control RC servo with STM32F4xx.
 * 
 * Basically, only limitation for number of servos is number of timers in MCU.
 */
#ifndef TM_SERVO_H
#define TM_SERVO_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif
	
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx TIM
 * - defines.h
 * - TM PWM
 * - TM TIMER PROPERTIES
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "defines.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_timer_properties.h"

/* Minimal pulse length */
#ifndef SERVO_MICROS_MIN
#define SERVO_MICROS_MIN	1000
#endif
/* Maximal pulse length */
#ifndef SERVO_MICROS_MAX
#define SERVO_MICROS_MAX	2000
#endif
	
/**
 * Servo struct
 *
 * Parameters:
 * 	- TM_PWM_TIM_t PWM:
 * 		PWM settings
 * 	- TIM_TypeDef* TIM:
 * 		Pointer to specific timer you will use for servo
 * 	- TM_PWM_Channel_t Channel:
 * 		Output channel on specific timer
 * 	- TM_PWM_PinsPack_t Pinspack:
 * 		Pinspack for specific channel
 */
typedef struct {
	TM_PWM_TIM_t PWM;
	TIM_TypeDef* TIM;
	TM_PWM_Channel_t Channel;
	TM_PWM_PinsPack_t Pinspack;
} TM_SERVO_t;

/**
 * Results enumeration
 *
 * Parameters:
 * 	- TM_SERVO_Result_Ok:
 * 		Everything OK
 * 	- TM_SERVO_Result_Error
 * 		An error occured somewhere
 */
typedef enum {
	TM_SERVO_Result_Ok = 0,
	TM_SERVO_Result_Error
} TM_SERVO_Result_t;

/**
 * Initialize servo
 *
 * Parameters:
 * 	- TM_SERVO_t* ServoStruct:
 * 		Pointer to an empty servo struct
 * 	- TIM_TypeDef* TIMx:
 * 		Pointer to TIMx you will use for servo
 * 	- TM_PWM_Channel_t PWMChannel:
 * 		Channel you will use for timer
 * 	- TM_PWM_PinsPack_t Pinspack
 * 		Pinspack for channel
 */
extern TM_SERVO_Result_t TM_SERVO_Init(TM_SERVO_t* ServoStruct, TIM_TypeDef* TIMx, TM_PWM_Channel_t PWMChannel, TM_PWM_PinsPack_t Pinspack);

/**
 * Set rotation degrees for servo.
 * Degrees can be between 0 and 180.
 *
 * Parameters:
 * 	- TM_SERVO_t* ServoStruct:
 * 		Pointer to servo struct
 * 	- float degrees
 * 		rotation in degrees, between 0 and 180
 */
extern TM_SERVO_Result_t TM_SERVO_SetDegrees(TM_SERVO_t* ServoStruct, float degrees);

/**
 * Set pulse length in microseconds
 *
 * Parameters:
 * 	- TM_SERVO_t* ServoStruct:
 * 		Pointer to servo struct
 * 	- uint16_t micros:
 * 		pulse length in microseconds
 */
extern TM_SERVO_Result_t TM_SERVO_SetMicros(TM_SERVO_t* ServoStruct, uint16_t micros);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
