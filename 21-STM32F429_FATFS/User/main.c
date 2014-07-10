/**
 *	Keil project
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_fatfs.h"
#include <stdio.h>
#include <string.h>

int main(void) {
	FATFS FatFs;
	FIL fil;
	
	//Initialize system
	SystemInit();
	//Initialize delays
	TM_DELAY_Init();
	//Initialize LEDs
	TM_DISCO_LedInit();

	//Mount drive
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		//Mounted OK, turn on RED LED
		TM_DISCO_LedOn(LED_RED);
		
		//Try to open file
		if (f_open(&fil, "first_file.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
			//File opened, turn off RED and turn on GREEN led
			TM_DISCO_LedOn(LED_GREEN);
			TM_DISCO_LedOff(LED_RED);
			
			//If we put more than 0 characters (everything OK)
			if (f_puts("First string in my file\n", &fil) > 0) {
				//Turn on both leds
				TM_DISCO_LedOn(LED_GREEN | LED_RED);
			}
			
			//Close file
			f_close(&fil);
		}
		
		//Unmount drive
		f_mount(0, "", 1);
	}
	
	
	while (1) {

	}
}

//Use custom get_fattime function
//Implement RTC get time here if you need it
/*
DWORD get_fattime (void) {
	return	  ((DWORD)(2000 - 1980) << 25)	// Year 2013
			| ((DWORD)0 << 21)				// Month 7
			| ((DWORD)0 << 16)				// Mday 28
			| ((DWORD)0 << 11)				// Hour 0
			| ((DWORD)0 << 5)				// Min 0
			| ((DWORD)0 >> 1);				// Sec 0
}
*/

