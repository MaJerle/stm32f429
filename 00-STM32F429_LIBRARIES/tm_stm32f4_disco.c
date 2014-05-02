#include "tm_stm32f4_disco.h"

void TM_DISCO_LedInit(void) {
	//GPIO Init structure
	GPIO_InitTypeDef GPIO_InitDef;
	//Enable GPIOG peripheral clock for leds
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	/**
	 * Pins set to PG13 and PG14
	 * Output type is push-pull
	 * Mode is output
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitDef.GPIO_Pin = LED_GREEN | LED_RED;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOG, &GPIO_InitDef);
}

void TM_DISCO_ButtonInit(void) {
	//GPIO Init structure
	GPIO_InitTypeDef GPIO_InitDef;
	//Enable GPIOA peripheral clock for button
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/**
	 * Pin set to PA0
	 * Output type is push-pull
	 * Mode is Input
	 * No pull resistor
	 * Speed 100MHz
	 */
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOA, &GPIO_InitDef);
}

void TM_DISCO_LedOn(uint16_t led) {
	GPIO_SetBits(GPIOG, led & 0x6000);
}

void TM_DISCO_LedOff(uint16_t led) {
	GPIO_ResetBits(GPIOG, led & 0x6000);
}

void TM_DISCO_LedToggle(uint16_t led) {
	GPIO_ToggleBits(GPIOG, led & 0x6000);
}

uint8_t TM_DISCO_ButtonPressed(void) {
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}
