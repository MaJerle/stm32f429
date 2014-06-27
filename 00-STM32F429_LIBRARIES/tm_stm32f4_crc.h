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

extern uint16_t TM_CRC_Generate(uint8_t* data, uint8_t length);

#endif

