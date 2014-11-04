/**
 *	Keil project for STDIO output & input
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
#include "tm_stm32f4_delay.h"

/* You can use only 1 thing for printf */
/* If you want more, then you can use fprintf */
/* For this you need FILE pointer for each */
/* Create global variables for fprintf function */
/* for USART2 and USART6 */
FILE USART2_Stream, USART6_Stream;

/* Output stream for USART2 and USART6 function references */
int USART2_Stream_OutputFunction(int ch, FILE* f);
int USART6_Stream_OutputFunction(int ch, FILE* f);

/* Input stream for USART2 and USART6 function references */
int USART2_Stream_InputFunction(FILE* f);
int USART6_Stream_InputFunction(FILE* f);

int main(void) {
	char str[100];
	
	/* Initialize system */
	SystemInit();

	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize USART1, TX = PB6, RX = PB7, 115200 bauds */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize USART2, TX = PD5, RX = PD6, 115200 bauds*/
	TM_USART_Init(USART2, TM_USART_PinsPack_2, 115200);
	
	/* Initialize USART6, TX = PC6, RX = PC7, 115200 bauds */
	TM_USART_Init(USART6, TM_USART_PinsPack_1, 115200);
	
	/* Add output function for USART2_Stream */
	TM_STDIO_SetOutputFunction(&USART2_Stream, USART2_Stream_OutputFunction);
	
	/* Add input function for USART2_Stream */
	TM_STDIO_SetInputFunction(&USART2_Stream, USART2_Stream_InputFunction);
	
	/* Add output function for USART6_Stream */
	TM_STDIO_SetOutputFunction(&USART6_Stream, USART6_Stream_OutputFunction);
	
	/* Add input function for USART6_Stream */
	TM_STDIO_SetInputFunction(&USART6_Stream, USART6_Stream_InputFunction);
	
	/* Print something on USART1 */
	printf("Hello USART1 user\n");
	
	/* Print something on USART2 */
	fprintf(&USART2_Stream, "Hello USART2 user\n");
	
	/* Print something on USART6 */
	fprintf(&USART6_Stream, "Hello USART6 user\n");
	
	while (1) {
		/* If data are available on stream */
		
		/* Check data on standard header, USART1 */
		/* stdin or standard input */
		if (fgets(str, 100, stdin) != NULL) {
			/* Send data back */
			
			/* Statement below is the same as of use printf function */
			fprintf(stdout, (char *)str);
		}
		
		/* Check data on USART2 stream */
		if (fgets(str, 100, &USART2_Stream) != NULL) {
			/* Send data back */
			
			/* Send data to USART2 stream */
			fprintf(&USART2_Stream, (char *)str);
		}
		
		/* Check data on USART6 stream */
		if (fgets(str, 100, &USART6_Stream) != NULL) {
			/* Send data back */
			
			/* Send data to USART2 stream */
			fprintf(&USART6_Stream, (char *)str);
		}
		
		/* Some delay */
		/* If you call your gets too often, then it can happen that USART characters are not in buffer yet */
		/* and function will not do as it should */
		Delayms(1000);
	}
}



/* Handle stdout actions = default name and can not be changed */
int TM_STDIO_StdoutHandler(int ch, FILE* f) {
	/* Send data to USART1 */
	TM_USART_Putc(USART1, (char) ch);
	
	/* Return ch, it means OK */
	return ch;
	/* If you want to return error, then you have to send EOF (-1) */
	//return -1;
}

/* Handle stdin actions */
int TM_STDIO_StdinHandler(FILE* f) {
	/* If any data at USART, return them */
	/* Do your custom implementation here when string ends */
	if (!TM_USART_BufferEmpty(USART1)) {
		return (int)TM_USART_Getc(USART1);
	}

	/* End of data, string is valid */
	/* You have to send -1 at the end of string */
	return -1;
}

/* USART2 output stream handler = custom function name, linked with USART2 stream in the beginning of main() function */
int USART2_Stream_OutputFunction(int ch, FILE* f) {
	/* Send char via USART2 */
	TM_USART_Putc(USART2, (char) ch);
	
	/* Return ch, it means OK */
	return ch;
	/* If you want to return error, then you have to send EOF (-1) */
	//return -1;
}

/* Handle USART2 stream input = custom function name, linked with USART2 stream in the beginning of main() function */
int USART2_Stream_InputFunction(FILE* f) {
	/* If any data at USART, return them */
	/* Do your custom implementation here when string ends */
	if (!TM_USART_BufferEmpty(USART2)) {
		return (int)TM_USART_Getc(USART2);
	}
	
	/* End of data, string is valid */
	/* You have to send -1 at the end of string */
	return -1;
}


/* USART6 output stream handler = custom function name, linked with USART6 stream in the beginning of main() function */
int USART6_Stream_OutputFunction(int ch, FILE* f) {
	/* Send char via USART6 */
	TM_USART_Putc(USART6, (char) ch);
	
	/* Return ch, it means OK */
	return ch;
	/* If you want to return error, then you have to send EOF (-1) */
	//return -1;
}

/* Handle USART6 stream input = custom function name, linked with USART6 stream in the beginning of main() function */
int USART6_Stream_InputFunction(FILE* f) {
	/* If any data at USART, return them */
	/* Do your custom implementation here when string ends */
	if (!TM_USART_BufferEmpty(USART6)) {
		return (int)TM_USART_Getc(USART6);
	}
	
	/* End of data, string is valid */
	/* You have to send -1 at the end of string */
	return -1;
}
