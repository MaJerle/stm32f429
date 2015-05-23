/**
 *	Keil project for STRINGS library
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.4.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_string.h"

#include "stdio.h"

/* String structure */
TM_STRING_t* String;

int main(void) {
	uint16_t i = 0;
	uint16_t strposition;
	
	/* Initialize system */
	SystemInit();
	
	/* Init USART2, TX: PA2, RX: PA3, 921600 bauds */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	/* Create string with 10 predefined locations for strings */
	String = TM_STRING_Create(10);
	
	/* Add string to memory, allocated memory will be set depending on string length */
	TM_STRING_AddString(String, "First string");
	
	/* Add another string to memory, allocated memory will be set depending on string length */
	TM_STRING_AddString(String, "Second string");
	
	/* Send strings over USART */
	for (i = 0; i < String->Count; i++) {
		/* Print string to user */
		printf("%s\n", String->Strings[i]);
	}
	
	/* Add some strings */
	TM_STRING_AddString(String, "Third string");
	strposition = TM_STRING_AddString(String, "Forth string");
	TM_STRING_AddString(String, "Fifth string");
	
	/* Modify string number 4 */
	TM_STRING_ReplaceString(String, strposition, "Updated string");
	
	/* Send strings over USART */
	for (i = 0; i < String->Count; i++) {
		/* Print string to user */
		printf("%s\n", String->Strings[i]);
	}
	
	/* Delete string on position 1 = "Second string" */
	TM_STRING_DeleteString(String, 1);
	
	/* Free entire string memory with all strings */
	TM_STRING_FreeAll(String);
	
	while (1) {
		
	}
}

/* Printf handler */
int fputc(int ch, FILE* fil) {
	/* Send over USART */
	TM_USART_Putc(USART2, ch);
	
	/* Return character */
	return ch;
}
