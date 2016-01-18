/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/04/library-07-da-converter-on-stm32f4xx/
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   DAC library for STM32F4xx
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
#ifndef TM_DAC_H
#define TM_DAC_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_DAC
 * @brief    DAC library for STM32F4xx - http://stm32f4-discovery.com/2014/04/library-07-da-converter-on-stm32f4xx/
 * @{
 *
 * This library provides 12-bit digital to analog output, values from 0 to 4095
 *	
 * @par Pinout
 *
@verbatim
DAC	channel		STM32F4xx
		
DAC1			PA4
DAC2			PA5
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 12, 2015
  - Support for my new GPIO library
 
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx DAC
 - defines.h
 - TM GPIO
@endverbatim
 */

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_DAC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Select which DAC channel
 */
typedef enum {
	TM_DAC1, /*!< DAC channel 1 */
	TM_DAC2  /*!< DAC channel 2 */
} TM_DAC_Channel_t;

/**
 * @}
 */

/**
 * @defgroup TM_DAC_Functions
 * @brief    Library Functions
 * @{
 */
 
/**
 * @brief  Initializes DAC channel and it's pin
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref TM_DAC_Channel_t enumeration
 * @retval None
 */
void TM_DAC_Init(TM_DAC_Channel_t DACx);

/**
 * @brief  Sets analog value to ADCx
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref TM_DAC_Channel_t enumeration
 * @param  value: 12-bit unsigned value for 12-bit DAC
 * @retval None
 */
void TM_DAC_SetValue(TM_DAC_Channel_t DACx, uint16_t value);

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
