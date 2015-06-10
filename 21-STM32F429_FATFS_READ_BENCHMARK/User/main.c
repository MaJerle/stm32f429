/**
 *	Keil project for FatFS for SD cards with benchmark for READ SPEED
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
#include "tm_stm32f4_usart.h"
#include <stdio.h>
#include <string.h>

/* Fatfs object */
FATFS FatFs;
/* File object */
FIL fil;

/* Create buffer of 20 512bytes long sector */
uint8_t SD_Buffer[512 * 20];

int main(void) {
	/* Free and total space */
	uint32_t write_ok = 0, cnt = 0;
	FRESULT fres;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delays */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	
	/* Initialize USART for debug, USART6, TX: PC6, 921600 bauds */
	TM_USART_Init(USART6, TM_USART_PinsPack_1, 921600);
	
	/* Print debug */
	TM_USART_Puts(USART6, "SDCARD benchmark test for reading and writing\n");
	
	while (1) {

		/* Lets first write something to SDCARD */
		if (!write_ok) {
			/* Mount drive */
			if (f_mount(&FatFs, "SD:", 1) == FR_OK) {
				
				/* Try to open file */
				if ((fres = f_open(&fil, "SD:benc.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE)) == FR_OK) {
					
					/* Reset time */
					TM_DELAY_SetTime(0);
					
					/* Write block of data */
					f_write(&fil, SD_Buffer, sizeof(SD_Buffer), &cnt);
					
					/* We are done here */
					printf("Writing is done. Written %d bytes in %d ms = %u B/s\n", cnt, TM_DELAY_Time(), (cnt * 1000) / TM_DELAY_Time());
					
					/* Save everything for sure */
					f_sync(&fil);
					
					/* Move pointer to the beginning of file */
					f_lseek(&fil, 0);
					
					/* Reset time */
					TM_DELAY_SetTime(0);
					
					/* Read everything from file */
					f_read(&fil, SD_Buffer, sizeof(SD_Buffer), &cnt);
					
					/* We are done here */
					printf("Reading is done. Read %d bytes in %d ms = %u B/s\n", cnt, TM_DELAY_Time(), (cnt * 1000) / TM_DELAY_Time());
					
					/* Close file, don't forget this! */
					f_close(&fil);
					
					/* GREEN LED on */
					TM_DISCO_LedOn(LED_GREEN);
					
					write_ok = 1;
				} else {
					printf("Could not open file for write; FRES = %d\n", fres);
				}
				
				/* Unmount drive, don't forget this! */
				f_mount(0, "SD:", 1);
			} else {
				printf("Could not mount SD CARD\n");
			}
		}		
	}
}

/* Printf handler */
int fputc(int ch, FILE* fil) {
	/* Send to USART */
	TM_USART_Putc(USART6, ch);
	
	/* Return character back */
	return ch;
}

