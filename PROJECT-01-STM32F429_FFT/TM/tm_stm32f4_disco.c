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

void TM_DISCO_LedInit(void) {
	//GPIO Init structure
	GPIO_InitTypeDef GPIO_InitDef;
	//Enable GPIOG peripheral clock for leds
	RCC_AHB1PeriphClockCmd(TM_DISCO_LED_RCC, ENABLE);

	/**
	 * Pins set to PG13 and PG14
	 * Output type is push-pull
	 * Mode is output
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitDef.GPIO_Pin = TM_DISCO_LED_PINS;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(TM_DISCO_LED_PORT, &GPIO_InitDef);
}

void TM_DISCO_ButtonInit(void) {
	//GPIO Init structure
	GPIO_InitTypeDef GPIO_InitDef;
	//Enable GPIOA peripheral clock for button
	RCC_AHB1PeriphClockCmd(TM_DISCO_BUTTON_RCC, ENABLE);

	/**
	 * Pin set to PA0
	 * Output type is push-pull
	 * Mode is Input
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitDef.GPIO_Pin = TM_DISCO_BUTTON_PIN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(TM_DISCO_BUTTON_PORT, &GPIO_InitDef);
}

void TM_DISCO_LedOn(uint16_t led) {
	GPIO_SetBits(TM_DISCO_LED_PORT, led);
}

void TM_DISCO_LedOff(uint16_t led) {
	GPIO_ResetBits(TM_DISCO_LED_PORT, led);
}

void TM_DISCO_LedToggle(uint16_t led) {
	GPIO_ToggleBits(TM_DISCO_LED_PORT, led);
}

uint8_t TM_DISCO_LedIsOn(uint16_t led) {
	return GPIO_ReadOutputDataBit(TM_DISCO_LED_PORT, led) == Bit_SET;
}

void TM_DISCO_SetLed(uint16_t led, uint8_t state) {
	if (state) {
		TM_DISCO_LedOn(led);
	} else {
		TM_DISCO_LedOff(led);
	}
}

uint8_t TM_DISCO_ButtonPressed(void) {
	return GPIO_ReadInputDataBit(TM_DISCO_BUTTON_PORT, TM_DISCO_BUTTON_PIN) == TM_DISCO_BUTTON_PRESSED;
}
