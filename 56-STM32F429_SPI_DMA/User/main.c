/**
 *	Keil project for SPI DMA
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
 *
 * I have tested this example on F411-Nucleo board.
 * To fake slave device, I just connected MOSI and MISO pins together, so what I sent I also expect to receive.
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_spi_dma.h"
#include "tm_stm32f4_general.h"
#include "stdio.h"
#include "string.h"

uint8_t TX_Buffer[200], RX_Buffer[200];
uint32_t val;

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Init SPI */
	TM_SPI_Init(SPI1, TM_SPI_PinsPack_1);
	
	/* Init SPI DMA */
	TM_SPI_DMA_Init(SPI1);
	
	/* Set fake SPI TX buffer */
	memset(TX_Buffer, 0x00, sizeof(TX_Buffer));
	
	val = TM_GENERAL_DWTCounterEnable();
	
	/* Set CS low first here before you send data over SPI */
	
	
	TM_GENERAL_DWTCounterSetValue(0);
	/* Send data over SPI1 with DMA */
	/* Exchange data with SPI slave using SPI DMA */
	/* Exchange 5 bytes of data */
	TM_SPI_DMA_Transmit(SPI1, TX_Buffer, RX_Buffer, sizeof(TX_Buffer));
	
	/* Wait till SPI DMA do it's job */
	/* You can do other stuff instead */
	while (TM_SPI_DMA_Working(SPI1));
	val = TM_GENERAL_DWTCounterGetValue();
	
	/* Little delay, for debug */
	Delayms(10);
	
	TM_GENERAL_DWTCounterSetValue(0);
	TM_SPI_SendMulti(SPI1, TX_Buffer, RX_Buffer, sizeof(TX_Buffer));
	val = TM_GENERAL_DWTCounterGetValue();
	
	/* Send 5 bytes of data and don't care what you receive back from slave via DMA */
	/* RX_Buffer should not be changed in debug window */
	TM_SPI_DMA_Transmit(SPI1, TX_Buffer, NULL, 5);
	
	/* Wait till SPI DMA do it's job */
	/* You can do other stuff instead */
	while (TM_SPI_DMA_Working(SPI1));
	
	/* Little delay, for debug */
	Delayms(10);
	
	/* Receive 5 bytes of data, sent dummy 0x00 bytes to slave via DMA */
	TM_SPI_DMA_Transmit(SPI1, NULL, RX_Buffer, 5);
	
	/* Wait till SPI DMA do it's job */
	/* You can do other stuff instead */
	while (TM_SPI_DMA_Working(SPI1));
	
	
	/* Set CS high here after last byte is done by SPI */
	
	while (1) {
		
	}
}
