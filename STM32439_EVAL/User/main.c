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
#include "tm_stm32f4_gps.h"

#include <stdio.h>
#include <string.h>
#include "math.h"

volatile uint32_t index2;
TM_FontDef_t * ActiveFont;
char Buffer[100];
TM_GPS_Data_t GPS;

#define CIRCLE_X   320
#define CIRCLE_Y   240
#define CIRCLE_R   238
#define MULT_X     ((float) 0.707)
#define MULT_Y     ((float) 0.707)
#define DEGREE2RADIAN       0.0174532

void DrawBackground(void) {
	/* Background and circles */
	TM_DMA2DGRAPHIC_DrawFilledCircle(CIRCLE_X, CIRCLE_Y, CIRCLE_R, 0x0000);
/*	TM_DMA2DGRAPHIC_DrawCircle(CIRCLE_X, CIRCLE_Y, CIRCLE_R, 0xAAAA);
	TM_DMA2DGRAPHIC_DrawCircle(CIRCLE_X, CIRCLE_Y, 3 * CIRCLE_R / 4, 0xAAAA);
	TM_DMA2DGRAPHIC_DrawCircle(CIRCLE_X, CIRCLE_Y, CIRCLE_R / 2, 0xAAAA);
	TM_DMA2DGRAPHIC_DrawCircle(CIRCLE_X, CIRCLE_Y, CIRCLE_R / 4, 0xAAAA);
*/	
	/* Draw lines */
	TM_DMA2DGRAPHIC_DrawHorizontalLine(CIRCLE_X - CIRCLE_R, CIRCLE_Y, 2 * CIRCLE_R, 0xAAAA);
/*	TM_DMA2DGRAPHIC_DrawLine(CIRCLE_X, CIRCLE_Y - CIRCLE_R, CIRCLE_X, CIRCLE_Y + CIRCLE_R, 0xAAAA);
	TM_DMA2DGRAPHIC_DrawLine(CIRCLE_X - CIRCLE_R * MULT_X, CIRCLE_Y - CIRCLE_R * MULT_Y, CIRCLE_X + CIRCLE_R * MULT_X, CIRCLE_Y + CIRCLE_R * MULT_Y, 0xAAAA);
	TM_DMA2DGRAPHIC_DrawLine(CIRCLE_X - CIRCLE_R * MULT_X, CIRCLE_Y + CIRCLE_R * MULT_Y, CIRCLE_X +	CIRCLE_R * MULT_X, CIRCLE_Y - CIRCLE_R * MULT_Y, 0xAAAA);
*/
}

int main(void) {
	uint16_t i;
	uint16_t x, y;
	uint8_t devices = 0;
	
	/* Initialize system */
	SystemInit();
	
	/* Select active font */
	ActiveFont = &TM_Font_11x18;

	/* Init button */
	TM_DISCO_ButtonInit();
	
	/* Init leds */
	TM_DISCO_LedInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Init LCD */
	TM_LCD_Init();
	
	/* Init DMA2D Graphic library */
	TM_DMA2DGRAPHIC_Init();
	
	/* Set time to 0 */
	TM_DELAY_SetTime(0);
	
	/* Clear screen */
	TM_DMA2DGRAPHIC_DrawFilledRectangle(0, 0, 640, 480, 0x1234);
	
	sprintf(Buffer, "%u", TM_DELAY_Time());
	TM_LCD_Puts(10, 10, Buffer, &TM_Font_11x18, 0x1234, 0x000);
	
	TM_DELAY_SetTime(0);
	DrawBackground();
	sprintf(Buffer, "%u", TM_DELAY_Time());
	
	TM_LCD_Puts(10, 30, Buffer, &TM_Font_11x18, 0x1234, 0x000);
	
	/* Fill fake GPS data */
	GPS.SatellitesInView = 4;
	
	GPS.SatDesc[0].ID = 2;
	GPS.SatDesc[0].Elevation = 45;
	GPS.SatDesc[0].Azimuth = 45;
	GPS.SatDesc[0].SNR = 31;
	
	GPS.SatDesc[1].ID = 6;
	GPS.SatDesc[1].Elevation = 26;
	GPS.SatDesc[1].Azimuth = 72;
	GPS.SatDesc[1].SNR = 27;
	
	GPS.SatDesc[2].ID = 12;
	GPS.SatDesc[2].Elevation = 76;
	GPS.SatDesc[2].Azimuth = 341;
	GPS.SatDesc[2].SNR = 10;
	
	GPS.SatDesc[3].ID = 14;
	GPS.SatDesc[3].Elevation = 32;
	GPS.SatDesc[3].Azimuth = 135;
	GPS.SatDesc[3].SNR = 0;
	
	
	/* Infinity loop */
	while (1) {
		if (TM_DISCO_ButtonPressed()) {
			TM_DISCO_LedOn(LED_ALL);
		} else {
			TM_DISCO_LedOff(LED_ALL);
		}
		
		if (TM_DELAY_Time() >= 200) {
			TM_DELAY_SetTime(0);
			
			DrawBackground();
		}
	}
}

int fputc(int ch, FILE* f) {
	TM_USART_Putc(USART1, ch);
	
	return ch;
}
