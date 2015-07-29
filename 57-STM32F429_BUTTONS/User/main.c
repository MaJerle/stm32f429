/**
 *	Keil project example for driving buttons
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
#include "tm_stm32f4_button.h"
#include "tm_stm32f4_usart.h"

/* Button 1 handler */
void BUTTON1_EventHandler(TM_BUTTON_PressType_t type);

/* Button 2 handler */
void BUTTON2_EventHandler(TM_BUTTON_PressType_t type);

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Init USART2, TX: PA2, RX: PA3, 921600 baud */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	/* For Discovery boards */
	/* GPIOA, pin 0, high (1) when pressed */
	TM_BUTTON_Init(GPIOA, GPIO_PIN_0, 1, BUTTON1_EventHandler);
	
	/* For Nucleo boards */
	/* GPIOA, pin 0, low (0) when pressed */
	TM_BUTTON_Init(GPIOC, GPIO_PIN_13, 0, BUTTON2_EventHandler);
	
	while (1) {
		/* Update buttons */
		TM_BUTTON_Update();
	}
}

void BUTTON1_EventHandler(TM_BUTTON_PressType_t type) {
	/* Check button */
	if (type == TM_BUTTON_PressType_OnPressed) {
		TM_USART_Puts(USART2, "Button 1 onPressed\n");
	} else if (type == TM_BUTTON_PressType_Normal) {
		TM_USART_Puts(USART2, "Normal press detected on button 1\n");
	} else {
		TM_USART_Puts(USART2, "Long press detected on button 1\n");
	}
}

void BUTTON2_EventHandler(TM_BUTTON_PressType_t type) {
	/* Check button */
	if (type == TM_BUTTON_PressType_OnPressed) {
		TM_USART_Puts(USART2, "Button 2 onPressed\n");
	} else if (type == TM_BUTTON_PressType_Normal) {
		TM_USART_Puts(USART2, "Normal press detected on button 2\n");
	} else {
		TM_USART_Puts(USART2, "Long press detected on button 2\n");
	}
}
