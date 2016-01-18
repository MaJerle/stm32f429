/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Brown-Out detection for STM32F4xx
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
#ifndef TM_BOR_H
#define TM_BOR_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_BOR
 * @brief    Brown-Out detection for STM32F4xx
 * @{
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx FLASH
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"
#include "defines.h"

/**
 * @defgroup TM_BOR_Macros
 * @brief    Library defines
 * @{
 */
/* Macros here */
/**
 * @}
 */
 
/**
 * @defgroup TM_BOR_Typedefs
 * @brief    Library Typedefs
 * @{
 */
 
typedef enum {
	TM_BOR_Level_None = OB_BOR_OFF,
	TM_BOR_Level_1 = OB_BOR_LEVEL1,
	TM_BOR_Level_2 = OB_BOR_LEVEL2,
	TM_BOR_Level_3 = OB_BOR_LEVEL3
} TM_BOR_Level_t;
 
typedef enum {
	TM_BOR_Result_Ok,   /*!< Everything OK */
	TM_BOR_Result_Error /*!< An error has occurred */
} TM_BOR_Result_t;

/**
 * @}
 */

/**
 * @defgroup TM_BOR_Functions
 * @brief    Library Functions
 * @{
 */

TM_BOR_Result_t TM_BOR_Set(TM_BOR_Level_t BORValue);
TM_BOR_Level_t TM_BOR_Get(void);

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
