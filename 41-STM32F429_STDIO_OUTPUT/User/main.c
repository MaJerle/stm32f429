/**
 *	Keil project for STDIO output
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
#include "tm_stm32f4_stdio.h"

/* You can use only 1 thing for printf */
/* If you want more, then you can use fprintf */
/* For this you need FILE pointer for each */
/* Create global variables for fprintf function */
/* for USART2 and USART6 */
FILE USART2_Stream, USART6_Stream;

/* Output stream for USART2 and USART3 function references */
int USART2_Stream_OutputFunction(int ch, FILE* f);
int USART6_Stream_OutputFunction(int ch, FILE* f);

int main(void) {
	/* Initialize system */
	SystemInit();

	/* Initialize USART1, TX = PB6, 115200 bauds */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize USART2, TX = PD5, 115200 bauds*/
	TM_USART_Init(USART2, TM_USART_PinsPack_2, 115200);
	
	/* Initialize USART6, TX = PC6, 115200 bauds */
	TM_USART_Init(USART6, TM_USART_PinsPack_1, 115200);
	
	/* Add output function for USART2_Stream */
	TM_STDIO_SetOutputFunction(&USART2_Stream, USART2_Stream_OutputFunction);
	
	/* Add output function for USART6_Stream */
	TM_STDIO_SetOutputFunction(&USART6_Stream, USART6_Stream_OutputFunction);
	
	/* Print something on USART1 */
	printf("Hello USART1 user\n");
	
	/* Print something on USART2 */
	fprintf(&USART2_Stream, "Hello USART2 user\n");
	
	/* Print something on USART6 */
	fprintf(&USART6_Stream, "Hello USART6 user\n");
	
	while (1) {
		
	}
}

/* Handle stdout actions */
int TM_STDIO_StdoutHandler(int ch, FILE* f) {
	/* Send data to USART1 */
	TM_USART_Putc(USART1, (char) ch);
	
	/* Return ch, it means OK */
	return ch;
	/* If you want to return error, then you have to send EOF (-1) */
	//return -1;
}

/* USART2_Stream function that will send char by char to stream */
int USART2_Stream_OutputFunction(int ch, FILE* f) {
	/* Send char via USART2 */
	TM_USART_Putc(USART2, (char) ch);
	
	/* Return ch, it means OK */
	return ch;
	/* If you want to return error, then you have to send EOF (-1) */
	//return -1;
}

int USART6_Stream_OutputFunction(int ch, FILE* f) {
	/* Send char via USART6 */
	TM_USART_Putc(USART6, (char) ch);
	
	/* Return ch, it means OK */
	return ch;
	/* If you want to return error, then you have to send EOF (-1) */
	//return -1;
}
