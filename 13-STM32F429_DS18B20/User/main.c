/**
 *	Keil project for DS18B20 library example
 *
 *	This example first search for all devices on 1wire bus
 *	Set some parameters, described down in code
 *	Start conversion on all devices
 *	Read temperatures and display it
 *	Checks for devices with alarm flag set
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_ds18b20.h"
#include "tm_stm32f4_disco.h"
#include <stdio.h>

/* How many sensors we are expecting on 1wire bus? */
#define EXPECTING_SENSORS	2

int main(void) {
	char buf[40];
	uint8_t devices, i, j, count, alarm_count;
	uint8_t device[EXPECTING_SENSORS][8];
	uint8_t alarm_device[EXPECTING_SENSORS][8];
	float temps[EXPECTING_SENSORS];
	
	/* OneWire working struct */
	TM_OneWire_t OneWire1;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize OneWire on pin PD0 */
	TM_OneWire_Init(&OneWire1, GPIOD, GPIO_Pin_0);
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize Leds */
	TM_DISCO_LedInit();
	
	/* Turn leds on */
	TM_DISCO_LedOn(LED_ALL);
	
	/* Checks for any device on 1-wire */
	count = 0;
	devices = TM_OneWire_First(&OneWire1);
	while (devices) {
		/* Increase counter */
		count++;
		
		/* Get full ROM value, 8 bytes, give location of first byte where to save */
		TM_OneWire_GetFullROM(&OneWire1, device[count - 1]);
		
		/* Get next device */
		devices = TM_OneWire_Next(&OneWire1);
	}
	
	/* If any devices on 1wire */
	if (count > 0) {
		sprintf(buf, "Devices found on 1-wire: %d\n", count);
		TM_USART_Puts(USART1, buf);
		/* Display 64bit rom code for each device */
		for (j = 0; j < count; j++) {
			for (i = 0; i < 8; i++) {
				sprintf(buf, "0x%02X ", device[j][i]);
				TM_USART_Puts(USART1, buf);
			}
			TM_USART_Puts(USART1, "\n");
		}
	} else {
		TM_USART_Puts(USART1, "No devices on OneWire.\n");
	}
	
	/* Go through all connected devices and set resolution to 12bits */
	for (i = 0; i < count; i++) {
		/* Set resolution to 12bits */
		TM_DS18B20_SetResolution(&OneWire1, device[i], TM_DS18B20_Resolution_12bits);
	}
	
	/* Set high temperature alarm on device number 0, 25 degrees celcius */
	TM_DS18B20_SetAlarmHighTemperature(&OneWire1, device[0], 25);
	
	/* Disable alarm temperatures on device number 1 */
	TM_DS18B20_DisableAlarmTemperature(&OneWire1, device[1]);
	
	while (1) {
		/* Start temperature conversion on all devices on one bus */
		TM_DS18B20_StartAll(&OneWire1);
		
		/* Wait until all are done on one onewire port */
		while (!TM_DS18B20_AllDone(&OneWire1));
		
		/* Read temperature from each device separatelly */
		for (i = 0; i < count; i++) {
			/* Read temperature from ROM address and store it to temps variable */
			if (TM_DS18B20_Read(&OneWire1, device[i], &temps[i])) {
				/* Print temperature */
				sprintf(buf, "Temp %d: %3.5f; \n", i, temps[i]);
				TM_USART_Puts(USART1, buf);
			} else {
				/* Reading error */
				TM_USART_Puts(USART1, "Reading error;\n");
			}
		}
		
		/* Reset alarm count */
		alarm_count = 0;
		
		/* Check if any device has alarm flag set */
		while (TM_DS18B20_AlarmSearch(&OneWire1)) {
			/* Store ROM of device which has alarm flag set */
			TM_OneWire_GetFullROM(&OneWire1, alarm_device[alarm_count]);
			/* Increase count */
			alarm_count++;
		}
		
		/* Format string and send over USART for debug */
		sprintf(buf, "Devices with alarm: %d\n", alarm_count);
		TM_USART_Puts(USART1, buf);
		
		/* Any device has alarm flag set? */
		if (alarm_count > 0) {
			/* Show rom of this devices */
			for (j = 0; j < alarm_count; j++) {
				TM_USART_Puts(USART1, "Device with alarm: ");
				for (i = 0; i < 8; i++) {
					sprintf(buf, "0x%02X ", alarm_device[j][i]);
					TM_USART_Puts(USART1, buf);
				}
				TM_USART_Puts(USART1, "\n    ");
			}
			TM_USART_Puts(USART1, "ALARM devices recognized!\n\r");
		}
		
		/* Print separator */
		TM_USART_Puts(USART1, "----------\n");
		
		/* Some delay */
		Delayms(1000);
	}
}

