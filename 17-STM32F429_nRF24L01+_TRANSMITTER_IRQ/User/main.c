/**
 *	Keil project for NRF24L01+ transceiver using IRQ pin for data send and receive back from another NRF module
 *
 *	Transmitter code
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
#include "tm_stm32f4_nrf24l01.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_exti.h"
#include <stdio.h>

/* My address */
uint8_t MyAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
/* Receiver address */
uint8_t TxAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};

uint8_t dataOut[32], dataIn[32];

/* Interrupt pin settings */
#define IRQ_PORT    GPIOA
#define IRQ_PIN     GPIO_PIN_10

/* Interrupt status */
TM_NRF24L01_Transmit_Status_t transmissionStatus;
uint8_t printed = 0;
char str[40];

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize system and Delay functions */
	TM_DELAY_Init();
	
	/* Initialize onboard leds */
	TM_DISCO_LedInit();
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
	/* By default 2Mbps data rate and 0dBm output power */
	/* NRF24L01 goes to RX mode by default */
	TM_NRF24L01_Init(15, 32);
	
	/* Set 2MBps data rate and -18dBm output power */
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
	
	/* Set my address, 5 bytes */
	TM_NRF24L01_SetMyAddress(MyAddress);
	
	/* Set TX address, 5 bytes */
	TM_NRF24L01_SetTxAddress(TxAddress);
	
	/* Attach interrupt for NRF IRQ pin */
	TM_EXTI_Attach(IRQ_PORT, IRQ_PIN, TM_EXTI_Trigger_Falling);
	
	/* Reset counter */
	TM_DELAY_SetTime(2001);
	
	while (1) {
		/* Every 2 seconds */
		if (TM_DELAY_Time() > 2000) {
			/* Fill data with something */
			sprintf((char *)dataOut, "abcdefghijklmnoszxABCDEFCBDA");
			
			/* Display on USART */
			TM_USART_Puts(USART1, "pinging: ");
			
			/* Reset time, start counting milliseconds */
			TM_DELAY_SetTime(0);
			
			/* Transmit data, goes automatically to TX mode */
			TM_NRF24L01_Transmit(dataOut);
			
			/* Turn on led to indicate sending */
			TM_DISCO_LedOn(LED_GREEN);
			
			/* Set NRF state to sending */
			transmissionStatus = TM_NRF24L01_Transmit_Status_Sending;
			
			/* Reset printed flag */
			printed = 0;
		}
		
		/* Check if status has changed */
		if (
			transmissionStatus != TM_NRF24L01_Transmit_Status_Sending && /*!< We are not sending anymore */
			!printed                                                     /*!< We didn't print status to user */
		) {
			/* Print time in ms */
			sprintf(str, "%u ms: ", TM_DELAY_Time());
			
			/* Send to USART */
			TM_USART_Puts(USART1, str);
			
			/* Transmission was OK */
			if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
				TM_USART_Puts(USART1, "OK\n");
			}
			
			/* Message LOST */
			if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
				TM_USART_Puts(USART1, "LOST\n");
			}
			
			/* Set flag */
			printed = 1;
		}
	}
}

/* Interrupt handler */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
	/* Check for proper interrupt pin */
	if (GPIO_Pin == IRQ_PIN) {
		/* Read interrupts */
		uint8_t irq = NRF24L01_GET_INTERRUPTS;
		
		/* Check if transmitted OK */
		if (irq & NRF24L01_IRQ_TRAN_OK) {
			/* Save transmission status */
			transmissionStatus = TM_NRF24L01_Transmit_Status_Ok;
			
			/* Turn off led */
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();
		}
		
		/* Check if max transmission reached and last transmission failed */
		if (irq & NRF24L01_IRQ_MAX_RT) {
			/* Save transmission status */
			transmissionStatus = TM_NRF24L01_Transmit_Status_Lost;
			
			/* Turn off led */
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();
		}
		
		/* If data is ready on NRF24L01+ */
		if (irq & NRF24L01_IRQ_DATA_READY) {
			/* Get data from NRF24L01+ */
			TM_NRF24L01_GetData(dataIn);		
		}
		
		/* Clear interrupts */
		NRF24L01_CLEAR_INTERRUPTS;
	}
}
