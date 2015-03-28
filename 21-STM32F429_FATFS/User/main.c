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
#include "tm_stm32f4_fatfs.h"
#include <stdio.h>
#include <string.h>

/* Fatfs object */
FATFS FatFs;
/* File object */
FIL fil;

int main(void) {
	/* Free and total space */
	uint32_t total, free;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delays */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();

	/* Mount drive */
	if (f_mount(&FatFs, "0:", 1) == FR_OK) {
		/* Mounted OK, turn on RED LED */
		TM_DISCO_LedOn(LED_RED);
		
		/* Try to open file */
		if (f_open(&fil, "0:1stfile.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
			/* File opened, turn off RED and turn on GREEN led */
			TM_DISCO_LedOn(LED_GREEN);
			TM_DISCO_LedOff(LED_RED);
			
			/* If we put more than 0 characters (everything OK) */
			if (f_puts("First string in my file\n", &fil) > 0) {
				if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {
					/* Data for drive size are valid */
				}
				
				/* Turn on both leds */
				TM_DISCO_LedOn(LED_GREEN | LED_RED);
			}
			
			/* Close file, don't forget this! */
			f_close(&fil);
		}
		
		/* Unmount drive, don't forget this! */
		f_mount(0, "0:", 1);
	}
	
	while (1) {

	}
}

