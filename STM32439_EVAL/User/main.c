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
#include "tm_stm32f4_sdram.h"
#include "stm324x9i_eval_lcd.h"

int main(void) {
	uint32_t i;
	__IO uint16_t *index;
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	TM_DISCO_ButtonInit();
	
	LCD_Init();
	
	TM_SDRAM_Write16(0x1234, 0x321);
	if (TM_SDRAM_Read16(0x1234) == 0x321) {	
		TM_DISCO_LedOn(LED_GREEN);
	} else  {	
		TM_DISCO_LedOn(LED_RED);
	}
	
	while (1) {
		if (TM_DISCO_ButtonPressed()) {
			LCD_Clear(0x6541);
		}
	}
}
