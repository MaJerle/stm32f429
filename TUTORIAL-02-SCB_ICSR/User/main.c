/**
 * Keil project example for detecting code execution inside interrupt
 *
 * Before you start, select your target, on the right of the "Load" button
 *
 * @author    Tilen Majerle
 * @email     tilen@majerle.eu
 * @website   http://stm32f4-discovery.com
 * @ide       Keil uVision 5
 * @conf      PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 * @packs     STM32F4xx Keil packs version 2.4.0 or greater required
 * @stdperiph STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"

/* Create function for checking */
void SendString(void) {
	/* Your function can do work here */
	
	/* Check if we are called from interrupt */
	if (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) {
		/* We are called from interrupt */
		TM_USART_Puts(USART2, "Called from interrupt\n");
	} else {
		/* Normal call, no interrupt, thread mode */
		TM_USART_Puts(USART2, "Normal call, no interrupt called\n");
	}
	
	/* Your function can do work here */
}

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay and 1ms interrupts */
	TM_DELAY_Init();
	
	/* Init USART2 on TX pin: PA2 at 921600 bauds */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	while (1) {
		/* Call function from thread mode every 500ms */
		if (TM_DELAY_Time() >= 500) {
			/* Reset tune */
			TM_DELAY_SetTime(0);
			
			/* Call function from thread mode */
			SendString();
		}
	}
}

/* Called when TM DELAY makes interrupts */
void TM_DELAY_1msHandler(void) {
	static uint32_t x;
	
	/* Call function from interrupt every 300ms */
	if (++x >= 300) {
		x = 0;
		
		/* Call function from interrupt */
		SendString();
	}
}
