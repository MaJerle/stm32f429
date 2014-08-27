/**
 *	Keil project for USB MSC HOST
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *
 * Select your target right of the "Load" button, compile and download to your discovery/nucleo board
 *
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usb_msc_host.h"

#include <stdio.h>

int main(void) {
	FATFS fs;
	FIL fil;
	uint8_t write = 1;
	
	/* Initialize system */
	SystemInit();
	/* Initialize delay functions */
	TM_DELAY_Init();
	/* Leds init */
	TM_DISCO_LedInit();
	
	/* Initialize USB MSC HOST */
	TM_USB_MSCHOST_Init();
  
	while (1) {
		/* Host Task handler */
		/* This have to be called periodically as fast as possible */
		TM_USB_MSCHOST_Process();
		/* Device is connected and ready to use */
		if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected) {
			/* If we didn't write data already */
			if (write) {
				/* Try to mount */
				if (f_mount(&fs, "0:", 1) == FR_OK) {
					/* Mounted ok */
					/* Try to open file */
					if (f_open(&fil, "usb_msc_host_first_file.txt", FA_READ | FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
						/* We want to write only once */
						write = 0;
						/* Put data */
						f_puts("This is my first file with USB and FatFS\n", &fil);
						f_puts("with USB MSC HOST library from stm32f4-discovery.com", &fil);
						/* Turn GREEN LED On and RED LED Off */
						TM_DISCO_LedOn(LED_GREEN);
						TM_DISCO_LedOff(LED_RED);
					}
					/* Close file */
					f_close(&fil);
				}
				/* Unmount */
				f_mount(0, "", 1);
			}
		} else {
			/* Not inserted, turn on RED led */
			TM_DISCO_LedOn(LED_RED);
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Ready to write next time */
			write = 1;
		}
		
		/* If device is not supported */
		if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_DeviceNotSupported) {
			/* Set leds */
			TM_DISCO_LedOff(LED_GREEN);
			TM_DISCO_LedOn(LED_RED);
			/* While device is not supported */
			/* Waiting for user to turn it off */
			while (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_DeviceNotSupported) {
				/* Process USB */
				TM_USB_MSCHOST_Process();
				/* Toggle leds */
				TM_DISCO_LedToggle(LED_RED | LED_GREEN);
				/* Some delay */
				Delayms(100);
			}
			/* Leds off */
			TM_DISCO_LedOff(LED_GREEN | LED_RED);
		}
	}
}
