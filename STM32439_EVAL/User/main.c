/**
 *	Keil project template
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_lcd.h"
#include "tm_stm32f4_sdram.h"

int main(void) {
	uint32_t i;
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	if (TM_SDRAM_Init()) {
		TM_DISCO_LedOn(LED_GREEN);
	} else {
		TM_DISCO_LedOn(LED_RED);
	}
	
	TM_LCD_Init();
	
	TM_SDRAM_Write32(1, 0x12345678);
	
	for (i = 0; i < (640 * 480 * 2); i += 1) {
		TM_SDRAM_Write16(i, 0x1234);
		TM_SDRAM_Write16(LCD_FRAME_OFFSET + i, 0x1234);
	}
	
	/* Turn on all leds */
	TM_DISCO_LedOn(LED_ALL);
	
	/* Delay 2 seconds */
	Delayms(2000);
	
	while (1) {
		/* Toggle leds */
		TM_DISCO_LedToggle(LED_ALL);
		
		/* Wait 500ms */
		Delayms(500);
	}
}
