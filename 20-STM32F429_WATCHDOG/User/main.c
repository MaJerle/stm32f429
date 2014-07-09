/**
 *	Keil project
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_watchdog.h"

int main(void) {
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initialize leds on board
	TM_DISCO_LedInit();
	//Initialize button
	TM_DISCO_ButtonInit();
	//Initialize watchdog timer
	//Set timeout to 1s
	if (TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_1s)) {
		//System was reset by watchdog
		TM_DISCO_LedOn(LED_RED);
	} else {
		//System was not reset by watchdog
		TM_DISCO_LedOn(LED_GREEN);
	}
	
	while (1) {
		//if button is pressed, do nothing and system will be reset after 1 second
		while (TM_DISCO_ButtonPressed());
		
		//Reset watchdog
		TM_WATCHDOG_Reset();
	}
}
