/**
 *	Keil project for DMA
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
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_usart_dma.h"

#include "string.h"

/* Create USART working buffer */
char USART_Buffer[100] = "Hello via USART2 with TX DMA before interrupt happens\n";

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Init USART2 on pins TX = PA2, RX = PA3 */
	/* This pins are used on Nucleo boards for USB to UART via ST-Link */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
	
	/* Say string without DMA */
	TM_USART_Puts(USART2, "Hello via USART2 without DMA\n");
	
	/* Init TX DMA for USART2 */
	/* Interrupts for USART2 DMA TX stream are also enabled */
	TM_USART_DMA_Init(USART2);
	
	/* Enable USART DMA interrupts */
	TM_USART_DMA_EnableInterrupts(USART2);
	
	while (1) {
		/* If any string arrived over USART */
		/* Expecting "\n" at the end of string from USART terminal or any other source */
		if (TM_USART_Gets(USART2, USART_Buffer, sizeof(USART_Buffer))) {
			/* Send it back over DMA */
			TM_USART_DMA_Send(USART2, (uint8_t *)USART_Buffer, strlen(USART_Buffer));

			/* Wait till DMA works */
			/* You can do other stuff here instead of waiting for DMA to end */
			while (TM_USART_DMA_Sending(USART2));
		}
	}
}

/* Called when transfer is completed for specific stream */
void TM_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		TM_USART_Puts(USART2, "Stream has finished with transfer\n");
	}
}

/* Called when half transfer is completed for specific stream */
void TM_DMA_HalfTransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "Stream has finished with half transfer\n");
	}
}

/* Called when transfer error occurs for specific stream */
void TM_DMA_TransferErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "Stream transfer error occured\n");
	}
}

/* Called when direct mode error occurs for specific stream */
void TM_DMA_DirectModeErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "Stream direct mode error occured\n");
	}
}

/* Called on FIFO error */
void TM_DMA_FIFOErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* Check if interrupt is for correct stream */
	if (DMA_Stream == TM_USART_DMA_GetStream(USART2)) {
		/* Do stuff here */
		//TM_USART_Puts(USART2, "FIFO error occured\n");
	}
}
