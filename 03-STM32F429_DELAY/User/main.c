/**
 *	Keil project for delay
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen MAJERLE
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.net
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

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize onboard leds */
	TM_DISCO_LedInit();

	while (1) {
		/* Toggle leds */
		TM_DISCO_LedToggle(LED_GREEN);
		
		/* Delay 500ms */
		Delayms(500);
		/* Delay 500 us */
		//Delay(500);
	}
}
