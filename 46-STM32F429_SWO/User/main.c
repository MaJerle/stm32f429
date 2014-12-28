/**
 *	Keil project template
 *
 *	Before you start, select your target, on the right of the "Load" button
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
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_swo.h"

#include <stdio.h>

volatile uint32_t i;

int main(void) {
	SystemInit();
	
	/* Initialize SWO output */
	TM_SWO_Init();
	
	/* Leds init */
	TM_DISCO_LedInit();
	
	/* Delay init */
	TM_DELAY_Init();
	
	/* Print via SWO, debug purpose */
	TM_SWO_Printf("123");
	
	//TM_SWO_Printf();
	
	while (1) {		
		/* Print via SWO */
		TM_SWO_Printf("%d\n", TM_DELAY_Time());
		
		Delayms(100);
	}
}
