/**
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#ifndef TM_ADC
#define TM_ADC 100

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"

/**
 * Initialize ADC 1
 *
 */
extern void TM_ADC1_Init(void);

/**
 * Initialize ADC 2
 *
 */
extern void TM_ADC2_Init(void);

/**
 * Initialize ADC 3
 *
 */
extern void TM_ADC3_Init(void);

/**
 * Get data from ADCx
 */
extern uint16_t TM_ADC_Read(ADC_TypeDef* ADCx, uint8_t ADC_Channel);

#endif
