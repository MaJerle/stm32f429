/**
 *	Keil project for GPS custom statements
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

TM_GPS_Custom_t *GPRMC, *GPGLL, *GPGSA;
	
int main(void) {
	uint8_t i;
	
	/* Initialize system */
	SystemInit();
	
	/* Delay init */
	TM_DELAY_Init();
	
	/* Initialize GPS on 115200 baudrate */
	TM_GPS_Init(&GPS_Data, 115200);
	
	/* Initialize USART2 for debug */
	/* TX = PA2 */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
	
	/* Register custom GPGxx statements */
	
	/* $GPRMC statement, term number 7 = Speed over ground in knots */
	GPRMC = TM_GPS_AddCustom(&GPS_Data, "$GPRMC", 7);
	/* $GPGLL statement, term number 1 = Current latitude */
	GPGLL = TM_GPS_AddCustom(&GPS_Data, "$GPGLL", 1);
	/* $GPGSA statement, term number 1 = M = Manual, forced to operate in 2D or 3D A=Automatic, 3D/2D */
	GPGSA = TM_GPS_AddCustom(&GPS_Data, "$GPGSA", 1);
	/* Add here more custom tags you want */
	/* ... */
	
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

				/* We have all data from GPS_Data structure valid, you can do here what you want */
				/* We will in this example show only custom data to user */
			
				/* Print custom statements */
				printf("Custom statements defined by user:\n");
				
				/* Go through all custom statements */
				for (i = 0; i < GPS_Data.CustomStatementsCount; i++) {
					printf(" - Statement: %s; TermNumber: %d; Value: %s\n",
						GPS_Data.CustomStatements[i]->Statement,  /*!< Statement value */
						GPS_Data.CustomStatements[i]->TermNumber, /*!< Term number */
						GPS_Data.CustomStatements[i]->Value       /*!< Value from GPS receiver */
					);
				}
				
				/* You can do it this way too for all your custom statements separatelly */
				printf(" - Statement2: %s; TermNumber: %d; Value: %s\n",
					GPRMC->Statement, GPRMC->TermNumber, GPRMC->Value
				);
				
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

