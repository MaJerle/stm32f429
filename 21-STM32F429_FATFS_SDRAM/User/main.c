/**
 *	Keil project template for using FATFS with SDRAM
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
 *
 * Check defines.h file for configuration settings about SDRAM and FATFS
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_fatfs.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* FATFS related */
FATFS fs;
FIL fil;
FRESULT fres;
uint8_t SD_Buffer[15];

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Mount FATFS on SDRAM */
	fres = f_mount(&fs, "SDRAM:", 0);
	
	/* Create FAT volume with default cluster size, IMPORTANT for SDRAM because content is not stored forever! */
	/* This has to be done only once, when you first time init your SDRAM */
	/* It is initialized first time using f_mount() function call above */
	fres = f_mkfs("SDRAM:", 0, 0);
	
	/* Open file */
	fres = f_open(&fil, "SDRAM:test.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	
	/* Write data */
	fres = f_write(&fil, "Hello world", 11, NULL);
	
	/* Close file */
	fres = f_close(&fil);
	
	/* Unmount */
	f_mount(NULL, "SDRAM:", 1);
	
	/* If everything succedded */
	if (fres == FR_OK) {
		/* Mount FATFS on SDRAM */
		fres = f_mount(&fs, "SDRAM:", 0);
		
		/* No need to call MKFS here again because fat system is already on SDRAM */
		
		/* Open file */
		fres = f_open(&fil, "SDRAM:test.txt", FA_READ | FA_WRITE);
		
		/* Read data */
		fres = f_read(&fil, SD_Buffer, 11, NULL);
		
		/* Close file */
		fres = f_close(&fil);
		
		/* Unmount SDRAM */
		f_mount(NULL, "SDRAM:", 1);
		
		/* Turn on LED GREEN */
		if (strncmp((char *)SD_Buffer, "Hello world", 11) == 0) {
			/* Turn on GREEN led */
			TM_DISCO_LedOn(LED_GREEN);
		} else {
			/* Red led */
			TM_DISCO_LedOn(LED_RED);
		}
	} else {
		/* Red led */
		TM_DISCO_LedOn(LED_RED);
	}
	
	/* Main loop */
	while (1) {
		
	}
}
