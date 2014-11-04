/**
 *	Keil project for Mifare RC522 RFID reader
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
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_mfrc522.h"
#include "tm_stm32f4_disco.h"

#include <stdio.h>

int main(void) {
	/* Recognized card ID */
	uint8_t CardID[5];
	/* My cards id */
	/* I read them with program below, and write this here */
	uint8_t MyID[5] = {
		0x43, 0xdc, 0x52, 0xb6, 0x7b	/* My card on my keys */
	};
	char buffer[50];

	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds */
	TM_DISCO_LedInit();
	
	/* Initialize LCD */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize MFRC522 RFID */
	TM_MFRC522_Init();
	
	/* Watermark */
	TM_ILI9341_Puts(90, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
	
	while (1) {
		/* If any card detected */
		if (TM_MFRC522_Check(CardID) == MI_OK) {
			/* CardID is valid */
			
			/* Check if this is my card */
			if (TM_MFRC522_Compare(CardID, MyID) == MI_OK) {
				TM_ILI9341_Puts(10, 150, "Hello tilz0R!", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);
			} else {
				TM_ILI9341_Puts(10, 150, "Good bye man!", &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
			}
			/* Print on LCD */
			TM_ILI9341_Puts(10, 10, "Card detected    ", &TM_Font_11x18, 0x00, 0xFFFF);
			sprintf(buffer, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
			TM_ILI9341_Puts(10, 30, buffer, &TM_Font_11x18, 0x00, 0xFFFF);
		} else {
			/* Some printing to delete content */
			TM_ILI9341_Puts(10, 10, "Card not detected", &TM_Font_11x18, 0x00, 0xFFFF);
			TM_ILI9341_Puts(10, 150, "             ", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
			TM_ILI9341_Puts(10, 30, "    \n    \n    \n    \n    ", &TM_Font_11x18, 0x00, 0xFFFF);
		}
	}
}
