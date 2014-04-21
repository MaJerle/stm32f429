/**
 * Includes
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
