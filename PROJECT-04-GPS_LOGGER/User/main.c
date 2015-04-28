/**
 *	Keil project for simple GPS logger with USB flash drive
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
 * How it works:
 *  - Program waits till USB key is inserted and be ready to use.
 *  - After that, it will save valid signal from GPS receiver to a usb file in CSV format
 * 
 * Leds status:
 *  - BLUE: If turned on, USB flash key is detected by USB stack
 *  - RED: If turned on, we received first data from GPS, so we know that GPS receiver works and has correct baudrate
 *  - ORANGE: If turned on, GPS signal is valid
 *  - GREEN: If turned on, USB flash key is mounted OK
 *
 * Button:
 *  - It works with 2 different modes:
 *    - LONG PRESS: File for GPS will be deleted from usb key and system will reset to start new measurements
 *    - SHORT PRESS: File will be closed, USB key unmounted and prepared to user to remove from discovery.
 *         All leds will blink when you are ready to remove USB flash key.
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_button.h"
#include "tm_stm32f4_gps.h"
#include "tm_stm32f4_general.h"
#include "tm_stm32f4_usb_msc_host.h"

/* FATFS objects */
FATFS fs;
FIL fil;
FRESULT fres;

/* GPS objects */
TM_GPS_t GPS;
TM_GPS_Distance_t GPS_Distance;

/* Buffer for USB write string */
/* For 10 GPS responses */
char USB_Buffer[5000];

/* Number of data packets in USB_Buffer array */
uint8_t PacketsInBuffer = 0;

/* Speed in kmph */
float SpeedKmph;

/* Home location status */
uint8_t HomeLocationIsSet = 0;

/* Button handler */
void ButtonHandler(TM_BUTTON_PressType_t type) {
	/* Check for long press */
	if (type == TM_BUTTON_PressType_Long) {
		/* Close file, delete file and restart */
		TM_DISCO_LedOn(LED_ORANGE);
		
		/* Close file */
		f_close(&fil);
		
		/* Delete file */
		f_unlink("1:gps_data.csv");
		
		/* Unmount drive */
		f_mount(NULL, "1:", 1);
		
		/* Delay */
		Delayms(500);
		
		/* System reset */
		TM_GENERAL_SystemReset();
	} else if (type == TM_BUTTON_PressType_Normal) {
		/* Stop measuring and do a while loop */
		
		/* Close file */
		f_close(&fil);
		
		/* Unmount device */
		f_mount(NULL, "1:", 1);
		
		/* Ready to remove usb key drive */
		
		/* While loop */
		while (1) {
			/* Toogle leds */
			TM_DISCO_LedToggle(LED_ALL);
			
			/* Delay */
			Delayms(100);
		}
	}
}

int main(void) {
	uint8_t i;
	char SatellitesIdsStr[100];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Init button, GPIOA, pin 0, 1 when pressed */
	TM_BUTTON_Init(GPIOA, GPIO_PIN_0, 1, ButtonHandler);
	
	/* Init GPS at 115200 baud, RX: PB7 */
	TM_GPS_Init(&GPS, 115200);
	
	/* Init USB MSC host stack */
	TM_USB_MSCHOST_Init();
	
	/* Wait till USB ready */
	while (TM_USB_MSCHOST_Device() != TM_USB_MSCHOST_Result_Connected) {
		/* USB MSC process */
		TM_USB_MSCHOST_Process();
	}
	
	/* USB detected */
	TM_DISCO_LedOn(LED_BLUE);
	
	/* Wait for USB key mounted */
	while (f_mount(&fs, "1:", 1) != FR_OK);

	/* Mounted OK */
	TM_DISCO_LedOn(LED_GREEN);
	
	/* Try to open file */
	fres = f_open(&fil, "1:gps_data.csv", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	
	/* Check if file empty */
	if (f_size(&fil) == 0) {
		/* Create header file for CSV */
		f_puts("Latitude;Longitude;Altitude;Direction;Speed;Speed kmh;Date;Time;SatellitesInView;SatellitesInUse;SatellitesIDs;Fix;Fix mode;HDOP;VDOP;PDOP;DistanceToHome;BearingToHome\n", &fil);
	}
	
	/* Go to the end of file for future writing */
	f_lseek(&fil, f_size(&fil));
	
	while (1) {
		/* USB MSC process */
		TM_USB_MSCHOST_Process();
		
		/* Update buttons */
		TM_BUTTON_Update();
		
		/* Update GPS */
		TM_GPS_Update(&GPS);
		
		/* Check status */
		if (GPS.Status != TM_GPS_Result_FirstDataWaiting) {
			/* Turn RED led, we have first GPS signal from module */
			TM_DISCO_LedOn(LED_RED);
		}
		
		/* New packet of data available */
		if (GPS.Status == TM_GPS_Result_NewData) {
			/* GPS signal is valid */
			if (GPS.Validity) {
				/* Led ON */
				TM_DISCO_LedOn(LED_ORANGE);
				
				/* Format data for saving */
				
				/* If home location is not set yet, do it now */
				if (!HomeLocationIsSet) {
					/* Location is set */
					HomeLocationIsSet = 1;
					
					/* Set home location */
					GPS_Distance.Latitude2 = GPS.Latitude;
					GPS_Distance.Longitude2 = GPS.Longitude;
				}
				
				/* Latitude, Longitude & Altitude */
				sprintf(USB_Buffer, "%s%7.5f;%8.5f;%8.5f;", USB_Buffer, GPS.Latitude, GPS.Longitude, GPS.Altitude);
				
				/* Convert speed to kmph */
				SpeedKmph = TM_GPS_ConvertSpeed(GPS.Speed, TM_GPS_Speed_KilometerPerHour);
			
				/* Direction, Speed and Speed km/h */
				sprintf(USB_Buffer, "%s%6.3f;%6.3f;%6.3f;", USB_Buffer, GPS.Direction, GPS.Speed, SpeedKmph);
			
				/* Date and time */
				sprintf(USB_Buffer, "%s%02d.%02d.%04d;%02d:%02d:%02d.%02d;", USB_Buffer,
					GPS.Date.Date, GPS.Date.Month, GPS.Date.Year + 2000,
					GPS.Time.Hours, GPS.Time.Minutes, GPS.Time.Seconds, GPS.Time.Hundredths
				);
				
				/* Satellites in view and use */
				sprintf(USB_Buffer, "%s%02d;%02d;", USB_Buffer, GPS.SatellitesInView, GPS.Satellites);
				
				/* Format all satellites in use */
				SatellitesIdsStr[0] = 0;
				for (i = 0; i < GPS.Satellites; i++) {
					if (i < (GPS.Satellites - 1)) {
						sprintf(SatellitesIdsStr, "%s%02d,", SatellitesIdsStr, GPS.SatelliteIDs[i]);
					} else {
						sprintf(SatellitesIdsStr, "%s%02d;", SatellitesIdsStr, GPS.SatelliteIDs[i]);
					}
				}
				
				/* Add to main buffer */
				/* Satellites in view and use */
				strcat(USB_Buffer, SatellitesIdsStr);
				
				/* Fix and fixmode */
				sprintf(USB_Buffer, "%s%d;%d;", USB_Buffer, GPS.Fix, GPS.FixMode);
				
				/* HDOP, VDOP, PDOP */
				sprintf(USB_Buffer, "%s%5.3f;%5.3f;%5.3f;", USB_Buffer, GPS.HDOP, GPS.VDOP, GPS.PDOP);
				
				/* Distance and bearing */
				/* Fill data */
				GPS_Distance.Latitude1 = GPS.Latitude;
				GPS_Distance.Longitude1 = GPS.Longitude;
				
				/* Calculate distance and bearing */
				TM_GPS_DistanceBetween(&GPS_Distance);
				
				/* Format */
				sprintf(USB_Buffer, "%s%10.3f;%6.3f;", USB_Buffer, GPS_Distance.Distance, GPS_Distance.Bearing);
				
				/* \n at the end */
				strcat(USB_Buffer, "\n");
			
				/* Increase number of packets in buffer */
				PacketsInBuffer++;
			} else {
				/* Led OFF */
				TM_DISCO_LedOff(LED_ORANGE);
			}
		}
		
		/* Check for saving to file */
		if (PacketsInBuffer >= 10) {
			/* Save data to file */
			f_write(&fil, USB_Buffer, strlen(USB_Buffer), NULL);
			
			/* Reset buffer */
			memset(USB_Buffer, 0, sizeof(USB_Buffer));
			
			/* Reset counter */
			PacketsInBuffer = 0;
		}
		
		/* Wait till USB ready */
		if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected) {
			/* Led BLUE */
			TM_DISCO_LedOn(LED_BLUE);
		} else {
			TM_DISCO_LedOff(LED_BLUE);
		}
	}
}
