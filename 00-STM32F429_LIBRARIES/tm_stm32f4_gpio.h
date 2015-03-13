/**
 *	GPIO library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/03/library-53-gpio-for-stm32f4
 *	@version 	v1.2
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
 * 
 * Version 1.2
 *	- March 10, 2015
 *	- Added functions TM_GPIO_SetPinAsInput and TM_GPIO_SetPinAsOutput
 *	- Added functions TM_GPIO_GetPortSource and TM_GPIO_GetPinSource
 * 
 * Version 1.1
 *	- March 09, 2015
 *	- Added function to deinit pin. Pin is set to analog input which allows lowest current consumption
 */
#ifndef TM_GPIO_H
#define TM_GPIO_H 120

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

/* Defines for GPIOs */
/* For HAL periph drivers */
#ifndef GPIO_PIN_0
#define GPIO_PIN_0		((uint16_t)0x0001)
#define GPIO_PIN_1		((uint16_t)0x0002)
#define GPIO_PIN_2		((uint16_t)0x0004)
#define GPIO_PIN_3		((uint16_t)0x0008)
#define GPIO_PIN_4		((uint16_t)0x0010)
#define GPIO_PIN_5		((uint16_t)0x0020)
#define GPIO_PIN_6		((uint16_t)0x0040)
#define GPIO_PIN_7		((uint16_t)0x0080)
#define GPIO_PIN_8		((uint16_t)0x0100)
#define GPIO_PIN_9		((uint16_t)0x0200)
#define GPIO_PIN_10		((uint16_t)0x0400)
#define GPIO_PIN_11		((uint16_t)0x0800)
#define GPIO_PIN_12		((uint16_t)0x1000)
#define GPIO_PIN_13		((uint16_t)0x2000)
#define GPIO_PIN_14		((uint16_t)0x4000)
#define GPIO_PIN_15		((uint16_t)0x8000)
#define GPIO_PIN_ALL	((uint16_t)0x8000)
#endif

/* STD drivers */
#ifndef GPIO_Pin_0
#define GPIO_Pin_0		((uint16_t)0x0001)
#define GPIO_Pin_1		((uint16_t)0x0002)
#define GPIO_Pin_2		((uint16_t)0x0004)
#define GPIO_Pin_3		((uint16_t)0x0008)
#define GPIO_Pin_4		((uint16_t)0x0010)
#define GPIO_Pin_5		((uint16_t)0x0020)
#define GPIO_Pin_6		((uint16_t)0x0040)
#define GPIO_Pin_7		((uint16_t)0x0080)
#define GPIO_Pin_8		((uint16_t)0x0100)
#define GPIO_Pin_9		((uint16_t)0x0200)
#define GPIO_Pin_10		((uint16_t)0x0400)
#define GPIO_Pin_11		((uint16_t)0x0800)
#define GPIO_Pin_12		((uint16_t)0x1000)
#define GPIO_Pin_13		((uint16_t)0x2000)
#define GPIO_Pin_14		((uint16_t)0x4000)
#define GPIO_Pin_15		((uint16_t)0x8000)
#define GPIO_Pin_All	((uint16_t)0x8000)
#endif

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
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
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
extern void TM_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed);

/**
 * Initialize GPIO pin(s) as alternate function to use with peripherals
 * 
 * This function also enables clock for GPIO
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
 * 	- TM_GPIO_Mode_t GPIO_OType:
 * 		Select GPIO output mode
 * 	- TM_GPIO_PuPd_t GPIO_PuPd:
 * 		Select GPIO pull resistor
 * 	- TM_GPIO_Speed_t GPIO_Speed:
 * 		Select GPIO speed
 * 	- uint8_t Alternate:
 * 		Alternate function you will use
 */
extern void TM_GPIO_InitAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed, uint8_t Alternate);

/**
 * Deinitialize GPIO pin(s)
 * 
 * This function will set pin(s) to analog input for smallest current consumption
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
 * 
 * No return
 */
extern void TM_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * Set pin(s) as input
 * 
 * This function will set pin(s) to input. Pins HAVE to be initialized first with TM_GPIO_Init() or TM_GPIO_InitAlternate()
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want operate with
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set them as input
 * 
 * No return
 */
extern void TM_GPIO_SetPinAsInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * Set pin(s) as output
 * 
 * This function will set pin(s) to output. Pins HAVE to be initialized first with TM_GPIO_Init() or TM_GPIO_InitAlternate()
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want operate with
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set them as output
 * 
 * No return
 */
extern void TM_GPIO_SetPinAsOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * Set pin low
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
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
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
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
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
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
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
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
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
 * 
 * Returns:
 * 	- 0: Pin is low
 * 	- > 0: pin is high
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_GetInputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->IDR & (GPIO_Pin)) == 0 ? 0 : 1)

/**
 * Get output data bit
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIO port you want to initialize
 * 	- uint16_t GPIO_Pin:
 * 		Select GPIO pin. You can select more pins with | (OR) operator to set same settings to more pins
 * 
 * Returns:
 * 	- 0: Pin is low
 * 	- > 0: pin is high
 *  	
 * Defined as macro for faster execution
 */
#define TM_GPIO_GetOutputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->ODR & (GPIO_Pin)) == 0 ? 0 : 1)

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

/**
 * Get port source from GPIO
 *
 * Meant for private use, unless you know what you are doing
 *
 * Parameters:
 *	- GPIO_TypeDef* GPIOx:
 *		GPIOx for which you want port source to be calculated
 * 
 * Returns calculated port source for GPIOx
 */
extern uint16_t TM_GPIO_GetPortSource(GPIO_TypeDef* GPIOx);

/**
 * Get pin source from GPIO_Pin bit
 *
 * Meant for private use, unless you know what you are doing
 *
 * Parameters:
 *	- uint16_t GPIO_Pin:
 *		GPIO pin for which you want pin source to be calculated
 * 
 * Returns calculated pin source for GPIOx
 */
extern uint16_t TM_GPIO_GetPinSource(uint16_t GPIO_Pin);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
