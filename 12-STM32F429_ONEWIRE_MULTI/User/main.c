/**
 *	Keil project for OneWire protocol library, 2 networks, PA3 and PD0 pins
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	Don't forget 4k7 pull up resistor!
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
	TM_OneWire_t OneWire1, OneWire2;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize OneWire1 instance on pin PA3 */
	TM_OneWire_Init(&OneWire1, GPIOA, GPIO_Pin_3);
	
	/* Initialize OneWire2 instance on pin PD0 */
	TM_OneWire_Init(&OneWire2, GPIOD, GPIO_Pin_0);
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Check PA3 OneWire1 instance */
	/* Check for any device on 1-wire port OneWire1 = PA3 */
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
	
	/* If any devices on 1-wire on pin PA3 */
	if (count > 0) {
		sprintf(buf, "Devices found on 1-wire instance 1 (PA3): %d\n", count);
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
		TM_USART_Puts(USART1, "No devices on OneWire instance 1.\n\n");
	}
	
	/* New line */
	TM_USART_Puts(USART1, "\n");
	
	/* Check PD0 OneWire2 instance */
	/* Check for any device on 1-wire port OneWire2 = PD0 */
	devices = TM_OneWire_First(&OneWire2);
	count = 0;
	while (devices) {
		/* Increase count variable */
		count++;
		
		/* Get full 8-bytes rom address */
		TM_OneWire_GetFullROM(&OneWire2, device[count - 1]);
		
		/* Check for new device */
		devices = TM_OneWire_Next(&OneWire2);
	}
	
	/* If any devices on 1-wire on pin PD0 */
	if (count > 0) {
		sprintf(buf, "Devices found on 1-wire instance 2 (PD0): %d\n", count);
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
		TM_USART_Puts(USART1, "No devices on OneWire instance 2.\n\n");
	}
	
	/* New line */
	TM_USART_Puts(USART1, "\n");
	
	while (1) {

	}
}

