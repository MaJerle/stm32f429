/**
 *	Keil project for LEDS and BUTTONS on Discovery and Nucleo boards
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_disco.h"

int main(void) {
	/* Initialize System */
	SystemInit();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	/* Initialize button on board */
	TM_DISCO_ButtonInit();
	
    while(1) {
		/* If button pressed */
    	if (TM_DISCO_ButtonPressed()) {
			/* Turn on leds */
    		TM_DISCO_LedOn(LED_RED | LED_GREEN);
    	} else {
			/* Turn off leds */
    		TM_DISCO_LedOff(LED_RED | LED_GREEN);
    	}
    }
}
