/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/07/library-22-true-random-number-generator-stm32f4xx/
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library template 
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
#ifndef TM_RNG_H
#define TM_RNG_H 110
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_RNG
 * @brief    Random number generator library for STM32F4xx devices - http://stm32f4-discovery.com/2014/07/library-22-true-random-number-generator-stm32f4xx/
 * @{
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 10, 2015
  - Added support for STD/HAL drivers
  
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - defines.h
@endverbatim
 */
#include "stm32f4xx.h"
#include "defines.h"

#ifndef RNG_NVIC_PREEMPTION_PRIORITY
#define RNG_NVIC_PREEMPTION_PRIORITY   0x02
#endif

#ifndef RNG_NVIC_SUBPRIORITY
#define RNG_NVIC_SUBPRIORITY           0x00
#endif

/**
 * @defgroup TM_RNG_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes RNG peripheral and enables clock
 * @param  None
 * @retval None
 */
void TM_RNG_Init(void);

/**
 * @brief  De initializes RNG peripheral and disables clock
 * @param  None
 * @retval None
 */
void TM_RNG_DeInit(void);

/**
 * @brief  Gets 32-bit random number
 * @param  None
 * @retval 32-bit random number
 */
uint32_t TM_RNG_Get(void);

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

