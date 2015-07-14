/**
 *	Keil project for HD44780 LCD controller
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
#include "tm_stm32f4_hd44780.h"

int main(void) {
	/* Rectangle for custom character */
	/* xxx means doesn't care, lower 5 bits are important for LCD */
	uint8_t customChar[] = {
		0x1F,	/*  xxx 11111 */
		0x11,	/*  xxx 10001 */
		0x11,	/*  xxx 10001 */
		0x11,	/*  xxx 10001 */
		0x11,	/*  xxx 10001 */
		0x11,	/*  xxx 10001 */
		0x11,	/*  xxx 10001 */
		0x1F	/*  xxx 11111 */
	};
	
	/* Initialize system */
	SystemInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Initialize LCD 20 cols x 4 rows */
	TM_HD44780_Init(20, 4);
	
	/* Save custom character on location 0 in LCD */
	TM_HD44780_CreateChar(0, &customChar[0]);
	
	/* Put string to LCD */
	TM_HD44780_Puts(0, 0, "STM32F4/29 Discovery");
	TM_HD44780_Puts(2, 1, "20x4 HD44780 LCD");
	TM_HD44780_Puts(0, 2, "stm32f4-\n\r       discovery.com");

	/* Wait a little */
	Delayms(3000);
	
	/* Clear LCD */
	TM_HD44780_Clear();
	
	/* Show cursor */
	TM_HD44780_CursorOn();
	
	/* Write new text */
	TM_HD44780_Puts(6, 2, "CLEARED!");
	
	/* Wait a little */
	Delayms(1000);
	
	/* Enable cursor blinking */
	TM_HD44780_BlinkOn();
	
	/* Show custom character at x = 1, y = 2 from RAM location 0 */
	TM_HD44780_PutCustom(1, 2, 0);

	while (1) {
	
	}
}
