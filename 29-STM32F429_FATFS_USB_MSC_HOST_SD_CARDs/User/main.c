/**
 *	Keil project for USB MSC HOST
 *	Data is saved into SD card via SDIO communication and into USB flash drive
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 * Select your target right of the "Load" button, compile and download to your discovery board
 * Works with F4-Discovery in USB FS mode and on F429-Discovery with USB HS in FS mode
 * Also works on all STM32F4xx devices with USB built in
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
	FATFS USB_Fs, SD_Fs;
	FIL USB_Fil, SD_Fil;
	char buffer[50];
	uint8_t usb_write = 1, sd_write = 1;
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
		/* USB Device is connected and ready to use */
		if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected) {
			TM_DISCO_LedOn(LED_GREEN);
			/* If we didn't write data already */
			if (usb_write) {
				/* Try to mount USB device */
				/* USB is at 1: */
				if (f_mount(&USB_Fs, "1:", 1) == FR_OK) {
					/* Mounted ok */
					/* Try to open USB file */
					if (f_open(&USB_Fil, "1:usb_file.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
						/* We want to write only once */
						usb_write = 0;
						
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

						/* Turn GREEN LED On */
						/* Indicate successful write for USB flash */
						TM_DISCO_LedOn(LED_GREEN);
					}
				}
				/* Unmount USB */
				f_mount(0, "1:", 1);
			}
		} else {
			/* Not inserted, turn off GREEN led */
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Ready to write next time */
			usb_write = 1;
		}
		/* Try to mount SD card */
		/* SD card is at 0: */
		if (f_mount(&SD_Fs, "0:", 1) == FR_OK) {
			/* Mounted ok */
			/* Write only once */
			if (sd_write) {
				/* Try to open SD card file */
				if (f_open(&SD_Fil, "0:sd_file.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
					/* We want to write only once */
					sd_write = 0;
					
					/* Get total and free space on SD card */
					TM_FATFS_DriveSize(&total, &free);
					
					/* Put data */
					f_puts("This is my first file with SD card and FatFs\n", &SD_Fil);
					f_puts("with SD card library from stm32f4-discovery.com\n", &SD_Fil);
					f_puts("----------------------------------------------------\n", &SD_Fil);
					f_puts("SD card total and free space:\n\n", &SD_Fil);
					/* Total space */
					sprintf(buffer, "Total: %8u kB; %5u MB; %2u GB\n", total, total / 1024, total / 1048576);
					f_puts(buffer, &SD_Fil);
					/* Free space */
					sprintf(buffer, "Free:  %8u kB; %5u MB; %2u GB\n", free, free / 1024, free / 1048576);
					f_puts(buffer, &SD_Fil);
					f_puts("----------------------------------------------------\n", &SD_Fil);
					
					/* Close SD card file */
					f_close(&SD_Fil);

					/* Turn RED LED On */
					/* Indicate successful write for SD card */
					TM_DISCO_LedOn(LED_RED);
				}
			}
			
			/* Unmount SD card */
			f_mount(0, "0:", 1);
		} else {
			/* Not inserted, turn off RED led */
			TM_DISCO_LedOff(LED_RED);
			
			/* Ready to write next time */
			sd_write = 1;
		}
	}
}
