/**
 *	Simple STDIO output stream implementation for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-41-stdio-implementation-for-stm32f4
 *	@version 	v1.0
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
 * This library allows you to use printf and fprintf functions on STM32F4
 * to output data to the stream.
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
 * - stdio.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "stdio.h"

/**
 * File structure for stdio stream output
 *
 * Parameters:
 * 	- int (*outputFuncPointer)(int, FILE *)
 * 		Pointer to function to call when need to output data
 */
struct __FILE {
	int (*outputFuncPointer)(int, FILE *);
};

/**
 * Link file output stream with handler function
 *
 * Parameters:
 * 	- FILE* f
 * 		Pointer to file stream
 * 	- int (*outputFuncPointer)(int, FILE *)
 * 		Pointer to function that will be used to output data
 *
 * No return
 */
extern void TM_STDIO_SetOutputFunction(FILE* f, int (*outputFuncPointer)(int, FILE *));

/**
 * Custom printf output handler
 * Needs to be implemented by user if you want to use printf function.
 * This function has __weak parameters to prevent link errors if it is not implemented by user
 *
 * Parameters:
 * 	- int c
 * 		Character for output to the stream
 * 	- FILE* f
 * 		Pointer to file stream
 *
 * Returns character value if write is OK
 * Returns -1 if you want to stop write at any time
 */
__weak int TM_STDIO_PrintfHandler(int c, FILE* f);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
