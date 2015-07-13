/**
 *	Keil project example for USART custom pin initialization
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"

int main(void) {
	uint8_t d;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize USART1, with custom pins */
	TM_USART_Init(USART1, TM_USART_PinsPack_Custom, 115200);
	
	while (1) {
		/* If anything received */
		if ((d = TM_USART_Getc(USART1)) != 0) {
			/* Send data back */
			TM_USART_Putc(USART1, d);
		}
	}
}

/* Custom pinout callback for USART */
void TM_USART_InitCustomPinsCallback(USART_TypeDef* USARTx, uint16_t AlternateFunction) {
	/* Check for proper USART */
	if (USARTx == USART1) {
		/* Initialize custom pins */
		/* TX pin = PB6 */
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low, AlternateFunction);
		/* RX pin = PA10 */
		TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low, AlternateFunction);
	}
}
