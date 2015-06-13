/**
 *	Keil project example for HMC5883L magnetic sensor
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
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_hmc5883l.h"

#include "stdio.h"

TM_HMC5883L_t HMC5883L; 

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Init USART2, TX: PA2, RX: PA3, 921600 baud */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	/* Init HMC5883L sensor */
	if (TM_HMC5883L_Init(&HMC5883L, TM_HMC5883L_Gain_1_3, TM_HMC5883L_OutputRate_15Hz) == TM_HMC5883L_Result_Ok) {
		/* Device OK */
		printf("Device Initialized\n");
	} else {
		printf("Device Error\n");
		
		/* Infinite loop */
		while (1);
	}
	
	while (1) {
		/* Check if data ready */
		if (TM_HMC5883L_DataReady(&HMC5883L) == TM_HMC5883L_Result_Ok) {
			/* Read data */
			TM_HMC5883L_Read(&HMC5883L);
			
			/* Send over USART */
			printf("X: %4f Y: %4f Z: %4f\n", HMC5883L.X, HMC5883L.Z, HMC5883L.Y); 
		}
	}
}

/* Main EXTI handler */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
	/* If data ready interrupt */
	if (GPIO_Pin == HMC5883L_DRDY_PIN) {
		/* Process interrupt */
		TM_HMC5883L_ProcessInterrupt(&HMC5883L);
	}
}

/* printf handler */
int fputc(int ch, FILE* f) {
	/* Send over USART */
	TM_USART_Putc(USART2, ch);
	
	/* Return character */
	return ch;
}
