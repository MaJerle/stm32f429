/**
 *	Keil project example for CRC
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
#include "tm_stm32f4_crc.h"
#include "tm_stm32f4_swo.h"

/* 32-bit data array for CRC unit */
uint32_t data[16] = {
	0x1234, 0x1234, 0x4562, 0x321,
	0x32, 0x879, 0x987, 0x745, 
	0x74, 0x90, 0x98, 0x2432,
	0x7453, 0xAFDE, 0xAAAA, 0xFFFF
};

int main(void) {
	uint32_t result;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Turn on all leds */
	TM_DISCO_LedOn(LED_ALL);
	
	/* Initialize CRC calculation unit */
	TM_CRC_Init();
	
	/* Calculate CRC from block of 32-bits data array, 16 elements in array, reset CRC unit before start with calculation */
	result = TM_CRC_Calculate32(data, 16, 1);
	
	while (1) {
		/* Toggle leds */
		TM_DISCO_LedToggle(LED_ALL);
		
		/* Some delay */
		Delayms(500);
	}
}
