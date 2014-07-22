/**
 * DAC library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-07-da-converter-on-stm32f4xx/
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	
 *	This library provides 12bit digital to analog output, values from 0 to 4095
 *	
 *	Pinout
 *
 * 		DAC	channel		Pin
 * 		
 * 		DAC1			PA4
 * 		DAC2			PA5
 */
#ifndef TM_DAC_
#define TM_DAC_ 100
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx DAC
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dac.h"
#include "defines.h"

/**
 * Select which DAC channel
 */
typedef enum {
	TM_DAC1,
	TM_DAC2
} TM_DAC_Channel_t;

/**
 * Initialize DAC channel and it's pin
 *
 * - Parameters:
 * 	- TM_DAC_Channel_t DACx:
 * 		- TM_DAC1, TM_DAC2
 */
extern void TM_DAC_Init(TM_DAC_Channel_t DACx);

/**
 * Set analog value to ADCx
 *
 * - Parameters:
 * 	- TM_DAC_Channel_t DACx:
 * 		- TM_DAC1, TM_DAC2
 * 	- uint16_t value
 * 		12Bit unsigned value for 12bit DAC value
 */
extern void TM_DAC_SetValue(TM_DAC_Channel_t DACx, uint16_t value);

#endif
