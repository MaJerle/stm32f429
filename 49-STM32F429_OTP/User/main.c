/**
 *	Keil project example for OTP memory
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
#include "tm_stm32f4_otp.h"
#include <stdio.h>

int main(void) {
	uint8_t d;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();

	/* Write data to block 15, byte number 6, write 0x96 to OTP memory */
	if (TM_OTP_Write(15, 6, 0x96) == TM_OTP_Result_Ok) {
		/* Turn on green LED */
		TM_DISCO_LedOn(LED_GREEN);
	}
	
	/* Lock block 15 forever */
	/* Be careful when you do this!! */
	//TM_OTP_BlockLock(15);
	
	/* Read data from block 15, byte number 6 from OTP memory */
	d = TM_OTP_Read(15, 6);
	
	/* Check if they are the same */
	if (d == 0x96) {
		/* Written data is the same as data we read back */
		TM_DISCO_LedOn(LED_RED);
	}
	
	/* If block 15 is locked */
	if (TM_OTP_BlockLocked(15)) {
		/* Do your stuff here if it is locked */
	}
	
	while (1) {
		
	}
}
