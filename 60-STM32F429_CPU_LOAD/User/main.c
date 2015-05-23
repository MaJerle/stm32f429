/**
 *	Keil project for CPU load monitor
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
#include "tm_stm32f4_cpu_load.h"
#include "tm_stm32f4_usart.h"

#include "stdio.h"

int main(void) {
	__IO uint32_t i;
	
	/* CPU load structure */
	TM_CPULOAD_t CPU_LOAD;
	
	/* Init CPU load monitor */
	TM_CPULOAD_Init(&CPU_LOAD);
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Init button */
	TM_DISCO_ButtonInit();
	
	/* Init USART2, TX: PA2, RX: PA3, 921600 bauds */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	while (1) {
		/* Check if CPU LOAD variable is updated */
		if (CPU_LOAD.Updated) {
			/* Print to user */
			printf("W: %u; S: %u; Load: %5.2f\n", CPU_LOAD.WCNT, CPU_LOAD.SCNT, CPU_LOAD.Load);
		}
		
		/* Toggle leds */
		TM_DISCO_LedToggle(LED_ALL);
		
		/* If button pressed, do some useless counting */
		if (TM_DISCO_ButtonPressed()) {
			/* Count something to waste some time before entering to sleep mode */
			i = 0;
			while (i++ < 0x1FFF);
		}
		
		/* Go low power mode, sleep mode until interrupt, measure CPU load */
		TM_CPULOAD_GoToSleepMode(&CPU_LOAD, TM_LOWPOWERMODE_SleepUntilInterrupt);
	}
}

/* Handle printf functionality */
int fputc(int ch, FILE* fil) {
	/* Send character over USART */
	TM_USART_Putc(USART2, ch);
	
	/* Return character */
	return ch;
}
