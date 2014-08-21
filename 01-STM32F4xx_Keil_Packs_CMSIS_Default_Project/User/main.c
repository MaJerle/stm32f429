/**
 *	Keil project
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries */
#include "defines.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Init delay functions */
	TM_DELAY_Init();
	
	/* LEDs init */
	TM_DISCO_LedInit();
	
	TM_DELAY_DisableSystick();
	TM_DELAY_EnableSystick();

	while (1) {
		/* Toggle green led */
		TM_DISCO_LedToggle(LED_GREEN);
		/* Delay 500ms */
		Delayms(500);
	}
}
