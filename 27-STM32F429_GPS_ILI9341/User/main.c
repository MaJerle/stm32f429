/**
 *	Keil project for GPS example on STM32F429-Discovery board
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
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_disco.h"

#include <stdio.h>

/* LCD Start Y address */
#define START_Y 		5

int main(void) {
	/* Variables used */
	TM_GPS_Data_t GPS_Data;
	TM_GPS_Result_t result, current;
	TM_GPS_Float_t GPS_Float;
	TM_GPS_Distance_t GPS_Distance;
	char buffer[40];
	uint8_t i;
	float temp;
	uint8_t iOff;
	
	/* Initialize system */
	SystemInit();
	
	/* Delay init */
	TM_DELAY_Init();
	
	/* Initialize leds */
	TM_DISCO_LedInit();
	
	/* Initialize GPS on 115200 baudrate */
	TM_GPS_Init(&GPS_Data, 115200);
	
	/* Initialize ili9341 LCD on STM32F429-Discovery board */
	TM_ILI9341_Init();
	/* Rotate LCD */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Go to the begginning of LCD */
	iOff = 0;
	
	/* Version 1.1 added */
	/* Set two test coordinates */
	/* from Ljubljana */
	GPS_Distance.Latitude1 = 46.050513;
	GPS_Distance.Longitude1 = 14.512873;
	/* to New York */
	GPS_Distance.Latitude2 = 40.711096;
	GPS_Distance.Longitude2 = -74.007529;
	
	/* Display location */
	TM_ILI9341_Puts(10, START_Y + 11 * iOff++, "Ljubljana -> New York", &TM_Font_7x10, 0x0000, 0xFFFF);
	
	/* Calculate distance and bearing between 2 pointes */
	TM_GPS_DistanceBetween(&GPS_Distance);
	
	/* Convert float number */
	TM_GPS_ConvertFloat(GPS_Distance.Distance, &GPS_Float, 1);
	sprintf(buffer, " - Distance: %d.%1d meters", GPS_Float.Integer, GPS_Float.Decimal);
	TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
	TM_GPS_ConvertFloat(GPS_Distance.Bearing, &GPS_Float, 3);
	sprintf(buffer, " - Bearing: %d.%03d degrees", GPS_Float.Integer, GPS_Float.Decimal);
	TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
	
	/* Delay 5s */
	Delayms(5000);
	
	/* Clear screen */
	TM_ILI9341_Fill(0xFFFF);
	
	/* Go to the begginning of LCD */
	iOff = 0;
	
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
			/* Display data on LCD */
			TM_ILI9341_Puts(10, START_Y + 11 * iOff++, "Check your GPS receiver!", &TM_Font_7x10, 0x0000, 0xFFFF);
		}
		/* If we have any unread data */
		if (result == TM_GPS_Result_NewData) {
			/* We received new packet of useful data from GPS */
			current = TM_GPS_Result_NewData;
			
			/* Go to the begginning of LCD */
			iOff = 0;
			
			/* Is GPS signal valid? */
			if (GPS_Data.Validity) {
				/* If you want to make a GPS tracker, now is the time to save your data on SD card */
				
				/* Toggle GREEN LED */
				TM_DISCO_LedToggle(LED_GREEN);
				
				/* We have valid GPS signal */
#ifndef GPS_DISABLE_GPGGA		
				/* Latitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Latitude, &GPS_Float, 6);
				sprintf(buffer, " - Latitude: %d.%d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Longitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Longitude, &GPS_Float, 6);
				sprintf(buffer, " - Longitude: %d.%d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Satellites in use */
				sprintf(buffer, " - Sats in use: %02d", GPS_Data.Satellites);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);	
				
				/* Current time */
				sprintf(buffer, " - UTC Time: %02d.%02d.%02d:%02d", GPS_Data.Time.Hours, GPS_Data.Time.Minutes, GPS_Data.Time.Seconds, GPS_Data.Time.Hundredths);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Fix: 0 = invalid, 1 = GPS, 2 = DGPS */
				sprintf(buffer, " - Fix: %d", GPS_Data.Fix);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);				
				
				/* Altitude */
				/* Convert float to integer and decimal part, with 6 decimal places */
				TM_GPS_ConvertFloat(GPS_Data.Altitude, &GPS_Float, 6);
				sprintf(buffer, " - Altitude: %3d.%06d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);				
#endif

#ifndef GPS_DISABLE_GPRMC
				/* Current date */
				sprintf(buffer, " - Date: %02d.%02d.%04d", GPS_Data.Date.Date, GPS_Data.Date.Month, GPS_Data.Date.Year + 2000);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Current speed in knots */
				TM_GPS_ConvertFloat(GPS_Data.Speed, &GPS_Float, 6);
				sprintf(buffer, " - Speed in knots: %2d.%06d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Current speed in km/h */
				temp = TM_GPS_ConvertSpeed(GPS_Data.Speed, TM_GPS_Speed_KilometerPerHour);
				TM_GPS_ConvertFloat(temp, &GPS_Float, 6);
				sprintf(buffer, " - Speed in km/h: %2d.%06d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				TM_GPS_ConvertFloat(GPS_Data.Direction, &GPS_Float, 3);
				sprintf(buffer, " - Direction: %3d.%03d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
#endif

#ifndef GPS_DISABLE_GPGSA				
				/* Horizontal dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.HDOP, &GPS_Float, 2);
				sprintf(buffer, " - HDOP: %2d.%02d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Vertical dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.VDOP, &GPS_Float, 2);
				sprintf(buffer, " - VDOP: %2d.%02d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Position dilution of precision */ 
				TM_GPS_ConvertFloat(GPS_Data.PDOP, &GPS_Float, 2);
				sprintf(buffer, " - PDOP: %2d.%02d", GPS_Float.Integer, GPS_Float.Decimal);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);	
				
				/* Current fix mode in use */ 
				sprintf(buffer, " - Fix mode: %d", GPS_Data.FixMode);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
				
				/* Display IDs of satellites in use */
				sprintf(buffer, "Sats ids: ");
				for (i = 0; i < GPS_Data.Satellites; i++) {
					if (i < (GPS_Data.Satellites - 1)) {
						sprintf(buffer, "%s%d,", buffer, GPS_Data.SatelliteIDs[i]);
					} else {
						sprintf(buffer, "%s%d", buffer, GPS_Data.SatelliteIDs[i]);
					}	
				}
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);		
#endif
				
#ifndef GPS_DISABLE_GPGSV
				/* Satellites in view */
				sprintf(buffer, " - Satellites in view: %02d", GPS_Data.SatellitesInView);
				TM_ILI9341_Puts(10, START_Y + 11 * iOff++, buffer, &TM_Font_7x10, 0x0000, 0xFFFF);
#endif
			} else {
				/* Clear screen */
				if (iOff > 0) {
					iOff = 0;
					TM_ILI9341_Fill(0xFFFF);
				}
				
				/* Go to the beginning of LCD */
				iOff = 0;
				
				/* Toggle RED LED */
				TM_DISCO_LedToggle(LED_RED);
				
				/* GPS signal is not valid */
				TM_ILI9341_Puts(10, START_Y + 11 * iOff, "New received data haven't valid GPS signal!", &TM_Font_7x10, 0x0000, 0xFFFF);
			}
		} else if (result == TM_GPS_Result_FirstDataWaiting && current != TM_GPS_Result_FirstDataWaiting) {
			current = TM_GPS_Result_FirstDataWaiting;
			TM_ILI9341_Puts(10, START_Y + 11 * iOff++, "Waiting first data from GPS!", &TM_Font_7x10, 0x0000, 0xFFFF);
		} else if (result == TM_GPS_Result_OldData && current != TM_GPS_Result_OldData) {
			current = TM_GPS_Result_OldData;
			/* We already read data, nothing new was received from GPS */
		}
	}
}

