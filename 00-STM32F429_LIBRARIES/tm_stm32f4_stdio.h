/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/10/library-41-stdio-implementation-for-stm32f4
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Simple STDIO output & input stream implementation for STM32F4
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
#ifndef TM_STDIO_H
#define TM_STDIO_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_STDIO
 * @brief    Simple STDIO output & input stream implementation for STM32F4 - http://stm32f4-discovery.com/2014/10/library-41-stdio-implementation-for-stm32f4
 * @{
 *
 * This library allows you to use printf (stdout) and fprintf functions on STM32F4
 * to output data to the stream.
 *
 * As of version 1.1, you are now able to handle input (stdin) data from standard
 * stream or user defined input.
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - October 19, 2014
  - Added input options

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
 - attributes.h
 - stdio.h
@endverbatim
 */

/**
 * Library dependencies
 * - STM32F4xx
 * - defines.h
 * - attributes.h
 * - stdio.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "attributes.h"
#include "stdio.h"

/**
 * @defgroup TM_STDIO_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  File structure for stdio stream output	
 */
struct __FILE {
	int (*outputFuncPointer)(int, FILE *); /*!< Pointer to function to call when need to output data to stream */
	int (*inputFuncPointer)(FILE *);       /*!< Pointer to function to call when trying to get data from stream */
};

/**
 * @}
 */

/**
 * @defgroup TM_STDIO_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Links file output stream with output put character function
 * @param  *f: Pointer to file stream
 * @param  *outputFuncPointer: Pointer to function to be used to output data to stream
 * @retval None
 */
void TM_STDIO_SetOutputFunction(FILE* f, int (*outputFuncPointer)(int, FILE *));

/**
 * @brief  Links file input stream with input get character function
 * @param  *f: Pointer to file stream
 * @param  *inputFuncPointer: Pointer to function to be used for input data from stream
 * @retval None
 */
void TM_STDIO_SetInputFunction(FILE* f, int (*inputFuncPointer)(FILE *));

/**
 * @brief  Default output handler for standard output (stdout)
 * @note   Needs to be implemented by user if you want to use printf function
 * @param  c: character to output to stream
 * @param  *f: Pointer to file stream
 * @retval Success status:
 *            - -1: Stop writing any data
 *            - character: Return back the same character as was passed as parameter
 *                 to continue with more characters if needed
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
int TM_STDIO_StdoutHandler(int c, FILE* f);

/**
 * @brief  Default input handler for standard input (stdin)
 * @note   Needs to be implemented by user if you want to use getc or gets functions
 * @param  *f: Pointer to file stream
 * @retval Success status:
 *            - -1: No data available
 *            - character: Return character value
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
int TM_STDIO_StdinHandler(FILE* f);

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
