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
	while (1) {
		//Toggle leds
		GPIO_ToggleBits(GPIOG, LED_GREEN);
		//Delay 500ms
		Delayms(500);
	}
}
