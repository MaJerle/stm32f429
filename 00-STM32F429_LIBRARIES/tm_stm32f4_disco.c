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

/* Button pressed status for onpressed/onreleased events */
static volatile uint8_t TM_INT_DISCO_ButtonPressed = 0;

void TM_DISCO_LedInit(void) {
	/* Set pins as output */
	TM_GPIO_Init(TM_DISCO_LED_PORT, LED_ALL, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
	
	/* Turn leds off */
	TM_DISCO_LedOff(LED_ALL);
}

void TM_DISCO_ButtonInit(void) {
	/* Set pin as input */
	TM_GPIO_Init(TM_DISCO_BUTTON_PORT, TM_DISCO_BUTTON_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_DISCO_BUTTON_PULL, TM_GPIO_Speed_Low);
}

uint8_t TM_DISCO_ButtonOnPressed(void) {
	/* If button is now pressed, but was not already pressed */
	if (TM_DISCO_ButtonPressed()) {
		if (!TM_INT_DISCO_ButtonPressed) {
			/* Set flag */
			TM_INT_DISCO_ButtonPressed = 1;
			
			/* Return button onpressed */
			return 1;
		}
	} else {
		/* Clear flag */
		TM_INT_DISCO_ButtonPressed = 0;
	}
	
	/* Button is not pressed or it was already pressed before */
	return 0;
}

uint8_t TM_DISCO_ButtonOnReleased(void) {
	/* If button is now released, but was not already released */
	if (!TM_DISCO_ButtonPressed()) {
		if (TM_INT_DISCO_ButtonPressed) {
			/* Set flag */
			TM_INT_DISCO_ButtonPressed = 0;
			
			/* Return button onreleased */
			return 1;
		}
	} else {
		/* Set flag */
		TM_INT_DISCO_ButtonPressed = 1;
	}
	
	/* Button is not released or it was already released before */
	return 0;
}
