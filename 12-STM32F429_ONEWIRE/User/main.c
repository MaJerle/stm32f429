/**
 *	Keil project for OneWire protocol library
 *
 *  Before you start, select your target, on the right of the "Load" button
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
#include <stdio.h>

int main(void) {
	char buf[30];
	uint8_t devices, i, j, count, device[2][8];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize One Wire on pin PD0 */
	TM_OneWire_Init();
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Checks for any device on 1-wire */
	devices = TM_OneWire_First();
	count = 0;
	while (devices) {
		count++;
		for (i = 0; i < 8; i++) {
			device[count - 1][i] = TM_OneWire_GetROM(i);
		}
		/* Check for new device */
		devices = TM_OneWire_Next();
	}
	/* If any devices on 1-wire */
	if (count > 0) {
		sprintf(buf, "Devices found on 1-wire: %d\n\r", count);
		TM_USART_Puts(USART1, buf);
		/* Display 64bit rom code */
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
	
	while (1) {

	}
}

