/**
 * ADC library for STM32F4xx
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-06-ad-converter-on-stm32f4xx/
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	
 *	Pinout
 *		CHANNEL			ADC1	ADC2	ADC3
 *		0				PA0		PA0		PA0
 *		1				PA1		PA1		PA1
 *		2				PA2		PA2		PA2
 *		3				PA3		PA3		PA3
 *		4				PA4		PA4		PF6
 *		5				PA5		PA5		PF7
 *		6				PA6		PA6		PF8
 *		7				PA7		PA7		PF9
 *		8				PB0		PB0		PF10
 *		9				PB1		PB1		PF3
 *		10				PC0		PC0		PC0
 *		11				PC1		PC1		PC1
 *		12				PC2		PC2		PC2
 *		13				PC3		PC3		PC3
 *		14				PC4		PC4		PF4
 *		15				PC5		PC5		PF5
 */
#ifndef TM_ADC_
#define TM_ADC_ 100
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx ADC
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "defines.h"

/**
 * This values can be overwriten in defines.h file
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
 * Initialize pin for ADC
 * 
 * Parameters:
 * 	- uint16_t RCCx: RCC constant
 * 	- GPIO_TypeDef* GPIOx: pointer to GPIOx port
 * 	- uint16_t PinX: pin number
 * 	
 * Called internally
 */
extern void TM_ADC_InitPin(uint16_t RCCx, GPIO_TypeDef* GPIOx, uint16_t PinX);

/**
 * Initialize ADC pins
 *
 * Parameters:
 * 	- ADC_TypeDef* ADCx: Select ADC
 *		- ADC1, ADC2, ADC3
 */
extern void TM_ADC_Channel_0_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_1_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_2_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_3_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_4_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_5_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_6_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_7_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_8_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_9_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_10_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_11_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_12_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_13_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_14_Init(ADC_TypeDef* ADCx);
extern void TM_ADC_Channel_15_Init(ADC_TypeDef* ADCx);

#endif
