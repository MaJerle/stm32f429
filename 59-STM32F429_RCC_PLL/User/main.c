/**
 *	Keil project for changing PLL settings when device is running
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
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_rcc.h"

#include "stdio.h"

int main(void) {
	/* RCC clocks typedef */
	RCC_ClocksTypeDef RCC_Clocks;
	/* RCC PLL structure */
	TM_RCC_PLL_t PLL;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay, systick has interrupts every 1ms by default */
	TM_DELAY_Init();
	
	/* Init USART2, TX: PA2, RX: PA3, 921600 baud */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	/* Get current system clock */
	RCC_GetClocksFreq(&RCC_Clocks);
	
	/* Print current system clock */
	printf("System core clock: %d\n", RCC_Clocks.SYSCLK_Frequency);
	
	/* Read PLL settings */
	TM_RCC_GetPLL(&PLL);
	
	/* Set PLL N to some random value */
	PLL.PLLN = 200;
	
	/* Change PLL settings */
	TM_RCC_SetPLL(&PLL);
	
	/* Send something over USART */
	printf("This will not be seen correct on USART\n");
	
	/* Reinit everything with new clock */
	/* You have to reinit everything what depends on system CLOCK */
	/* Like timer prescalers, usarts, i2c, etc */
	
	/* Set new systick preload value */
	TM_DELAY_Init();
	
	/* Set new prescalers for USART settings */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	/* Get current system clock */
	RCC_GetClocksFreq(&RCC_Clocks);
	
	/* Print current system clock */
	/* This will be seen on console with new settings */
	printf("System core clock after PLL change: %d\n", RCC_Clocks.SYSCLK_Frequency);
	
	while (1) {

	}
}

/* Handle printf functionality */
int fputc(int ch, FILE* fil) {
	/* Send character over USART */
	TM_USART_Putc(USART2, ch);
	
	/* Return character */
	return ch;
}

/* Handle 1ms interrupts from systick timer */
void TM_DELAY_1msHandler(void) {
	/* Do work, called every 1ms */
}
