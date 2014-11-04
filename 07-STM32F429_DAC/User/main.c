/**
 *	Keil project for DAC example
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_dac.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize DAC channel 1, pin PA4 */
	TM_DAC_Init(TM_DAC1);
	/* Initialize DAC channel 2, pin PA5 */
	TM_DAC_Init(TM_DAC2);
	
	/* Set 12bit analog value of 1500/4096 * 3.3V */
	TM_DAC_SetValue(TM_DAC1, 1500);
	/* Set 12bit analog value of 2047/4096 * 3.3V */
	TM_DAC_SetValue(TM_DAC2, 2047);
	
	while (1) {
	
	}
}
