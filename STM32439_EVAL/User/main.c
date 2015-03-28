/**
 *	Keil project for FatFS for SD cards
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
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_lcd.h"
#include "stm324x9i_eval_fmc_sdram.h"
#include <stdio.h>
#include <string.h>

/* Clock speed in MHz */
uint16_t SystemMHz = 180;

static void TM_DELAY2_Init(void) {
	/* Enable TRC */
	CoreDebug->DEMCR &= ~0x01000000;
	CoreDebug->DEMCR |= 0x01000000;
	/* Reset counter */
	DWT->CYCCNT = 0;
	/* Enable counter */
	DWT->CTRL &= ~0x00000001;
	DWT->CTRL |= 0x00000001;
}

static void Delay2(uint32_t micros) {
	unsigned int start, current;

	start = DWT->CYCCNT;

	do {
		/* Get counter value */
		current = DWT->CYCCNT;
	} while((current - start) < (micros * SystemMHz));
}

#define DelayMicros(micros) {DWT->CYCCNT = 0; while (DWT->CYCCNT < (micros * SystemMHz));}


int main(void) {
	uint16_t *index;
	uint8_t i;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delays */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	
	while (1) {
		/* Toggle all leds */
		TM_DISCO_LedToggle(LED_ALL);
		DelayMicros(100);
	}
}

int fputc(int ch, FILE* f) {
	TM_USART_Putc(USART1, ch);
	
	return ch;
}
