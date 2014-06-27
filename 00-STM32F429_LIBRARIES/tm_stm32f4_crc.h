/**
 * 16bit CRC Generator
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_CRC_
#define TM_CRC_ 100
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
extern uint16_t TM_CRC_Generate(uint8_t* data, uint8_t length);

#endif

