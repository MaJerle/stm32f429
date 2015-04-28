/**
 *	Keil project for Random Number Generator
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
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_rng.h"
#include <stdio.h>

int main(void) {
	char buffer[20];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize USART2, TX: PA2, 921600 baud */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);	
	
	/* Initialize random number generator */
	TM_RNG_Init();
	
	while (1) {
		/* Get and format random number */
		sprintf(buffer, "Number: %u\n", TM_RNG_Get());
		
		/* Put to USART */
		TM_USART_Puts(USART2, buffer);
		
		/* Delay 1 second */
		Delayms(1000);
	}
}
