/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/08/library-25-am2301-dht21-sensor-stm32f4xx/
 * @version v1.3
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Basic library for AM2301 (DHT21) temperature and humidity sensor
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
#ifndef TM_AM2301_H
#define TM_AM2301_H 130

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_AM2301
 * @brief    Basic library for AM2301 (DHT21) temperature and humidity sensor - http://stm32f4-discovery.com/2014/08/library-25-am2301-dht21-sensor-stm32f4xx/
 * @{
 *
 * This library does not use any timers and interrupts to increase speed, only basic pooling mode.
 * 
 * \par Default pinout
 *
@verbatim
AM2301		STM32F4xx		Description

VCC			3V3-5V			Supply voltage from 3V3 to 5V
GND			GND				Ground
DATA		PD1				Data line
@endverbatim
 *
 * By default, PD1 pin is used for data. If you want to change it, use lines below in defines.h to and edit them:
 *
@verbatim
//Select custom pin for AM2301 sensor
#define AM2301_PORT				GPIOD
#define AM2301_PIN				GPIO_PIN_1
@endverbatim
 *
 * \par Data output
 *
 * Temperature and humidity are returned from sensor in x10 multiplier, so like 55.5% humidity,
 * sensor will return 555 and 27.3°C, sensor will return 273.
 *
 * @note This values are also returned from my library, you have to manually convert them (divide by 10)
 *
 * \par Changelog
 *
@verbatim
 Version 1.3
  - March 12, 2015
  - Added support for my new GPIO library

 Version 1.2
  - December 01, 2014
  - Added comments

 Version 1.1
  - November 28, 2014
  - Designed for new Delay system
 
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - defines.h
 - TM DELAY
 - TM GPIO
 
@endverbatim
 */
/**
 * Dependencies
 * - STM32F4xx
 * - defines.h
 * - TM DELAY
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_AM2301_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Default data pin, overwrite it in defines.h file 
 */
#ifndef AM2301_PIN
#define AM2301_PORT				GPIOD
#define AM2301_PIN				GPIO_PIN_1
#endif

/* Pin settings */
#define AM2301_PIN_LOW			TM_GPIO_SetPinLow(AM2301_PORT, AM2301_PIN)
#define AM2301_PIN_HIGH			TM_GPIO_SetPinHigh(AM2301_PORT, AM2301_PIN)
#define AM2301_PIN_IN			TM_GPIO_SetPinAsInput(AM2301_PORT, AM2301_PIN)
#define AM2301_PIN_OUT			TM_GPIO_SetPinAsOutput(AM2301_PORT, AM2301_PIN)
#define AM2301_PIN_READ			TM_GPIO_GetInputPinValue(AM2301_PORT, AM2301_PIN)

/**
 * @}
 */
 
/**
 * @defgroup TM_AM2301_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * Enumerations
 *
 * There are several different possible results.
 * If TM_AM2301_OK is returned from read function then you have valid data.
 */
typedef enum {
	TM_AM2301_OK,						/*!< Data OK */
	TM_AM2301_ERROR,					/*!< An error occurred */
	TM_AM2301_CONNECTION_ERROR,			/*!< Device is not connected */
	TM_AM2301_WAITHIGH_ERROR,           /*!< Wait high pulse timeout */
	TM_AM2301_WAITLOW_ERROR,            /*!< Wait low pulse timeout */
	TM_AM2301_WAITHIGH_LOOP_ERROR,      /*!< Loop error for high pulse */
 	TM_AM2301_WAITLOW_LOOP_ERROR,       /*!< Loop error for low pulse */
	TM_AM2301_PARITY_ERROR				/*!< Data read fail */
} TM_AM2301_t;

/**
 * @brief  AM2301 main data structure		
 */
typedef struct {
	int16_t Temp; /*!< Temperature in tenths of degrees.
	                   If real temperature is 27.3°C, this variable's value is 273 */
	uint16_t Hum; /*!< Humidity in tenths of percent.
	                   If real humidity is 55.5%, this variable's value is 555 */
} TM_AM2301_Data_t;

/**
 * @}
 */

/**
 * @defgroup TM_AM2301_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes AM2301 sensor
 * @param  None
 * @retval TM_AM2301_OK
 */
TM_AM2301_t TM_AM2301_Init(void);

/**
 * @brief  Reads data from sensor
 * @param  *data: Pointer to @ref TM_AM2301_Data_t data structure to store data into
 * @retval Data valid:
 *            - TM_AM2301_OK: Data valid
 *            - Else: Data not valid
 */
TM_AM2301_t TM_AM2301_Read(TM_AM2301_Data_t* data);

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

