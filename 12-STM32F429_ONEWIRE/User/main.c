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
	char buf[100];
	uint8_t devices, i, j, count, device[2][8];
	/* OneWire working structure */
	TM_OneWire_t OneWire1;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize OneWire1 instance on pin PD0 */
	TM_OneWire_Init(&OneWire1, GPIOD, GPIO_Pin_0);
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Check for any device on 1-wire */
	devices = TM_OneWire_First(&OneWire1);
	count = 0;
	while (devices) {
		/* Increase count variable */
		count++;
		
		/* Get full 8-bytes rom address */
		TM_OneWire_GetFullROM(&OneWire1, device[count - 1]);
		
		/* Check for new device */
		devices = TM_OneWire_Next(&OneWire1);
	}
	
	/* If any devices on 1-wire */
	if (count > 0) {
		sprintf(buf, "Devices found on 1-wire instance: %d\n", count);
		TM_USART_Puts(USART1, buf);
		
		/* Display 64bit rom code */
		for (j = 0; j < count; j++) {
			for (i = 0; i < 8; i++) {
				sprintf(buf, "0x%02X ", device[j][i]);
				TM_USART_Puts(USART1, buf);
			}
			TM_USART_Puts(USART1, "\n");
		}
	} else {
		/* Nothing on OneWire */
		TM_USART_Puts(USART1, "No devices on OneWire.\n\n");
	}
	
	while (1) {

	}
}

