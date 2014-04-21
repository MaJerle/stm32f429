/**
 *	Pretty precise delay functions with SysTick timer
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"

int main(void) {
	//Initialize system for 180MHz core clock
	SystemInit();

	//Initialize delay Systick timer
	TM_DELAY_Init();
	//Initialize onboard leds
	TM_DISCO_LedInit();

	TM_Time = 0;
	while (1) {
		if (TM_Time >= 500000) { //Every 500ms
			TM_Time = 0;
			//Toggle LED here
		}
		//Do other stuff here
	}
}
