/**
 *	Keil project for USB MSC HOST
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

FATFS USB_Fs;
FIL USB_Fil;
FRESULT fres;
	
int main(void) {
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
				if ((fres = f_mount(&USB_Fs, "USB:", 1)) == FR_OK) {
					TM_DISCO_LedOn(LED_GREEN);
					/* Mounted ok */
					/* Try to open USB file */
					if ((fres = f_open(&USB_Fil, "USB:usb_file.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS)) == FR_OK) {
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
				f_mount(0, "USB:", 1);
			}
		} else {
			/* Not inserted, turn on RED led */
			TM_DISCO_LedOn(LED_RED);
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Ready to write next time */
			write = 1;
		}
	}
}
