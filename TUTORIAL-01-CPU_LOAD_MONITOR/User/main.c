/**
 *	Keil project for very basic and primitive CPU load monitor
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
#include "tm_stm32f4_general.h"
#include "tm_stm32f4_usart.h"

#include "stdio.h"

/* Go to sleep mode */
void GoToSleepMode(void);

int main(void) {
	__IO uint32_t i;
	
	/* Initialize system */
	SystemInit();
	
	/* Init DWT timer to count processor cycles */
	TM_GENERAL_DWTCounterEnable();
	
	/* Reset counter */
	TM_GENERAL_DWTCounterSetValue(0);
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Initialize button */
	TM_DISCO_ButtonInit();
	
	/* Init USART2, TX: PA2, RX: PA3, 921600 baud */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 921600);
	
	while (1) {
		/* If button is pressed, do some useless dummy counting */
		/* Otherwise, go sleep directly */
		if (TM_DISCO_ButtonPressed()) {
			/* Do some while looping */
			i = 0;
			while (i++ < 0x2FFF);
		}
		
		/* Toggle leds */
		TM_DISCO_LedToggle(LED_ALL);
		
		/* Go to sleep mode */
		GoToSleepMode();
	}
}

void GoToSleepMode(void) {
	uint32_t t;
	static uint32_t l = 0;
	static uint32_t WorkingTime = 0;
	static uint32_t SleepingTime = 0;
	static uint32_t LastTime = 0;
	
	/* Add to working time */
	WorkingTime += DWT->CYCCNT - l;
	
	/* Save count cycle time */
	t = DWT->CYCCNT;
	
	/* Disable interrupts */
	__disable_irq();
	
	/* Go to sleep mode */
	/* Wait for wake up interrupt, systick can do it too */
	__WFI();
	
	/* Increase number of sleeping time in CPU cycles */
	SleepingTime += DWT->CYCCNT - t;
	
	/* Save current time to get number of working CPU cycles */
	l = DWT->CYCCNT;
	
	/* Enable interrupts, process/execute an interrupt routine which wake up CPU */
	__enable_irq();
	
	/* Every 1000ms print CPU load via USART */
	if ((TM_DELAY_Time() - LastTime) >= 1000) {
		/* Save new time */
		LastTime = TM_DELAY_Time();
		
		/* Print to user */
		printf("W: %u; S: %u; L: %5.2f %%\n", WorkingTime, SleepingTime, ((float)WorkingTime / (float)(SleepingTime + WorkingTime) * 100));
		
		/* Reset time */
		SleepingTime = 0;
		WorkingTime = 0;
	}
}

/* Handle printf functionality */
int fputc(int ch, FILE* fil) {
	/* Send character over USART */
	TM_USART_Putc(USART2, ch);
	
	/* Return character */
	return ch;
}

