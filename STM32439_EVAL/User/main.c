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
//#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_general.h"
#include "tm_stm32f4_lcd.h"

#include <stdio.h>
#include <string.h>

volatile uint32_t index2;
	
int main(void) {
	/* Initialize system */
	SystemInit();

	/* Init button */
	TM_DISCO_ButtonInit();
	
	/* Init leds */
	TM_DISCO_LedInit();
	
	/* Init LCD */
	TM_LCD_Init();
	
	/* Infinity loop */
	while (1) {
		if (TM_DISCO_ButtonPressed()) {
			TM_DISCO_LedOn(LED_ALL);
		
			index2 = (LCD_FRAME_BUFFER);
			for (; index2 < (LCD_FRAME_BUFFER + LCD_FRAME_OFFSET); index2 += 2) {
				*(__IO uint16_t *)(index2) = 0x789A;
			}
		} else {
			TM_DISCO_LedOff(LED_ALL);
		}
	}
}

int fputc(int ch, FILE* f) {
	TM_USART_Putc(USART1, ch);
	
	return ch;
}
