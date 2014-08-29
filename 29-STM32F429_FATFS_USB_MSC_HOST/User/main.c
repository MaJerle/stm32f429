/**
 *	Keil project for USB MSC HOST
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *
 * Select your target right of the "Load" button, compile and download to your discovery board
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"

/* Include fatfs and usb libraries */
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usb_msc_host.h"

#include <stdio.h>

int main(void) {
	FATFS USB_Fs;
	FIL USB_Fil;
	char buffer[50];
	uint8_t write = 1;
	uint32_t free, total;
	
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
				/* Try to mount USB device */
				/* USB is at 1: */
				if (f_mount(&USB_Fs, "1:", 1) == FR_OK) {
					/* Mounted ok */
					/* Try to open USB file */
					if (f_open(&USB_Fil, "1:usb_msc_host_first_file.txt", FA_READ | FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
						/* We want to write only once */
						write = 0;
						
						/* Get total and free space on USB */
						TM_FATFS_USBDriveSize(&total, &free);
						
						/* Put data */
						f_puts("This is my first file with USB and FatFS\n", &USB_Fil);
						f_puts("with USB MSC HOST library from stm32f4-discovery.com\n", &USB_Fil);
						f_puts("----------------------------------------------------\n", &USB_Fil);
						f_puts("USB total and free space:\n\n", &USB_Fil);
						/* Total space */
						sprintf(buffer, "Total: %8u kB; %5u MB; %2u GB\n", total, total / 1024, total / 1048576);
						f_puts(buffer, &USB_Fil);
						/* Free space */
						sprintf(buffer, "Free:  %8u kB; %5u MB; %2u GB\n", free, free / 1024, free / 1048576);
						f_puts(buffer, &USB_Fil);
						f_puts("----------------------------------------------------\n", &USB_Fil);
						/* Close USB file */
						f_close(&USB_Fil);

						/* Turn GREEN LED On and RED LED Off */
						/* Indicate successful write */
						TM_DISCO_LedOn(LED_GREEN);
						TM_DISCO_LedOff(LED_RED);
					}
				}
				/* Unmount USB */
				f_mount(0, "1:", 1);
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
				Delayms(60);
			}
			/* Leds off */
			TM_DISCO_LedOff(LED_GREEN | LED_RED);
		}
	}
}
