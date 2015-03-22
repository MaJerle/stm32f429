/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/08/library-26-rotary-encoder-stm32f4
 * @version v2.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Rotary encoder library for STM32F4xx devices
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
#ifndef TM_RE_H
#define TM_RE_H 200

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_ROTARY_ENCODER
 * @brief    Rotary encoder library for STM32F4xx devices - http://stm32f4-discovery.com/2014/08/library-26-rotary-encoder-stm32f4
 * @{
 *
 * This library supports up to 16 rotary encoders at a time.
 * It does not use TIM's rotary input feature but external interrupts for handling.
 *
 *
 * \par Pinout
 *
 * Pinout is fully selectable on rotary initialization.
 *
 * \par Encoder modes
 *
 * By default, if you turn encoder CW, you will increase counter, if CCW, you will decrease counter. Basically, this depends on your wiring for rotary.
 *
 * Library allows you to turn this mode "on the fly" anytime you want. Look for @ref TM_RE_SetMode() function for that.
 *
 * \par Changelog
 *
@verbatim
 Version 2.0
  - March 22, 2015
  - Library has been rewritten and now supports up to 16 rotary encoders at a time
  
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
 - STM32F4xx EXTI
 - STM32F4xx SYSCFG
 - misc.h
 - defines.h
 - TM EXTI
 - TM GPIO
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_exti.h"
 
/**
 * @defgroup TM_ROTARY_ENCODER_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Rotary encoder rotation status
 */
typedef enum {
	TM_RE_Rotate_Increment, /*!< Encoder was incremented */
	TM_RE_Rotate_Decrement, /*!< Encoder was decremented */
	TM_RE_Rotate_Nothing    /*!< Encoder stop at it was before */
} TM_RE_Rotate_t;

/**
 * @brief  Rotary encoder mode selection for rotation
 */
typedef enum {
	TM_RE_Mode_Zero, /*!< Rotary encoder mode zero. It is used for direction when it will be increment od decrement, default used */
	TM_RE_Mode_One   /*!< Rotary encoder mode one. It is used for direction when it will be increment od decrement */
} TM_RE_Mode_t;

/**
 * @brief  Rotary main working structure 
 */
typedef struct {
	int32_t Absolute;        /*!< Absolute rotation from beginning, for public use */
	int32_t Diff;            /*!< Rotary difference from last check, for public use */
	TM_RE_Rotate_t Rotation; /*!< Increment, Decrement or nothing, for public use */
	TM_RE_Mode_t Mode;       /*!< Rotary encoder mode selected */
	uint8_t LastA;           /*!< Last status of A pin when checking. Meant for private use */
	int32_t RE_Count;        /*!< Temporary variable to store data between rotation and user check */
	GPIO_TypeDef* GPIO_A;    /*!< Pointer to GPIOx for Rotary encode A pin. Meant for private use */
	GPIO_TypeDef* GPIO_B;    /*!< Pointer to GPIOx for Rotary encode B pin. Meant for private use */
	uint16_t GPIO_PIN_A;     /*!< GPIO pin for rotary encoder A pin. This pin is also set for interrupt */
	uint16_t GPIO_PIN_B;     /*!< GPIO pin for rotary encoder B pin. */
} TM_RE_t;

/**
 * @}
 */

/**
 * @defgroup TM_ROTARY_ENCODER_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Prepare Rotary Encoder to work
 * @param  *data: Pointer to @ref TM_RE_t structure
 * @retval None
 */
void TM_RE_Init(TM_RE_t* data, GPIO_TypeDef* GPIO_A_Port, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_Port, uint16_t GPIO_B_Pin);

/**
 * @brief  Set rotary encoder custom mode
 * @param  *data: Pointer to @ref TM_RE_t structure for specific rotary encoder input
 * @param  mode: Rotary mode you will use. This parameter can be a value of @ref TM_RE_Mode_t enumeration
 * @retval None
 */
void TM_RE_SetMode(TM_RE_t* data, TM_RE_Mode_t mode);

/**
 * @brief  Checks and gets new values of rotary encoder
 * @param  *data: Pointer to @ref TM_RE_t structure
 * @retval Member of @ref TM_RE_Rotate_t
 */
TM_RE_Rotate_t TM_RE_Get(TM_RE_t* data);

/**
 * @brief  Process function.
 * @note   This function have to be called inside your interrupt handler.
 * @param  *data: Pointer to rotary encoder @ret TM_RE_t data where interrupt occured
 * @retval None
 */
void TM_RE_Process(TM_RE_t* data);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

#endif


