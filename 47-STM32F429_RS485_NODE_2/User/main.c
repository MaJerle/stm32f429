/**
 *	Keil project example for RS-485 communication, node 2
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
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_rs485.h"

int main(void) {
	/* RS485 working struct */
	TM_RS485_t RS485;
	/* Dummy data bytes */
	uint8_t RS485_Data[5] = {0x15, 0x16, 0x78, 0xFF, 0x65};
	uint8_t buttonPressed = 0;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();

	/* Initialize button on board */
	TM_DISCO_ButtonInit();
	
	/* Initialize RS-485, node 2 */
	/* Our ID = 0x02, use 115200 baud for USART communication */
	TM_RS485_Init(&RS485, 0x02, 115200);
	
	while (1) {
		/* Check button */
		if (TM_DISCO_ButtonPressed() && !buttonPressed) {
			/* Button pressed */
			buttonPressed = 1;
			
			/* Send data to RS-485 */
			/* Send 5 bytes to RS-485, send to node with ID = 0x01 */
			TM_RS485_Send(&RS485, 0x01, RS485_Data, 5);
			
		} else if (!TM_DISCO_ButtonPressed() && buttonPressed) {
			/* Button released */
			buttonPressed = 0;
		}
		
		/* Check data from RS-485 network for us */
		if (TM_RS485_Receive(&RS485, RS485_Data)) {
			/* We have new data, stored in RS485_Data variable */
			/* 
			
			Data received from: RS485.ReceivedID
			Data received count: RS485.ReceivedCount
			
			*/
			
			/* Toggle green LED */
			TM_DISCO_LedToggle(LED_GREEN);
		}
	}
}
