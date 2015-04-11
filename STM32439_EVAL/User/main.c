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
#include "tm_stm32f4_dma2d_graphic.h"
#include "tm_stm32f4_i2c.h"

#include <stdio.h>
#include <string.h>

volatile uint32_t index2;

TM_FontDef_t * ActiveFont;
	
char Buffer[100];

int main(void) {
	uint16_t i;
	uint8_t devices = 0;
	
	/* Initialize system */
	SystemInit();
	
	/* Select active font */
	ActiveFont = &TM_Font_11x18;

	/* Init button */
	TM_DISCO_ButtonInit();
	
	/* Init leds */
	TM_DISCO_LedInit();
	
	/* Init LCD */
	TM_LCD_Init();
	
	/* Clear screen */
	index2 = (LCD_FRAME_BUFFER);
	for (; index2 < (LCD_FRAME_BUFFER + LCD_FRAME_OFFSET); index2 += 2) {
		*(__IO uint16_t *)(index2) = 0x1234;
	}
	for (; index2 < (LCD_FRAME_BUFFER + LCD_FRAME_OFFSET * 2); index2 += 2) {
		*(__IO uint16_t *)(index2) = 0x4321;
	}
	
	/* Init DMA2D Graphic library */
	TM_DMA2DGRAPHIC_Init();
	
	/* Init I2C peripheral */
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_3, 100000);
	
	/* Go through all devices */
	for (i = 0x02; i < 0xFF; i += 2) {
		if (TM_I2C_IsDeviceConnected(I2C1, i)) {
			sprintf(Buffer, "Device: 0x%02X", i);
			TM_LCD_Puts(10, 10 + devices++ * (ActiveFont->FontHeight + 2), Buffer, &TM_Font_11x18, 0x1234, 0x0000);
		}
	}
	
	TM_LCD_Puts(10, 10 + devices++ * (ActiveFont->FontHeight + 2), "Test", &TM_Font_11x18, 0x1234, 0x0000);
	
	/* Infinity loop */
	while (1) {
		if (TM_DISCO_ButtonPressed()) {
			TM_DISCO_LedOn(LED_ALL);
		} else {
			TM_DISCO_LedOff(LED_ALL);
		}
	}
}

int fputc(int ch, FILE* f) {
	TM_USART_Putc(USART1, ch);
	
	return ch;
}
