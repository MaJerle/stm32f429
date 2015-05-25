/**
 *	Keil project example for SSD1306 OLED LCD
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
#include "tm_stm32f4_ssd1306.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Init LEDS */
	TM_DISCO_LedInit();
	
	/* Init SSD1306 LCD 128 x 64 px */
	if (TM_SSD1306_Init()) {
		/* SSD1306 is connected */
		TM_DISCO_LedOn(LED_GREEN);
	} else {
		/* SSD1306 is not connected */
		TM_DISCO_LedOn(LED_RED);
	}
	
	/* Go to location X = 30, Y = 4 */
	TM_SSD1306_GotoXY(30, 4);
	TM_SSD1306_Puts("STM32F4xx", &TM_Font_7x10, SSD1306_COLOR_WHITE);
	
	/* Go to location X = 20, Y = 25 */
	TM_SSD1306_GotoXY(8, 25);
	TM_SSD1306_Puts("I2C SSD1306 LCD", &TM_Font_7x10, SSD1306_COLOR_WHITE);
	
	/* Go to location X = 15, Y = 45 */
	TM_SSD1306_GotoXY(15, 45);
	TM_SSD1306_Puts("Tilen Majerle", &TM_Font_7x10, SSD1306_COLOR_WHITE);
	
	/* Update screen, send changes to LCD */
	TM_SSD1306_UpdateScreen();
	
	while (1) {
		/* Invert pixels */
		TM_SSD1306_ToggleInvert();
		
		/* Update screen */
		TM_SSD1306_UpdateScreen();
		
		/* Make a little delay */
		Delayms(500);
	}
}
