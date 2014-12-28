/**
 *	Simple STDIO output & input stream implementation for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-41-stdio-implementation-for-stm32f4
 *	@version 	v1.1
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
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
 * Version 1.1
 * 	- October 19, 2014
 * 	- Added input options
 *
 * Version 1.0
 * 	- October 18, 2014
 * 	- Initial release
 *
 * This library allows you to use printf (stdout) and fprintf functions on STM32F4
 * to output data to the stream.
 *
 * As of version 1.1, you are now able to handle input (stdin) data from standard
 * stream or user defined.
 */
#ifndef TM_STDIO_H
#define TM_STDIO_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

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
 * File structure for stdio stream output
 *
 * Parameters:
 * 	- int (*outputFuncPointer)(int, FILE *):
 * 		Pointer to function to call when need to output data to stream
 * 	- int (*inputFuncPointer)(FILE *):
 * 		Pointer to function to call when trying to get data from stream
 */
struct __FILE {
	int (*outputFuncPointer)(int, FILE *);
	int (*inputFuncPointer)(FILE *);
};

/**
 * Link file output stream with output put character function
 *
 * Parameters:
 * 	- FILE* f
 * 		Pointer to file stream
 * 	- int (*outputFuncPointer)(int, FILE *)
 * 		Pointer to function that will be used to output data to stream
 *
 * No return
 */
extern void TM_STDIO_SetOutputFunction(FILE* f, int (*outputFuncPointer)(int, FILE *));

/**
 * Link file input stream with input get character function
 *
 * Parameters:
 * 	- FILE* f:
 * 		Pointer to file stream
 * 	- int (*inputFuncPointer)(FILE *):
 * 		Pointer to function that will be used to input data from stream
 *
 * No return
 */
extern void TM_STDIO_SetInputFunction(FILE* f, int (*inputFuncPointer)(FILE *));

/**
 * Default output handler for standard output (stdout)
 * Needs to be implemented by user if you want to use printf function.
 * This function has __weak parameters to prevent link errors if it is not implemented by user
 *
 * Parameters:
 * 	- int c:
 * 		Character for output to the stream
 * 	- FILE* f:
 * 		Pointer to file stream
 *
 * Returns character value if write is OK
 * Returns -1 if you want to stop write at any time
 */
__weak int TM_STDIO_StdoutHandler(int c, FILE* f);

/**
 * Default input handler for standard input (stdin)
 * Needs to be implemented by user if you want to get data from standard input.
 * This function has __weak parameter to prevent link errors if it is not implemented by user
 *
 * Parameters:
 * 	- FILE* f:
 * 		Pointer to file stream
 *
 * Returns character value if exists
 * Returns -1 if at the end of string or no data available
 */
__weak int TM_STDIO_StdinHandler(FILE* f);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
