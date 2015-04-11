/**
 *	Keil project for GPS
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 *	Make sure you have correct baudrate set for your GPS
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_gps.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"

#include <stdio.h>

/* Variables used */
TM_GPS_Data_t GPS_Data;
TM_GPS_Result_t result, current;
TM_GPS_Float_t GPS_Float;
TM_GPS_Distance_t GPS_Distance;
	
int main(void) {
	uint8_t i;
	float temp;
	
	/* Initialize system */
	SystemInit();
	
	/* Delay init */
	TM_DELAY_Init();
	
	/* Initialize GPS on 115200 baudrate */
	TM_GPS_Init(&GPS_Data, 115200);
	
	/* Initialize USART2 for debug */
	/* TX = PA2 */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
	
	/* Version 1.1 added */
	/* Set two test coordinates */
	GPS_Distance.Latitude1 = 48.300215;
	GPS_Distance.Longitude1 = -122.285903;
	GPS_Distance.Latitude2 = 45.907813;
	GPS_Distance.Longitude2 = 56.659407;
	
	/* Calculate distance and bearing between 2 points */
	TM_GPS_DistanceBetween(&GPS_Distance);
	
	/* Convert float number */
	TM_GPS_ConvertFloat(GPS_Distance.Distance, &GPS_Float, 6);
	printf("Distance is: %d.%06d meters\n", GPS_Float.Integer, GPS_Float.Decimal);
	
	TM_GPS_ConvertFloat(GPS_Distance.Bearing, &GPS_Float, 6);
	printf("Bearing is: %d.%06d degrees\n\n", GPS_Float.Integer, GPS_Float.Decimal);
	
	/* Reset counter */
	TM_DELAY_SetTime(0);
	while (1) {
		/* Update GPR data */
		/* Call this as faster as possible */
		result = TM_GPS_Update(&GPS_Data);

		/* If we have any unread data */
		if (result == TM_GPS_Result_NewData) {
			/* We received new packet of useful data from GPS */
			current = TM_GPS_Result_NewData;
			
			/* Is GPS signal valid? */
			if (GPS_Data.Validity) {
				/* If you want to make a GPS tracker, now is the time to save your data on SD card */
				
				/* We have valid GPS signal */
				printf("New received data have valid GPS signal\n");
				printf("---------------------------------------\n");
#ifndef GPS_DISABLE_GPGGA
				/* GPGGA data */
				printf("GPGGA statement:\n");
				
				/* Latitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Latitude, &GPS_Float, 6);
				printf(" - Latitude: %d.%d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				/* Longitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Longitude, &GPS_Float, 6);
				printf(" - Longitude: %d.%d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				/* Satellites in use */
				printf(" - Sats in use: %02d\n", GPS_Data.Satellites);
				
				/* Current time */
				printf(" - UTC Time: %02d.%02d.%02d:%02d\n", GPS_Data.Time.Hours, GPS_Data.Time.Minutes, GPS_Data.Time.Seconds, GPS_Data.Time.Hundredths);
				
				/* Fix: 0 = invalid, 1 = GPS, 2 = DGPS */
				printf(" - Fix: %d\n", GPS_Data.Fix);
				
				/* Altitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Altitude, &GPS_Float, 6);
				printf(" - Altitude: %3d.%06d\n", GPS_Float.Integer, GPS_Float.Decimal);
#endif
#ifndef GPS_DISABLE_GPRMC
				/* GPRMC data */
				printf("GPRMC statement:\n");
				
				/* Current date */
				printf(" - Date: %02d.%02d.%04d\n", GPS_Data.Date.Date, GPS_Data.Date.Month, GPS_Data.Date.Year + 2000);
				
				/* Current speed in knots */
				TM_GPS_ConvertFloat(GPS_Data.Speed, &GPS_Float, 6);
				printf(" - Speed in knots: %d.%06d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				/* Current speed in km/h */
				temp = TM_GPS_ConvertSpeed(GPS_Data.Speed, TM_GPS_Speed_KilometerPerHour);
				TM_GPS_ConvertFloat(temp, &GPS_Float, 6);
				printf(" - Speed in km/h: %d.%06d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				TM_GPS_ConvertFloat(GPS_Data.Direction, &GPS_Float, 3);
				printf(" - Direction: %3d.%03d\n", GPS_Float.Integer, GPS_Float.Decimal);
#endif
#ifndef GPS_DISABLE_GPGSA
				/* GPGSA data */
				printf("GPGSA statement:\n");
				
				/* Horizontal dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.HDOP, &GPS_Float, 2);
				printf(" - HDOP: %2d.%02d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				/* Vertical dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.VDOP, &GPS_Float, 2);
				printf(" - VDOP: %2d.%02d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				/* Position dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.PDOP, &GPS_Float, 2);
				printf(" - PDOP: %2d.%02d\n", GPS_Float.Integer, GPS_Float.Decimal);
				
				/* Current fix mode in use */ 
				printf(" - Fix mode: %d\n", GPS_Data.FixMode);
				
				/* Display IDs of satellites in use */
				printf(" - ID's of used satellites: ");
				for (i = 0; i < GPS_Data.Satellites; i++) {
					printf("%d,", GPS_Data.SatelliteIDs[i]);
				}
				printf("\n");
				
#endif
#ifndef GPS_DISABLE_GPGSV
				/* GPGSV data */
				printf("GPGSV statement:\n");
				
				/* Satellites in view */
				printf(" - Satellites in view: %d\n", GPS_Data.SatellitesInView);
				
				/* Print satellites description */
				for (i = 0; i < GPS_Data.SatellitesInView; i++) {
					printf(" - S: %02d, A: %03d, E: %02d, SNR: %02d\n", 
						GPS_Data.SatDesc[i].ID, 
						GPS_Data.SatDesc[i].Azimuth, 
						GPS_Data.SatDesc[i].Elevation, 
						GPS_Data.SatDesc[i].SNR
					);
				}
#endif				
				printf("---------------------------------------\n");
			} else {
				/* GPS signal is not valid */
				printf("New received data haven't valid GPS signal!\n");
			}
		} else if (result == TM_GPS_Result_FirstDataWaiting && current != TM_GPS_Result_FirstDataWaiting) {
			current = TM_GPS_Result_FirstDataWaiting;
			printf("Waiting first data from GPS!\n");
		} else if (result == TM_GPS_Result_OldData && current != TM_GPS_Result_OldData) {
			current = TM_GPS_Result_OldData;
			/* We already read data, nothing new was received from GPS */
		}
	}
}

/* printf handler */
int fputc(int ch, FILE* fil) {
	/* Send over USART */
	TM_USART_Putc(USART2, ch);
	
	/* Return character */
	return ch;
}

