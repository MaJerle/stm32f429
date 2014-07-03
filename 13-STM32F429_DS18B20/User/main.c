/**
 *	DS18B20 library example
 *	
 *	This example first search for all devices on 1wire bus
 *	Set some parameters, described down in code
 *	Start conversion on all devices
 *	Read temperatures and display it
 *	Checks for devices with alarm flag set
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_ds18b20.h"
#include "tm_stm32f4_disco.h"
#include <stdio.h>

//How many sensors we are expecting on 1wire bus?
#define EXPECTING_SENSORS	2

int main(void) {
	char buf[40];
	uint8_t devices, i, j, count, alarm_count;
	uint8_t device[EXPECTING_SENSORS][8];
	uint8_t alarm_device[EXPECTING_SENSORS][8];
	float temps[EXPECTING_SENSORS];
	
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initialize OneWire on pin PD0
	TM_OneWire_Init();
	//Initialize USART, TX: PA9, RX: PA10
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	//Initialize Leds
	TM_DISCO_LedInit();
	//Turn leds on
	TM_DISCO_LedOn(LED_RED | LED_GREEN);
	
	//Checks for any device on 1-wire
	devices = TM_OneWire_First();
	count = 0;
	while (devices) {
		count++;
		//Get full ROM value, 8 bytes, give location of first byte where to save
		TM_OneWire_GetFullROM(&device[count - 1][0]);
		//Get next device
		devices = TM_OneWire_Next();
	}
	//If any devices on 1wire
	if (count > 0) {
		sprintf(buf, "Devices found on 1-wire: %d\n\r", count);
		TM_USART_Puts(USART1, buf);
		//Display 64bit rom code for each device
		for (j = 0; j < count; j++) {
			for (i = 0; i < 8; i++) {
				sprintf(buf, "0x%02X ", device[j][i]);
				TM_USART_Puts(USART1, buf);
			}
			TM_USART_Puts(USART1, "\n\r");
		}
	} else {
		TM_USART_Puts(USART1, "No devices on OneWire.\n\r");
	}
	
	//Go through all connected devices and set resolution to 12bits
	for (i = 0; i < count; i++) {
		TM_DS18B20_SetResolution(&device[i][0], TM_DS18B20_Resolution_12bits);
	}
	//Set high temperature alarm on device number 0, 25degrees celcius
	TM_DS18B20_SetAlarmHighTemperature(&device[0][0], 27);
	//Disable alarm temperatures on device number 1
	TM_DS18B20_DisableAlarmTemperature(&device[1][0]);
	
	while (1) {
		//Start temperature conversion on all bits
		TM_DS18B20_StartAll();
		//Wait until all are done
		while (!TM_DS18B20_AllDone());
		//Read temperature from each device separatelly
		for (i = 0; i < count; i++) {
			//Read temperature from ROM address and store it to temps variable
			TM_DS18B20_Read(&device[i][0], &temps[i]);
			//Print temperature
			sprintf(buf, "Temp %d: %3.5f; ", i, temps[i]);
			TM_USART_Puts(USART1, buf);
		}
		alarm_count = 0;
		//Check if any device has alarm flag set
		while (TM_DS18B20_AlarmSearch()) {
			//Store ROM of device which has alarm flag set
			TM_OneWire_GetFullROM(&alarm_device[alarm_count][0]);
			alarm_count++;
		}
		sprintf(buf, "alarm: %d\n\r", alarm_count);
		TM_USART_Puts(USART1, buf);
		//Any device has alarm flag set?
		if (alarm_count > 0) {
			TM_USART_Puts(USART1, "THIS DEVICES HAVE ALARM!\n\r    ");
			//Show rom of this devices
			for (j = 0; j < alarm_count; j++) {
				for (i = 0; i < 8; i++) {
					sprintf(buf, "0x%02X ", alarm_device[j][i]);
					TM_USART_Puts(USART1, buf);
				}
				TM_USART_Puts(USART1, "\n\r    ");
			}
			TM_USART_Puts(USART1, "ALARM devices recognized!\n\r");
		}
		//Some delay
		Delayms(1000);
	}
}
