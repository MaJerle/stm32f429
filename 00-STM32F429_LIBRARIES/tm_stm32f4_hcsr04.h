/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/08/library-30-measure-distance-with-hc-sr04-and-stm32f4xx
 * @version v2.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Measure distance with HC-SR04 Ultrasonic distance sensor on STM32F4xx devices
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
#ifndef TM_HCSR04_H
#define TM_HCSR04_H 200
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_HCSR04
 * @brief    Measure distance with HC-SR04 Ultrasonic distance sensor on STM32F4xx devices - http://stm32f4-discovery.com/2014/08/library-30-measure-distance-with-hc-sr04-and-stm32f4xx
 * @{
 *
 * \par Default pinout
 * 
 * There is not default pinout anymore.
 * As of version 2.0, you can use unlimited HC-SR sensors connected to STM32F4xx device.
 *
 * You just make sure that you use different pins for different sensors.
 *
 * Pins are passed on @ref TM_HCSR04_Init function.
 *
@verbatim
I didn't tested that, but you might use the same ECHO pin for all sensors if you read sensor by sensor at a time. Do that by own risk!
@endverbatim
 * 
 *
 * \par Changelog
 *
@verbatim
 Version 2.0
  - March 29, 2015
  - Library has been rewritten. Now supports unlimited number of HCSR04 devices
  - Only limit is number of GPIO pins available in your MCU package

 Version 1.2
  - March 11, 2015
  - Added support for my new GPIO library

 Version 1.1
  - November 29, 2014
  - Update to fit new delay system 

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - defines.h
 - TM DELAY
 - TM GPIO
@endverbatim
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_HCSR04_Macros
 * @brief    Library macros
 * @{
 */

/* Default timeout pulses */
#ifndef HCSR04_TIMEOUT
#define HCSR04_TIMEOUT			1000000
#endif

/**
 * @brief  Time in microseconds to centimeters conversion
 */
#define HCSR04_NUMBER			((float)0.0171821)

/**
 * @}
 */

/**
 * @defgroup TM_HCSR04_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  HC-SR04 working structure
 */
typedef struct {
	float Distance;              /*!< Distance measured from sensor in centimeters */
	GPIO_TypeDef* ECHO_GPIOx;    /*!< Pointer to GPIOx PORT for ECHO pin. Meant for private use only */
	uint16_t ECHO_GPIO_Pin;      /*!< GPIO Pin for ECHO pin. Meant for private use only */
	GPIO_TypeDef* TRIGGER_GPIOx; /*!< Pointer to GPIOx PORT for TRIGGER pin. Meant for private use only */
	uint16_t TRIGGER_GPIO_Pin;   /*!< GPIO Pin for ECHO pin. Meant for private use only */
} TM_HCSR04_t;

/**
 * @}
 */

/**
 * @defgroup TM_HCSR04_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes HC-SR04 sensor
 * @param  *HCSR04: Pointer to empty @ref TM_HCSR04_t structure to save initialization data
 * @param  *ECHO_GPIOx: Pointer to GPIOx PORT for ECHO pin
 * @param  ECHO_GPIO_Pin: GPIO Pin for ECHO pin
 * @param  *TRIGGER_GPIOx: Pointer to GPIOx PORT for TRIGGER pin
 * @param  TRIGGER_GPIO_Pin: GPIO Pin for ECHO pin
 * @retval HC-SR04 status:
 *            - 0: Device not detected
 *            - > 0: Device is ready to use
 */
uint8_t TM_HCSR04_Init(TM_HCSR04_t* HCSR04, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin);

/**
 * @brief  Starts sensor measurement and read it's data
 * @param  *HCSR04: Pointer to @ref TM_HCSR04_t structure to save initialization data
 * @retval Distance in float:
 *            - > 0: Valid distance in cm (centimeters)
 *            -  -1: Error
 */
float TM_HCSR04_Read(TM_HCSR04_t* HCSR04);

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
