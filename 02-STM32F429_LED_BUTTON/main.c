/**
 *	Leds and button example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_disco.h"

int main(void) {
	//Initialize leds on board
	TM_DISCO_LedInit();
	//Initialize button ob board
	TM_DISCO_ButtonInit();
    while(1) {
    	if (TM_DISCO_ButtonPressed()) {
    		TM_DISCO_LedOn(LED_RED | LED_GREEN);
    	} else {
    		TM_DISCO_LedOff(LED_RED | LED_GREEN);
    	}
    }
}
