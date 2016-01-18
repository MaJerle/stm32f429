/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/10/library-42-control-rc-servo-stm32f4
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Servo library for STM32F4xx
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
#ifndef TM_SERVO_H
#define TM_SERVO_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_SERVO
 * @brief    SERVO library for STM32F4xx devices - http://stm32f4-discovery.com/2014/10/library-42-control-rc-servo-stm32f4
 * @{
 *
 * This library allows you to control RC servo with STM32F4xx.
 * 
 * Basically, only limitation for number of servos is number of timers in MCU.
 *
 * \par Calculations
 *
 * Equation for pulse length to get specific rotation is:
\code
Pulse length (degrees) = (MAX – MIN) * degrees / 180 + MIN
\endcode
 *
 * where:
 *  - MAX: maximum pulse length for servo, 2000us
 *  - MIN: minimum pulse length for servo, 1000us
 *
 * If you want to get rotation in degrees from know pulse length:
 *
\code
Degrees (pulse_length) = (pulse_length – MIN) * 180 / (MAX – MIN)
\endcode
 *
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - January 03, 2015
  - Added "micros" and "degrees" values to structure for user feedback where servo is located
  - Supports TM PWM 2.0 version library
 
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
 - TM PWM
 - TM TIMER PROPERTIES
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "defines.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_timer_properties.h"

/**
 * @defgroup TM_SERVO_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Minimal pulse length for servo in micros
 */
#ifndef SERVO_MICROS_MIN
#define SERVO_MICROS_MIN	1000
#endif

/**
 * @brief  Maximal pulse length for servo in micros
 */
#ifndef SERVO_MICROS_MAX
#define SERVO_MICROS_MAX	2000
#endif
	
/**
 * @}
 */
 
/**
 * @defgroup TM_SERVO_Typedefs
 * @brief    Library Typedefs
 * @{
 */
	
/**
 * @brief  Servo structure	
 */
typedef struct {
	TM_PWM_TIM_t PWM;           /*!< PWM settings */
	TIM_TypeDef* TIM;           /*!< Pointer to specific timer you will use for servo */
	TM_PWM_Channel_t Channel;   /*!< Output channel on specific timer */
	TM_PWM_PinsPack_t Pinspack; /*!< Pinspack for specific channel */
	float Degrees;              /*!< Position of servo in degrees */
	uint16_t Micros;            /*!< Pulse length in micro seconds for current servo position */
} TM_SERVO_t;

/**
 * @brief Results enumeration	
 */
typedef enum {
	TM_SERVO_Result_Ok = 0, /*!< Everything OK */
	TM_SERVO_Result_Error   /*!< An error occurred somewhere */
} TM_SERVO_Result_t;

/**
 * @}
 */

/**
 * @defgroup TM_SERVO_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes TIM and PWM for servo motor purpose
 * @param  *ServoStruct: Pointer to an empty @ref TM_SERVO_t servo structure
 * @param  *TIMx: Pointer to TIMx you will use for servo
 * @param  PWMChannel: Channel you will use for timer. This parameter can be a value of @ref TM_PWM_Channel_t enumeration
 * @param  Pinspack: Pinspack for channel. This parameter can be a value of @ref TM_PWM_PinsPack_t enumeration
 * @retval Member of TM_SERVO_Result_t
 */
TM_SERVO_Result_t TM_SERVO_Init(TM_SERVO_t* ServoStruct, TIM_TypeDef* TIMx, TM_PWM_Channel_t PWMChannel, TM_PWM_PinsPack_t Pinspack);

/**
 * @brief  Set rotation degrees for servo
 * @note   Degrees can be between 0 and 180
 * @param  *ServoStruct: Pointer to an @ref TM_SERVO_t servo structure
 * @param  degrees: Rotation in degrees, between 0 and 180
 * @retval Member of TM_SERVO_Result_t
 */
TM_SERVO_Result_t TM_SERVO_SetDegrees(TM_SERVO_t* ServoStruct, float degrees);

/**
 * @brief  Sets pulse length in microseconds
 * @param  *ServoStruct: Pointer to an @ref TM_SERVO_t servo structure
 * @param  micros: pulse length in microseconds
 * @retval Member of TM_SERVO_Result_t
 */
TM_SERVO_Result_t TM_SERVO_SetMicros(TM_SERVO_t* ServoStruct, uint16_t micros);

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
