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
	uint16_t *index;
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	TM_DISCO_ButtonInit();
	
	TM_LCD_Init();
	
	/* Turn on all leds */
	TM_DISCO_LedOn(LED_ALL);
	
	while (1) {
		if (TM_DISCO_ButtonPressed()) {		
			uint16_t *index ;

			/* erase memory */
			for (index = (uint16_t *)LCD_FRAME_BUFFER; index < (uint16_t *)(LCD_FRAME_BUFFER+(LCD_FRAME_OFFSET)); index++) {
				*index = 0x1234;
			}
		}
	}
}
