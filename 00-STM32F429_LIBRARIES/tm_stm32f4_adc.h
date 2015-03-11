/**
 * ADC library for STM32F4xx
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-06-ad-converter-on-stm32f4xx/
 *	@version 	v1.2
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
 * Version 1.2
 *	- March 08, 2015
 *	- Support for new GPIO system
 *
 * Version 1.1
 * 	- November 02, 2014
 *	- Added support for measure Vbat pin with ADC
 *	
 * Pinout
 *	CHANNEL			ADC1	ADC2	ADC3
 *	0				PA0		PA0		PA0
 *	1				PA1		PA1		PA1
 *	2				PA2		PA2		PA2
 *	3				PA3		PA3		PA3
 *	4				PA4		PA4		PF6
 *	5				PA5		PA5		PF7
 *	6				PA6		PA6		PF8
 *	7				PA7		PA7		PF9
 *	8				PB0		PB0		PF10
 *	9				PB1		PB1		PF3
 *	10				PC0		PC0		PC0
 *	11				PC1		PC1		PC1
 *	12				PC2		PC2		PC2
 *	13				PC3		PC3		PC3
 *	14				PC4		PC4		PF4
 *	15				PC5		PC5		PF5
 */
#ifndef TM_ADC_H
#define TM_ADC_H 120
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx ADC
 * - defines.h
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

/**
 * This values can be overwritten in defines.h file
 */
#ifndef TM_ADC1_RESOLUTION
#define TM_ADC1_RESOLUTION		ADC_Resolution_12b
#endif

#ifndef TM_ADC2_RESOLUTION
#define TM_ADC2_RESOLUTION		ADC_Resolution_12b
#endif

#ifndef TM_ADC3_RESOLUTION
#define TM_ADC3_RESOLUTION		ADC_Resolution_12b
#endif

/* Supply voltage is 3.3V */
#ifndef ADC_SUPPLY_VOLTAGE
#define ADC_SUPPLY_VOLTAGE		3300
#endif

/* Get multipliers for vbat, vref measurements */
#if defined (STM32F40_41xxx)
#define ADC_VBAT_MULTI			2
#endif
#if defined (STM32F427_437xx) || defined (STM32F429_439xx) || defined (STM32F401xx) || defined (STM32F411xE)
#define ADC_VBAT_MULTI			4
#endif

/* Channel you want to use */
typedef enum {
	TM_ADC_Channel_0,
	TM_ADC_Channel_1,
	TM_ADC_Channel_2,
	TM_ADC_Channel_3,
	TM_ADC_Channel_4,
	TM_ADC_Channel_5,
	TM_ADC_Channel_6,
	TM_ADC_Channel_7,
	TM_ADC_Channel_8,
	TM_ADC_Channel_9,
	TM_ADC_Channel_10,
	TM_ADC_Channel_11,
	TM_ADC_Channel_12,
	TM_ADC_Channel_13,
	TM_ADC_Channel_14,
	TM_ADC_Channel_15,
	TM_ADC_Channel_16,
	TM_ADC_Channel_18,
	TM_ADC_Channel_19
} TM_ADC_Channel_t;

/**
 * Initialize ADCx peripheral
 *
 * Parameters:
 * 	- ADC_TypeDef* ADCx: ADCx
 * 		ADC1, ADC2, ADC3
 */
extern void TM_ADC_InitADC(ADC_TypeDef* ADCx);

/**
 * Initialize ADCx with ADCx channel
 *
 * Parameters:
 * 	- ADC_TypeDef* ADCx: ADCx
 * 		ADC1, ADC2, ADC3
 * 	- uint8_t channel: channel (pin) for ADCx
 * 		- ADC_Channel_0, ADC_Channel_1, ..., ADC_Channel_15
 */
extern void TM_ADC_Init(ADC_TypeDef* ADCx, uint8_t channel);

/**
 * Read from ADCx channel
 *
 * Parameters:
 * 	- ADC_TypeDef* ADCx: ADCx
 * 		ADC1, ADC2, ADC3
 * 	- uint8_t channel: channel (pin) for ADCx
 * 		- ADC_Channel_0, ADC_Channel_1, ..., ADC_Channel_15
 */
extern uint16_t TM_ADC_Read(ADC_TypeDef* ADCx, uint8_t channel);

/**
 * Enable Vbat channel for ADC
 *
 * No return
 */
extern void TM_ADC_EnableVbat(void);

/**
 * Disable Vbat channel for ADC
 *
 * No return
 */
extern void TM_ADC_DisableVbat(void);

/**
 * Read vbat pin voltage
 *
 * Parameters:
 * 	- ADC_TypeDef* ADCx: ADCx
 * 		ADC1, ADC2, ADC3
 *
 * Result is in mV
 */
extern uint16_t TM_ADC_ReadVbat(ADC_TypeDef* ADCx);

#endif
