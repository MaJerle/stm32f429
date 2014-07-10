/**
 *	Keil project for true random number generator
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_rng.h"
#include <stdio.h>

int main(void) {
	char buffer[20];
	
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initialize USART1, TX: PA9, RX: PA10
	TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);	
	//Initialize random number generator
	TM_RNG_Init();
	
	while (1) {
		sprintf(buffer, "Number: %u\n", TM_RNG_Get());
		TM_USART_Puts(USART1, buffer);
		Delayms(1000);
	}
}
