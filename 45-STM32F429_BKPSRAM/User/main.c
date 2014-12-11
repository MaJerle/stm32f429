/**
 *	Keil project example for backup SRAM on STM32F4
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 *	This will not work in STM32F401/411, these devices don't have backup SRAM.
 *	If you will try to use this library, HardFault error will happen. 
 *
 *	If you see RED led, then press reset button.
 *	After that, you should see GREEN led, which indicates successful write
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_bkpsram.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
		
	/* Initialize backup SRAM */
	TM_BKPSRAM_Init();
	
	/* Read from backup SRAM location 0x00 and check for value */
	/* if it is the same as one we wrote */
	if (TM_BKPSRAM_Read8(0x00) == 0x15) {
		/* Indicator that value after reset is still the same in backup SRAM */
		
		/* Turn on green LED */
		TM_DISCO_LedOn(LED_GREEN);
	} else {
		/* Write 8-bit value at backup SRAM location 0x00 */
		TM_BKPSRAM_Write8(0x00, 0x15);
		
		/* Turn on red LED */
		TM_DISCO_LedOn(LED_RED);
	}
	
	while (1) {
		
	}
}
