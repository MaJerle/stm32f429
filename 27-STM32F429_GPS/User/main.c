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

#include <stdio.h>

int main(void) {
	/* Variables used */
	TM_GPS_Data_t GPS_Data;
	TM_GPS_Result_t result, current;
	TM_GPS_Float_t GPS_Float;
	TM_GPS_Distance_t GPS_Distance;
	char buffer[40];
	uint8_t i;
	float temp;
	
	/* Initialize system */
	SystemInit();
	
	/* Delay init */
	TM_DELAY_Init();
	
	/* Initialize GPS on 115200 baudrate */
	TM_GPS_Init(&GPS_Data, 115200);
	
	/* Initialize USART3 for debug */
	/* TX = PB10 */
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 115200);
	
	/* Version 1.1 added */
	/* Set two test coordinates */
	GPS_Distance.Latitude1 = 48.300215;
	GPS_Distance.Longitude1 = -122.285903;
	GPS_Distance.Latitude2 = 45.907813;
	GPS_Distance.Longitude2 = 56.659407;
	
	/* Calculate distance and bearing between 2 pointes */
	TM_GPS_DistanceBetween(&GPS_Distance);
	/* Convert float number */
	TM_GPS_ConvertFloat(GPS_Distance.Distance, &GPS_Float, 6);
	sprintf(buffer, "Distance is: %d.%06d meters\n", GPS_Float.Integer, GPS_Float.Decimal);
	TM_USART_Puts(USART3, buffer);
	TM_GPS_ConvertFloat(GPS_Distance.Bearing, &GPS_Float, 6);
	sprintf(buffer, "Bearing is: %d.%06d degrees\n\n", GPS_Float.Integer, GPS_Float.Decimal);
	TM_USART_Puts(USART3, buffer);
	
	/* Reset counter */
	TM_DELAY_SetTime(0);
	while (1) {
		/* Update GPR data */
		/* Call this as faster as possible */
		result = TM_GPS_Update(&GPS_Data);
		/* If we didn't receive any useful data in the start */
		if (result == TM_GPS_Result_FirstDataWaiting && TM_DELAY_Time() > 3000) {
			/* If we didn't receive nothing within 3 seconds */
			TM_DELAY_SetTime(0);
			/* Display data on USART */
			TM_USART_Puts(USART3, "\nNothing received after 3 seconds. Is your GPS connected and baudrate set correct?\n");
			TM_USART_Puts(USART3, "Most GPS receivers has by default 9600 baudrate and 1Hz refresh rate. Check your settings!\n\n");
		}
		/* If we have any unread data */
		if (result == TM_GPS_Result_NewData) {
			/* We received new packet of useful data from GPS */
			current = TM_GPS_Result_NewData;
			
			/* Is GPS signal valid? */
			if (GPS_Data.Validity) {
				/* If you want to make a GPS tracker, now is the time to save your data on SD card */
				
				/* We have valid GPS signal */
				TM_USART_Puts(USART3, "New received data have valid GPS signal\n");
				TM_USART_Puts(USART3, "---------------------------------------\n");
#ifndef GPS_DISABLE_GPGGA
				/* GPGGA data */
				TM_USART_Puts(USART3, "GPGGA statement:\n");
				
				/* Latitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Latitude, &GPS_Float, 6);
				sprintf(buffer, " - Latitude: %d.%d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
				
				/* Longitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Longitude, &GPS_Float, 6);
				sprintf(buffer, " - Longitude: %d.%d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
				
				/* Satellites in use */
				sprintf(buffer, " - Sats in use: %02d\n", GPS_Data.Satellites);
				TM_USART_Puts(USART3, buffer);	
				
				/* Current time */
				sprintf(buffer, " - UTC Time: %02d.%02d.%02d:%02d\n", GPS_Data.Time.Hours, GPS_Data.Time.Minutes, GPS_Data.Time.Seconds, GPS_Data.Time.Hundredths);
				TM_USART_Puts(USART3, buffer);
				
				/* Fix: 0 = invalid, 1 = GPS, 2 = DGPS */
				sprintf(buffer, " - Fix: %d\n", GPS_Data.Fix);
				TM_USART_Puts(USART3, buffer);				
				
				/* Altitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Altitude, &GPS_Float, 6);
				sprintf(buffer, " - Altitude: %3d.%06d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);				
#endif
#ifndef GPS_DISABLE_GPRMC
				/* GPRMC data */
				TM_USART_Puts(USART3, "GPRMC statement:\n");
				
				/* Current date */
				sprintf(buffer, " - Date: %02d.%02d.%04d\n", GPS_Data.Date.Date, GPS_Data.Date.Month, GPS_Data.Date.Year + 2000);
				TM_USART_Puts(USART3, buffer);
				
				/* Current speed in knots */
				TM_GPS_ConvertFloat(GPS_Data.Speed, &GPS_Float, 6);
				sprintf(buffer, " - Speed in knots: %d.%06d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
				
				/* Current speed in km/h */
				temp = TM_GPS_ConvertSpeed(GPS_Data.Speed, TM_GPS_Speed_KilometerPerHour);
				TM_GPS_ConvertFloat(temp, &GPS_Float, 6);
				sprintf(buffer, " - Speed in km/h: %d.%06d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
				
				TM_GPS_ConvertFloat(GPS_Data.Direction, &GPS_Float, 3);
				sprintf(buffer, " - Direction: %3d.%03d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
#endif
#ifndef GPS_DISABLE_GPGSA
				/* GPGSA data */
				TM_USART_Puts(USART3, "GPGSA statement:\n");
				
				/* Horizontal dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.HDOP, &GPS_Float, 2);
				sprintf(buffer, " - HDOP: %2d.%02d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
				
				/* Vertical dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.VDOP, &GPS_Float, 2);
				sprintf(buffer, " - VDOP: %2d.%02d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);
				
				/* Position dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.PDOP, &GPS_Float, 2);
				sprintf(buffer, " - PDOP: %2d.%02d\n", GPS_Float.Integer, GPS_Float.Decimal);
				TM_USART_Puts(USART3, buffer);	
				
				/* Current fix mode in use */ 
				sprintf(buffer, " - Fix mode: %d\n", GPS_Data.FixMode);
				TM_USART_Puts(USART3, buffer);
				
				/* Display IDs of satellites in use */
				TM_USART_Puts(USART3, "- ID's of used satellites: ");
				for (i = 0; i < GPS_Data.Satellites; i++) {
					if (i < (GPS_Data.Satellites - 1)) {
						sprintf(buffer, "%d,", GPS_Data.SatelliteIDs[i]);
					} else {
						sprintf(buffer, "%d\n", GPS_Data.SatelliteIDs[i]);
					}
					TM_USART_Puts(USART3, buffer);
				}
				
#endif
#ifndef GPS_DISABLE_GPGSV
				/* GPGSV data */
				TM_USART_Puts(USART3, "GPGSV statement:\n");
				
				/* Satellites in view */
				sprintf(buffer, " - Satellites in view: %d\n", GPS_Data.SatellitesInView);
				TM_USART_Puts(USART3, buffer);	
#endif
				TM_USART_Puts(USART3, "---------------------------------------\n");
			} else {
				/* GPS signal is not valid */
				TM_USART_Puts(USART3, "New received data haven't valid GPS signal!\n");
			}
		} else if (result == TM_GPS_Result_FirstDataWaiting && current != TM_GPS_Result_FirstDataWaiting) {
			current = TM_GPS_Result_FirstDataWaiting;
			TM_USART_Puts(USART3, "Waiting first data from GPS!\n");
		} else if (result == TM_GPS_Result_OldData && current != TM_GPS_Result_OldData) {
			current = TM_GPS_Result_OldData;
			/* We already read data, nothing new was received from GPS */
		}
	}
}

