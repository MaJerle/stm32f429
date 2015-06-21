/**
 *	Keil project for NRF24L01+ transceiver using IRQ pin for data receive
 *
 *	Receiver code
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

/* Receiver address */
uint8_t TxAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
/* My address */
uint8_t MyAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};

uint8_t dataIn[32];

/* Interrupt pin settings */
#define IRQ_PORT    GPIOA
#define IRQ_PIN     GPIO_PIN_10

/* NRF transmission status */
TM_NRF24L01_Transmit_Status_t transmissionStatus;

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize system and Delay functions */
	TM_DELAY_Init();
	
	/* Initialize onboard leds and button */
	TM_DISCO_LedInit();
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
	/* By default 2Mbps data rate and 0dBm output power */
	/* NRF24L01 goes to RX mode by default */
	TM_NRF24L01_Init(15, 32);
	
	/* Set RF settings, Data rate to 2Mbps, Output power to -18dBm */
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
	
	/* Set my address, 5 bytes */
	TM_NRF24L01_SetMyAddress(MyAddress);
	
	/* Set TX address, 5 bytes */
	TM_NRF24L01_SetTxAddress(TxAddress);
	
	/* Enable interrupts for NRF24L01+ IRQ pin */
	TM_EXTI_Attach(IRQ_PORT, IRQ_PIN, TM_EXTI_Trigger_Falling);

	while (1) {

	}
}

/* Interrupt handler */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
	/* Check for proper interrupt pin */
	if (GPIO_Pin == IRQ_PIN) {
		/* Read interrupts */
		uint8_t irq = NRF24L01_GET_INTERRUPTS;
		
		/* If data is ready on NRF24L01+ */
		if (irq & NRF24L01_IRQ_DATA_READY) {
			/* Get data from NRF24L01+ */
			TM_NRF24L01_GetData(dataIn);
			
			/* Start send */
			TM_DISCO_LedOn(LED_GREEN);
			
			/* Send it back, automatically goes to TX mode */
			TM_NRF24L01_Transmit(dataIn);
			
			/* Wait for data to be sent */
			do {
				/* Wait till sending */
				transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
			} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
			
			/* Send done */
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();		
		}
		
		/* Clear interrupts */
		NRF24L01_CLEAR_INTERRUPTS;
	}
}
