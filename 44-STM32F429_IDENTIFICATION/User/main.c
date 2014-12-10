/**
 *	Keil project for STM32F4 Identification
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
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_id.h"
#include <stdio.h>

int main(void) {
	char buff[200];
	
	/* Initialize system */
	SystemInit();

	/* Initialize USART2, TX: PA2, on Nucleo boards connected to USB (STLink) */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
	
	
	/* Format device signature */
	sprintf(buff, "Device signature: 0x%04X\r\n", TM_ID_GetSignature());
	/* Send to USART */
	TM_USART_Puts(USART2, buff);
	
	
	/* Format device signature */
	sprintf(buff, "Device revision: 0x%04X\r\n", TM_ID_GetRevision());
	/* Send to USART */
	TM_USART_Puts(USART2, buff);

	
	/* Format flash size */
	sprintf(buff, "Flash size: %u kB\r\n", TM_ID_GetFlashSize());
	/* Send to USART */
	TM_USART_Puts(USART2, buff);
	
	
	/* Format unique ID in 32-bit read mode */
	sprintf(buff, "Unique ID in 32-bit read mode: 0x%08X 0x%08X 0x%08X\r\n",
		TM_ID_GetUnique32(0),	/* LSB */
		TM_ID_GetUnique32(1),
		TM_ID_GetUnique32(2)	/* MSB */
	);
	/* Send to USART */
	TM_USART_Puts(USART2, buff);
	
	
	/* Format unique ID in 16-bit read mode */
	sprintf(buff, "Unique ID in 16-bit read mode: 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\r\n",
		TM_ID_GetUnique16(0),	/* LSB */
		TM_ID_GetUnique16(1),
		TM_ID_GetUnique16(2),
		TM_ID_GetUnique16(3),
		TM_ID_GetUnique16(4),
		TM_ID_GetUnique16(5)	/* MSB */
	);
	/* Send to USART */
	TM_USART_Puts(USART2, buff);
	
	
	/* Format unique ID in 8-bit read mode */
	sprintf(buff, "Unique ID in 8-bit read mode: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
		TM_ID_GetUnique8(0),	/* LSB */
		TM_ID_GetUnique8(1),
		TM_ID_GetUnique8(2),
		TM_ID_GetUnique8(3),
		TM_ID_GetUnique8(4),
		TM_ID_GetUnique8(5),
		TM_ID_GetUnique8(6),
		TM_ID_GetUnique8(7),
		TM_ID_GetUnique8(8),
		TM_ID_GetUnique8(9),
		TM_ID_GetUnique8(10),
		TM_ID_GetUnique8(11)	/* MSB */
	);
	/* Send to USART */
	TM_USART_Puts(USART2, buff);
	
	while (1) {

	}
}
