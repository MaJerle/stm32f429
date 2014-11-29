/**
 *	Keil project for delay using TIMx
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	To see, how timer is configured for delay, open project's defines.h file
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

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Init LEDs */
	TM_DISCO_LedInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	while (1) {
		/* Toggle all leds */
		TM_DISCO_LedToggle(LED_ALL);
		
		/* Delay 500us */
		Delay(500);
	}
}
