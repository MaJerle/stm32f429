/** 
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/03/library-53-gpio-for-stm32f4
 * @version v1.5
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   GPIO Library for STM32F4xx devices
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
#ifndef TM_GPIO_H
#define TM_GPIO_H 150

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_GPIO
 * @brief    TM GPIO Library for STM32F4xx - http://stm32f4-discovery.com/2015/03/library-53-gpio-for-stm32f4
 * @{
 *
 * GPIO library can be used for GPIO pins. 
 *
 * It features fast initialization methods as well pin input/output methods.
 *
 * It can be used as replacement for STD/HAL drivers GPIO library.
 *
 * \par Changelog
 *
@verbatim
 Version 1.5
  - June 10 2015
  - Added 2 new functions for getting used GPIO pins
  
 Version 1.4
  - April 28, 2015
  - Added support for PORT locking
  
 Version 1.3
  - March 23, 2015
  - Totally independent from HAL / SPD drivers
  - Library can be used with any drivers or totally itself
  
 Version 1.2
  - March 10, 2015
  - Added functions TM_GPIO_SetPinAsInput and TM_GPIO_SetPinAsOutput
  - Added functions TM_GPIO_GetPortSource and TM_GPIO_GetPinSource
0
 Version 1.1
  - March 09, 2015
  - Added function to deinit pin. Pin is set to analog input which allows lowest current consumption
  
 Version 1.0
  - March 08, 2015
  - Initial release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx GPIO
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

/**
 * @defgroup TM_GPIO_Macros
 * @brief    GPIO Library macros
 * @{
 */

/**
 * @brief GPIO Pins declarations 
 * @note  For HAL drivers compatibility
 */
  
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
#define GPIO_PIN_ALL	((uint16_t)0xFFFF)
#endif

/**
 * @brief GPIO Pins declarations 
 * @note  For STD Periph drivers compatibility
 */

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
#define GPIO_Pin_All	((uint16_t)0xFFFF)
#endif

/**
 * @}
 */

/**
 * @defgroup TM_GPIO_Typedefs
 * @brief    GPIO Typedefs used for GPIO library for initialization purposes
 * @{
 */

/**
 * @brief GPIO Mode enumeration
 */
typedef enum {
	TM_GPIO_Mode_IN = 0x00,  /*!< GPIO Pin as General Purpose Input */
	TM_GPIO_Mode_OUT = 0x01, /*!< GPIO Pin as General Purpose Output */
	TM_GPIO_Mode_AF = 0x02,  /*!< GPIO Pin as Alternate Function */
	TM_GPIO_Mode_AN = 0x03,  /*!< GPIO Pin as Analog */
} TM_GPIO_Mode_t;

/**
 * @brief GPIO Output type enumeration
 */
typedef enum {
	TM_GPIO_OType_PP = 0x00, /*!< GPIO Output Type Push-Pull */
	TM_GPIO_OType_OD = 0x01  /*!< GPIO Output Type Open-Drain */
} TM_GPIO_OType_t;

/**
 * @brief  GPIO Speed enumeration
 */
typedef enum {
	TM_GPIO_Speed_Low = 0x00,    /*!< GPIO Speed Low */
	TM_GPIO_Speed_Medium = 0x01, /*!< GPIO Speed Medium */
	TM_GPIO_Speed_Fast = 0x02,   /*!< GPIO Speed Fast */
	TM_GPIO_Speed_High = 0x03    /*!< GPIO Speed High */
} TM_GPIO_Speed_t;

/**
 * @brief GPIO pull resistors enumeration
 */
typedef enum {
	TM_GPIO_PuPd_NOPULL = 0x00, /*!< No pull resistor */
	TM_GPIO_PuPd_UP = 0x01,     /*!< Pull up resistor enabled */
	TM_GPIO_PuPd_DOWN = 0x02    /*!< Pull down resistor enabled */
} TM_GPIO_PuPd_t;

/**
 * @} TM_GPIO_Typedefs
 */

/**
 * @defgroup TM_GPIO_Functions
 * @brief    GPIO Functions
 * @{
 */
 
/**
 * @brief  Initializes GPIO pins(s)
 * @note   This function also enables clock for GPIO port
 * @param  GPIOx: Pointer to GPIOx port you will use for initialization
 * @param  GPIO_Pin: GPIO pin(s) you will use for initialization
 * @param  GPIO_Mode: Select GPIO mode. This parameter can be a value of @ref TM_GPIO_Mode_t enumeration
 * @param  GPIO_OType: Select GPIO Output type. This parameter can be a value of @ref TM_GPIO_OType_t enumeration
 * @param  GPIO_PuPd: Select GPIO pull resistor. This parameter can be a value of @ref TM_GPIO_PuPd_t enumeration
 * @param  GPIO_Speed: Select GPIO speed. This parameter can be a value of @ref TM_GPIO_Speed_t enumeration
 * @retval None
 */
void TM_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed);

/**
 * @brief  Initializes GPIO pins(s) as alternate function
 * @note   This function also enables clock for GPIO port
 * @param  GPIOx: Pointer to GPIOx port you will use for initialization
 * @param  GPIO_Pin: GPIO pin(s) you will use for initialization
 * @param  GPIO_OType: Select GPIO Output type. This parameter can be a value of @ref TM_GPIO_OType_t enumeration
 * @param  GPIO_PuPd: Select GPIO pull resistor. This parameter can be a value of @ref TM_GPIO_PuPd_t enumeration
 * @param  GPIO_Speed: Select GPIO speed. This parameter can be a value of @ref TM_GPIO_Speed_t enumeration
 * @param  Alternate: Alternate function you will use
 * @retval None
 */
void TM_GPIO_InitAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed, uint8_t Alternate);

/**
 * @brief  Deinitializes pin(s)
 * @note   Pins(s) will be set as analog mode to get low power consumption
 * @param  GPIOx: GPIOx PORT where you want to set pin as input
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as input
 * @retval None
 */
void TM_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  Sets pin(s) as input 
 * @note   Pins HAVE to be initialized first using @ref TM_GPIO_Init() or @ref TM_GPIO_InitAlternate() function
 * @note   This is just an option for fast input mode
 * @param  GPIOx: GPIOx PORT where you want to set pin as input
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as input
 * @retval None
 */
void TM_GPIO_SetPinAsInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  Sets pin(s) as output
 * @note   Pins HAVE to be initialized first using @ref TM_GPIO_Init() or @ref TM_GPIO_InitAlternate() function
 * @note   This is just an option for fast output mode 
 * @param  GPIOx: GPIOx PORT where you want to set pin as output
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as output
 * @retval None
 */
void TM_GPIO_SetPinAsOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  Sets pin(s) as analog
 * @note   Pins HAVE to be initialized first using @ref TM_GPIO_Init() or @ref TM_GPIO_InitAlternate() function
 * @note   This is just an option for fast analog mode 
 * @param  GPIOx: GPIOx PORT where you want to set pin as analog
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as analog
 * @retval None
 */
void TM_GPIO_SetPinAsAnalog(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/** 
 * @brief  Sets pin(s) as alternate function
 * @note   For proper alternate function, you should first init pin using @ref TM_GPIO_InitAlternate() function.
 *            This functions is only used for changing GPIO mode
 * @param  GPIOx: GPIOx PORT where you want to set pin as alternate
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as alternate
 * @retval None
 */
void TM_GPIO_SetPinAsAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  Sets pull resistor settings to GPIO pin(s)
 * @note   Pins HAVE to be initialized first using @ref TM_GPIO_Init() or @ref TM_GPIO_InitAlternate() function
 * @param  *GPIOx: GPIOx PORT where you want to select pull resistor
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them as output
 * @param  GPIO_PuPd: Pull resistor option. This parameter can be a value of @ref TM_GPIO_PuPd_t enumeration
 * @retval None
 */
void TM_GPIO_SetPullResistor(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_PuPd_t GPIO_PuPd);

/**
 * @brief  Sets pin(s) low
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set pin low
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them low
 * @retval None
 */
#define TM_GPIO_SetPinLow(GPIOx, GPIO_Pin)			((GPIOx)->BSRRH = (GPIO_Pin))

/**
 * @brief  Sets pin(s) high
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set pin high
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them high
 * @retval None
 */
#define TM_GPIO_SetPinHigh(GPIOx, GPIO_Pin) 		((GPIOx)->BSRRL = (GPIO_Pin))

/**
 * @brief  Sets pin(s) value
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set pin value
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them value
 * @param  val: If parameter is 0 then pin will be low, otherwise high
 * @retval None
 */
#define TM_GPIO_SetPinValue(GPIOx, GPIO_Pin, val)	((val) ? TM_GPIO_SetPinHigh(GPIOx, GPIO_Pin) : TM_GPIO_SetPinLow(GPIOx, GPIO_Pin))

/**
 * @brief  Toggles pin(s)
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to toggle pin value
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to toggle them all at a time
 * @retval None
 */
#define TM_GPIO_TogglePinValue(GPIOx, GPIO_Pin)		((GPIOx)->ODR ^= (GPIO_Pin))

/**
 * @brief  Sets value to entire GPIO PORT
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set value
 * @param  value: Value for GPIO OUTPUT data
 * @retval None
 */
#define TM_GPIO_SetPortValue(GPIOx, value)			((GPIOx)->ODR = (value))

/**
 * @brief  Gets input data bit
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to read input bit value
 * @param  GPIO_Pin: GPIO pin where you want to read value
 * @retval 1 in case pin is high, or 0 if low
 */
#define TM_GPIO_GetInputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->IDR & (GPIO_Pin)) == 0 ? 0 : 1)

/**
 * @brief  Gets output data bit
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to read output bit value
 * @param  GPIO_Pin: GPIO pin where you want to read value
 * @retval 1 in case pin is high, or 0 if low
 */
#define TM_GPIO_GetOutputPinValue(GPIOx, GPIO_Pin)	(((GPIOx)->ODR & (GPIO_Pin)) == 0 ? 0 : 1)

/**
 * @brief  Gets input value from entire GPIO PORT
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to read input data value
 * @retval Entire PORT INPUT register
 */
#define TM_GPIO_GetPortInputValue(GPIOx)			((GPIOx)->IDR)

/**
 * @brief  Gets output value from entire GPIO PORT
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to read output data value
 * @retval Entire PORT OUTPUT register
 */
#define TM_GPIO_GetPortOutputValue(GPIOx)			((GPIOx)->ODR)

/**
 * @brief  Gets port source from desired GPIOx PORT
 * @note   Meant for private use, unless you know what are you doing
 * @param  GPIOx: GPIO PORT for calculating port source
 * @retval Calculated port source for GPIO
 */
uint16_t TM_GPIO_GetPortSource(GPIO_TypeDef* GPIOx);

/**
 * @brief  Gets pin source from desired GPIO pin
 * @note   Meant for private use, unless you know what are you doing
 * @param  GPIO_Pin: GPIO pin for calculating port source
 * @retval Calculated pin source for GPIO pin
 */
uint16_t TM_GPIO_GetPinSource(uint16_t GPIO_Pin);

/**
 * @brief  Locks GPIOx register for future changes
 * @note   You are not able to config GPIO registers until new MCU reset occurs
 * @param  *GPIOx: GPIOx PORT where you want to lock config registers
 * @param  GPIO_Pin: GPIO pin(s) where you want to lock config registers
 * @retval None
 */
void TM_GPIO_Lock(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/** 
 * @brief  Gets bit separated pins which were used at least once in library and were not deinitialized
 * @param  *GPIOx: Pointer to GPIOx peripheral where to check used GPIO pins
 * @retval Bit values for used pins
 */
uint16_t TM_GPIO_GetUsedPins(GPIO_TypeDef* GPIOx);

/** 
 * @brief  Gets bit separated pins which were not used at in library or were deinitialized
 * @param  *GPIOx: Pointer to GPIOx peripheral where to check used GPIO pins
 * @retval Bit values for free pins
 */
uint16_t TM_GPIO_GetFreePins(GPIO_TypeDef* GPIOx);

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
