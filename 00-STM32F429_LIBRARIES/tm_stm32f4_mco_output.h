/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/10/library-40-output-clocks-stm32f4/
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   MCO Output for STM32F4xx
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
#ifndef TM_MCOOUTPUT_H
#define TM_MCOOUTPUT_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_MCO
 * @brief    MCO Output clock for STM32F4xx - http://stm32f4-discovery.com/2014/10/library-40-output-clocks-stm32f4/
 * @{
 *
 * This library allows you to output different frequencies. They are split into 2 different output sections.
 *
 * \par MCO1 output = PA8
 *
 * MCO1 output
 * 	- Pin PA8
 * 	- HSI: High Speed Internal clock, 16MHz RC oscillator
 * 	- HSE: High Speed External clock, external crystal or user clock
 * 	- LSE: External 32738Hz low-speed oscillator or ceramic resonator
 * 	- PLLCLK: Output from PLL
 *
 * \par MCO2 output = PC9
 *
 * MCO2 output
 * 	- Pin PC9
 * 	- SYSCLK: Output system core clock
 * 	- PLLI2SCLK: Accurate clock for high-quality audio performance in I2S and SAI interfaces
 * 	- HSE: High Speed External clock, external crystal or user clock
 * 	- PLLCLK: Output from PLL
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 11, 2015
  - Support for my new GPIO library

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
 - TM GPIO
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

 
/**
 * @defgroup TM_MCO_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Prescaler values for output clock
 */
typedef enum {
	TM_MCOOUTPUT_Prescaler_1, /*!< Output original value of clock on MCO pin */
	TM_MCOOUTPUT_Prescaler_2, /*!< Output clock is original / 2 */
	TM_MCOOUTPUT_Prescaler_3, /*!< Output clock is original / 3 */
	TM_MCOOUTPUT_Prescaler_4, /*!< Output clock is original / 4 */
	TM_MCOOUTPUT_Prescaler_5  /*!< Output clock is original / 5 */
} TM_MCOOUTPUT_Prescaler_t;

/**
 * @brief  Possible output clocks on MCO1 pin
 */
typedef enum {
	TM_MCOOUTPUT1_Source_HSI,   /*!< High Speed Internal clock, 16MHz RC oscillator */
	TM_MCOOUTPUT1_Source_HSE,   /*!< High Speed External clock, external crystal or user clock */
	TM_MCOOUTPUT1_Source_LSE,   /*!< External 32738Hz low-speed oscillator or ceramic resonator */
	TM_MCOOUTPUT1_Source_PLLCLK /*!< Output from PLL */
} TM_MCOOUTPUT1_Source_t;

/**
 * @brief  Possible output clocks on MCO2 pin
 */
typedef enum {
	TM_MCOOUTPUT2_Source_SYSCLK,    /*!< System core clock */
	TM_MCOOUTPUT2_Source_PLLI2SCLK, /*!< Accurate clock for high-quality audio performance in I2S and SAI interfaces */
	TM_MCOOUTPUT2_Source_HSE,       /*!< High Speed External clock, external crystal or user clock */
	TM_MCOOUTPUT2_Source_PLLCLK     /*!< Output from PLL */
} TM_MCOOUTPUT2_Source_t;

/**
 * @}
 */

/**
 * @defgroup TM_MCO_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes and prepares MCO1 pin to output clock
 * @param  None
 * @retval None
 */
void TM_MCOOUTPUT_InitMCO1(void);

/**
 * @brief  Sets output for MCO1 pin
 * @param  Source: Clock source to output. This parameter can be a value of @ref TM_MCOOUTPUT1_Source_t enumeration.
 * @param  Prescaler: Prescaler used for clock. This parameter can be a value of @ref TM_MCOOUTPUT_Prescaler_t enumeration.
 * @retval None
 */
void TM_MCOOUTPUT_SetOutput1(TM_MCOOUTPUT1_Source_t Source, TM_MCOOUTPUT_Prescaler_t Prescaler);

/**
 * @brief  Initializes and prepares MCO2 pin to output clock
 * @param  None
 * @retval None
 */
void TM_MCOOUTPUT_InitMCO2(void);
 
/**
 * @brief  Sets output for MCO2 pin
 * @param  Source: Clock source to output. This parameter can be a value of @ref TM_MCOOUTPUT2_Source_t enumeration.
 * @param  Prescaler: Prescaler used for clock. This parameter can be a value of @ref TM_MCOOUTPUT_Prescaler_t enumeration.
 * @retval None
 */
void TM_MCOOUTPUT_SetOutput2(TM_MCOOUTPUT2_Source_t Source, TM_MCOOUTPUT_Prescaler_t Prescaler);

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
