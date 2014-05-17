/**
 *	Onewire library example
 *	
 *	Find all devices on 1-wire bus and show their 8bytes rom code
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
#include <stdio.h>

int main(void) {
	char buf[30];
	uint8_t devices, i, j, count, device[2][8];
	float temp;
	uint8_t converted;
	
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initialize OneWire on pin PD0
	TM_OneWire_Init();
	//Initialize USART, TX: PA9, RX: PA10
	TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);
	
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
		//Display 64bit rom code
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
	
	//Set DS18B20 resolution to 12bits, last device read 
	TM_DS18B20_SetResolution(&device[count - 1][0], TM_DS18B20_Resolution_12bit);
	while (1) {
		//Start temperature conversion
		TM_DS18B20_Start(&device[count - 1][0]);
		//It tooks about 750ms to convert temperature at 12bits
		Delayms(750);
		//Check if temperature is converted
		converted = TM_DS18B20_Read(&device[count - 1][0], &temp);
		if (converted) {
			sprintf(buf, "Temp is: %3.5f\n\r", temp);
			TM_USART_Puts(USART1, buf);
		}
	}
}
