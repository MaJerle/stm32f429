/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_disco.h"

volatile uint8_t TM_INT_DISCO_ButtonPressed = 0;

void TM_DISCO_LedInit(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable GPIO peripheral clock for leds */
	RCC_AHB1PeriphClockCmd(TM_DISCO_LED_RCC, ENABLE);

	/**
	 * Pins set to PG13 and PG14
	 * Output type is push-pull
	 * Mode is output
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitStruct.GPIO_Pin = TM_DISCO_LED_PINS;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* Initialize pins */
	GPIO_Init(TM_DISCO_LED_PORT, &GPIO_InitStruct);
}

void TM_DISCO_ButtonInit(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable GPIO peripheral clock for button */
	RCC_AHB1PeriphClockCmd(TM_DISCO_BUTTON_RCC, ENABLE);

	/**
	 * Pin set to PA0
	 * Output type is push-pull
	 * Mode is Input
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitStruct.GPIO_Pin = TM_DISCO_BUTTON_PIN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* Initialize pin */
	GPIO_Init(TM_DISCO_BUTTON_PORT, &GPIO_InitStruct);
}

uint8_t TM_DISCO_ButtonOnPressed(void) {
	/* If button is now pressed, but was not already pressed */
	if (TM_DISCO_ButtonPressed() && !TM_INT_DISCO_ButtonPressed) {
		/* Set flag */
		TM_INT_DISCO_ButtonPressed = 1;
		
		/* Return button onpressed */
		return 1;
	}
	
	/* Button is not pressed or it was already pressed before */
	return 0;
}

uint8_t TM_DISCO_ButtonOnReleased(void) {
	/* If button is now released, but was not already released */
	if (!TM_DISCO_ButtonPressed() && TM_INT_DISCO_ButtonPressed) {
		/* Clear flag */
		TM_INT_DISCO_ButtonPressed = 0;
		
		/* Return button onreleased */
		return 1;
	}
	
	/* Button is not released or it was already released before */
	return 0;
}
