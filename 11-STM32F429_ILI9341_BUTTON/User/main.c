/**
 *	Keil project for I2C example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@ide		Keil uVision 5
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include "tm_stm32f4_disco.h"
#include "button_back.h"
#include <stdio.h>

//Slave address STMPE811
#define ADDRESS		0x82 // 1000 001 0

void HexToBin(char *str, uint32_t hex, uint8_t count) {
	uint8_t i;
	for (i = 0; i < count; i++) {
		str[i] = (hex >> (count - i - 1) & 1) ? '1' : '0';
	}
	str[i] = 0;
}

int main(void) {
	TM_STMPE811_TouchData touchData;
	TM_ILI9341_Button_t button;
	int8_t buttonPressed;
	
	char str[30];
	//Initialize system
	SystemInit();
	
	
	//Initialize LCD
	TM_ILI9341_Init();
	TM_ILI9341_Fill(ILI9341_COLOR_GRAY);
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	//Select touch screen orientation
	touchData.orientation = TM_STMPE811_Orientation_Portrait_2;
	
	//Initialize Touch
	if (TM_STMPE811_Init()) {
		TM_ILI9341_Puts(20, 20, "Error", &TM_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
		while(1);
	}

	button.x = 10;
	button.y = 30;
	button.width = 220;
	button.height = 50;
	button.background = ILI9341_COLOR_RED;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = "Button 1";
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_11x18;
	TM_ILI9341_Button_Add(&button);
	
	//Button with custom background and without label text
	button.x = 10;
	button.y = 260;
	button.width = 105;
	button.height = 50;
	button.background = ILI9341_COLOR_GREEN;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = "Button 2";
	button.flags = TM_BUTTON_FLAG_NOLABEL;
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_11x18;
	button.image = buttonBackground;
	TM_ILI9341_Button_Add(&button);
	
	
	button.x = 125;
	button.y = 260;
	button.background = ILI9341_COLOR_RED;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = "Button 3";
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_7x10;
	button.flags = TM_BUTTON_FLAG_IMAGE;
	TM_ILI9341_Button_Add(&button);
	
	//Draw buttons
	TM_ILI9341_Button_DrawAll();

	while (1) {
		if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {
			buttonPressed = TM_ILI9341_Button_Touch(&touchData);
			if (buttonPressed >= 0) {
				//Any button pressed
				sprintf(str, "Pressed: Button %d", (buttonPressed + 1));
			} else {
				sprintf(str, "Press the button ");
			}
		}
		TM_ILI9341_Puts(10, 5, str, &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_GRAY);
	}
}

