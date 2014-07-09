/**
 * 16bit CRC Software Generator
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_SOFTCRC_
#define TM_SOFTCRC_ 100
/**
 * Library dependencies
 * 
 * None
 */
/**
 * Includes
 */
#include "stm32f4xx.h"

/**
 * Generate CRC from given data
 *
 * Parameters:
 * 	- uint8_t* data: pointer to data array
 * 	- uint8_t length: length of data
 *
 * Returns 16bit CRC generated
 */
extern uint16_t TM_SOFTCRC_Generate(uint8_t* data, uint8_t length);

#endif

