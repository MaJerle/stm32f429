/**
 *	Keil project example for GENERAL library
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.4.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_general.h"
#include "tm_stm32f4_usart.h"

#include "stdio.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Init button */
	TM_DISCO_ButtonInit();
	
	/* Init USART6, TX: PC6, 115200 baud */
	TM_USART_Init(USART6, TM_USART_PinsPack_1, 115200);
	
	/* Get system reset source and clear flags after read */
	printf("System reset source: %d\n", (uint8_t)TM_GENERAL_GetResetSource(1));
	
	/* Get system reset source and clear flags after read */
	/* You should see number which corresponds to "None", because we cleared flags in statement above */
	printf("System reset source: %d\n", (uint8_t)TM_GENERAL_GetResetSource(1));
	
	/* Get system core and PCLK1 (Peripheral Clock 1, APB1) clocks */
	printf("System core clock: %u Hz; PCLK1 clock: %u Hz\n", 
		TM_GENERAL_GetClockSpeed(TM_GENERAL_Clock_SYSCLK),
		TM_GENERAL_GetClockSpeed(TM_GENERAL_Clock_PCLK1)
	);
	
	while (1) {
		/* If button pressed */
		if (TM_DISCO_ButtonOnPressed()) {
			/* Send to USER */
			printf("Software reset will happen in a moment\n");
			/* Wait a little */
			Delayms(500);
			/* Perform system software reset */
			TM_GENERAL_SystemReset();
		}
	}
}

/* Called from printf */
int fputc(int ch, FILE* f) {
	/* Send over USART */
	TM_USART_Putc(USART6, ch);
	
	/* Return character */
	return ch;
}

