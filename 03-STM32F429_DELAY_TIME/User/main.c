/**
 *	Keil project for delay time
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

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize onboard leds */
	TM_DISCO_LedInit();

	/* Reset counter to 0 */
	TM_DELAY_SetTime(0);
	while (1) {
		/* If time is more than 500ms */
		if (TM_DELAY_Time() >= 500) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			/* Toggle leds here */
			TM_DISCO_LedToggle(LED_RED | LED_GREEN);
		}
		/* Place your code here */
		/* Code here will be checked without any delay */
		/* Constantly */
	}
}
