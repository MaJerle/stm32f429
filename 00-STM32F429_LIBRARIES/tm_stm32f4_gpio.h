/**
 *	GPIO library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/03/library-53-gpio-for-stm32f4
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
 */
#ifndef TM_GPIO_H
#define TM_GPIO_H 100

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
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

/**
 * GPIO Mode enumeration
 * 
 * Parameters;
 * 	- TM_GPIO_Mode_IN:
 * 		Set pin as input
 * 	- TM_GPIO_Mode_OUT:
 * 		Set pin as output
 * 	- TM_GPIO_Mode_AF:
 * 		Set pin as alternate function
 * 	- TM_GPIO_Mode_AN:
 * 		Set pin as analog
 */
typedef enum {
	TM_GPIO_Mode_IN = 0x00,
	TM_GPIO_Mode_OUT = 0x01,
	TM_GPIO_Mode_AF = 0x02,
	TM_GPIO_Mode_AN = 0x03,
} TM_GPIO_Mode_t;

/**
 * GPIO Output Type enumeration
 * 
 * Parameters:
 * 	- TM_GPIO_OType_PP:
 * 		GPIO pin is push-pull
 * 	- TM_GPIO_OType_OD:
 * 		GPIO pin is open-drain
 */
typedef enum {
	TM_GPIO_OType_PP = 0x00,
	TM_GPIO_OType_OD = 0x01
} TM_GPIO_OType_t;

/**
 * GPIO speed enumeration
 * 
 * Parameters:
 * 	- TM_GPIO_Speed_Low
 * 	- TM_GPIO_Speed_Medium
 * 	- TM_GPIO_Speed_Fast
 * 	- TM_GPIO_Speed_High
 * 		GPIO Speeds
 */
typedef enum {
	TM_GPIO_Speed_Low = 0x00,
	TM_GPIO_Speed_Medium = 0x01,
	TM_GPIO_Speed_Fast = 0x02,
	TM_GPIO_Speed_High = 0x03
} TM_GPIO_Speed_t;

/**
 * GPIO pull resistors enumeration
 * 
 * Parameters:
 * 	- TM_GPIO_PuPd_NOPULL:
 * 		No pull
 * 	- TM_GPIO_PuPd_UP:
 * 		Pull up
 * 	- TM_GPIO_PuPd_DOWN:
 * 		Pull down
 */
typedef enum {
	TM_GPIO_PuPd_NOPULL = 0x00,
	TM_GPIO_PuPd_UP = 0x01,
	TM_GPIO_PuPd_DOWN = 0x02
} TM_GPIO_PuPd_t;

/**
 * Initialize GPIO pin(s)
 * 
 * This function also enables clock for GPIO
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 * 	- TM_GPIO_Mode_t GPIO_Mode:
 * 		Select GPIO mode
 * 	- TM_GPIO_OType_t GPIO_OType:
 * 		Select GPIO output type
 * 	- TM_GPIO_PuPd_t GPIO_PuPd:
 * 		Select GPIO pull resistor
 * 	- TM_GPIO_Speed_t GPIO_Speed:
 * 		Select GPIO speed
 * 
 * No return
 */
void TM_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed);

/**
 * Initialize GPIO pin(s) as alternate function to use with peripherals
 * 
 * This function also enables clock for GPIO
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 * 	- TM_GPIO_Mode_t GPIO_OType:
 * 		Select GPIO output mode
 * 	- TM_GPIO_PuPd_t GPIO_PuPd:
 * 		Select GPIO pull resistor
 * 	- TM_GPIO_Speed_t GPIO_Speed:
 * 		Select GPIO speed
 * 	- uint8_t Alternate:
 * 		Alternate function you will use
 */
void TM_GPIO_InitAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed, uint8_t Alternate);

/**
 * Set pin low
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 * 
 * Defined as macro for faster execution
 */
#define TM_GPIO_SetPinLow(GPIOx, GPIO_Pin)			((GPIOx)->BSRRH = (GPIO_Pin))

/**
 * Set pin high
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 * 
 * Defined as macro for faster execution
 */
#define TM_GPIO_SetPinHigh(GPIOx, GPIO_Pin) 		((GPIOx)->BSRRL = (GPIO_Pin))

/**
 * Set pin value
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 *  - uint8_t value:
 *  	If value is 0, pin is low, if > 0 pin is high
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_SetPinValue(GPIOx, GPIO_Pin, val)	((val) ? TM_GPIO_SetPinHigh(GPIOx, GPIO_Pin) : TM_GPIO_SetPinLow(GPIOx, GPIO_Pin))

/**
 * Toggle pin
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 *  - uint8_t value:
 *  	If value is 0, pin is low, if > 0 pin is high
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_TogglePinValue(GPIOx, GPIO_Pin)		((GPIOx)->ODR ^= (GPIO_Pin))

/**
 * Set port value
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint32_t value:
 * 		Value to be set on output port
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_SetPortValue(GPIOx, value)			((GPIOx)->ODR = (value))

/**
 * Get input data bit
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 * 
 * Returns:
 * 	- 0: Pin is low
 * 	- > 0: pin is high
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_GetInputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->IDR & (GPIO_Pin)) == 0 ? Bit_RESET : Bit_SET)

/**
 * Get output data bit
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin
 * 
 * Returns:
 * 	- 0: Pin is low
 * 	- > 0: pin is high
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_GetOutputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->ODR & (GPIO_Pin)) == 0 ? Bit_RESET : Bit_SET)

/**
 * Get input data
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 
 * Returns input data register value
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_GetPortInputValue(GPIOx)			((GPIOx)->IDR)

/**
 * Get output data
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 
 * Returns input data register value
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_GetPortOutputValue(GPIOx)			((GPIOx)->ODR)

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
