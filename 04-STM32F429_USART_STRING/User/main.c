/**
 *	Keil project for USART using strings
 *
 *  Before you start, select your target, on the right of the "Load" button
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
#include "tm_stm32f4_usart.h"

int main(void) {
	char buffer[100];
	
	/* Initialize system */
	SystemInit();

	/* Initialize USART1 at 115200 baud, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Put string to USART */
	TM_USART_Puts(USART1, "Hello world\n");

	while (1) {
		/* Get string from internal buffer */
		if (TM_USART_Gets(USART1, buffer, sizeof(buffer))) {
			/* Return string back */
			TM_USART_Puts(USART1, buffer);
		}
	}
}

