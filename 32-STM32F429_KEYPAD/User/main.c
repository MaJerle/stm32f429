/**
 *	Keil project for matrix keypad 4x4
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
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_keypad.h"
#include "tm_stm32f4_usart.h"

#include <stdio.h>

int main(void) {
	/* Create keypad instance */
	TM_KEYPAD_Button_t Keypad_Button;
	char buff[20];
	
	/* Initialize system */
	SystemInit();
	
	/* Init delay functions */
	TM_DELAY_Init();
	
	/* Initialize leds */
	TM_DISCO_LedInit();
	
	/* Initialize USART 1, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize matrix keyboard */
	TM_KEYPAD_Init(TM_KEYPAD_Type_Large);

	while (1) {
		/* Read keyboard data */
		Keypad_Button = TM_KEYPAD_Read();
		
		/* Keypad was pressed */
		if (Keypad_Button != TM_KEYPAD_Button_NOPRESSED) {/* Keypad is pressed */
			switch (Keypad_Button) {
				case TM_KEYPAD_Button_0:		/* Button 0 pressed */
					TM_DISCO_LedToggle(LED_ALL);
					break;
				case TM_KEYPAD_Button_1:		/* Button 1 pressed */
					TM_DISCO_LedOn(LED_GREEN);
					break;
				case TM_KEYPAD_Button_2:		/* Button 2 pressed */
					TM_DISCO_LedOff(LED_GREEN);
					break;
				case TM_KEYPAD_Button_3:		/* Button 3 pressed */
					TM_DISCO_LedOn(LED_RED);
					break;
				case TM_KEYPAD_Button_4:		/* Button 4 pressed */
					TM_DISCO_LedOff(LED_RED);
					break;
				case TM_KEYPAD_Button_5:		/* Button 5 pressed */
					TM_DISCO_LedOn(LED_ORANGE);
					break;
				case TM_KEYPAD_Button_6:		/* Button 6 pressed */
					TM_DISCO_LedOff(LED_ORANGE);
					break;
				case TM_KEYPAD_Button_7:		/* Button 7 pressed */
					TM_DISCO_LedOn(LED_BLUE);
					break;
				case TM_KEYPAD_Button_8:		/* Button 8 pressed */
					TM_DISCO_LedOff(LED_BLUE);
					break;
				case TM_KEYPAD_Button_9:		/* Button 9 pressed */
					/* Do your stuff here */
					break;
				case TM_KEYPAD_Button_STAR:		/* Button STAR pressed */
					TM_DISCO_LedOn(LED_ALL);
					break;
				case TM_KEYPAD_Button_HASH:		/* Button HASH pressed */
					TM_DISCO_LedOff(LED_ALL);
					break;
				case TM_KEYPAD_Button_A:		/* Button A pressed, only on large keyboard */
					/* Do your stuff here */
					break;
				case TM_KEYPAD_Button_B:		/* Button B pressed, only on large keyboard */
					/* Do your stuff here */
					break;
				case TM_KEYPAD_Button_C:		/* Button C pressed, only on large keyboard */
					/* Do your stuff here */
					break;
				case TM_KEYPAD_Button_D:		/* Button D pressed, only on large keyboard */
					/* Do your stuff here */
					break;
				default:
					break;
			}
			
			/* Send to user */
			sprintf(buff, "Pressed: %u us\n", (uint8_t)Keypad_Button);
			TM_USART_Puts(USART1, buff);
		}
	}
}

/* 1ms handler */
void TM_DELAY_1msHandler(void) {
	/* Process keypad */
	TM_KEYPAD_Update();
}
