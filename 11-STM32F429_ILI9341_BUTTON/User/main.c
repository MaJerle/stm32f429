/**
 *	Keil project for ILI9341 button
 *
 *  Before you start, select your target, on the right of the "Load" button
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
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include "tm_stm32f4_disco.h"
#include "button_back.h"
#include <stdio.h>

int main(void) {
	/* TM_STMPE811_TouchData instance */
	TM_STMPE811_TouchData touchData;
	/* TM_ILI9341_Button_t instance */
	TM_ILI9341_Button_t button;
	int8_t buttonPressed, button1, button2, button3;
	char str[30];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize onboard leds */
	TM_DISCO_LedInit();
	
	/* Initialize LCD */
	TM_ILI9341_Init();
	/* Fill LCD with gray color */
	TM_ILI9341_Fill(ILI9341_COLOR_GRAY);
	/* Select orientation */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Select touch screen orientation */
	touchData.orientation = TM_STMPE811_Orientation_Portrait_2;
	
	/* Initialize Touch */
	TM_STMPE811_Init();
	
	/* Button 1, default configuration */
	/* Red with black border and black font 11x18 */
	button.x = 10;	/* X location */
	button.y = 30;	/* Y location */
	button.width = 219;
	button.height = 50;
	button.background = ILI9341_COLOR_RED;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = "Button 1";
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_11x18;
	/* Add button */
	button1 = TM_ILI9341_Button_Add(&button);
	
	/* Button with custom background and without label */
	button.x = 10;
	button.y = 260;
	button.width = 105;
	button.height = 50;
	button.background = ILI9341_COLOR_GREEN;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = "Button 2";
	/* Use background image and no label */
	button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_IMAGE;
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_11x18;
	button.image = buttonBackground; /* Variable stored in  */
	/* Add button */
	button2 = TM_ILI9341_Button_Add(&button);
	
	/* Button with custom background and with label and without border and 7x10 fontsize */
	button.x = 125;
	button.y = 260;
	button.background = ILI9341_COLOR_BLUE2;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = "Button 3";
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_7x10;
	button.flags = TM_BUTTON_FLAG_IMAGE | TM_BUTTON_FLAG_NOBORDER;	/* Use background image, without border */
	/* Add button */
	button3 = TM_ILI9341_Button_Add(&button);
	
	if (!TM_DISCO_LedIsOn(LED_RED)) {
		/* If led res is turned off, disable buttons 2 and 3 */
		TM_ILI9341_Button_Disable(button2);
		TM_ILI9341_Button_Disable(button3);
		TM_ILI9341_Puts(25, 220, "Buttons disabled!", &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_GRAY);
	}
	
	/* Draw buttons */
	TM_ILI9341_Button_DrawAll();
	
	/* Draw some strings */
	TM_ILI9341_Puts(45, 245, "LED on         LED off", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_GRAY);
	TM_ILI9341_Puts(10, 100, "Bottom buttons work\nonly if red led is turned on.\nYou can toggle red\nled with Button 1.", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_GRAY);

	while (1) {
		if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {
			buttonPressed = TM_ILI9341_Button_Touch(&touchData);
			if (buttonPressed >= 0) {
				/* Any button pressed */
				sprintf(str, "Pressed: Button %d", (buttonPressed + 1));
			} else {
				sprintf(str, "Press the button ");
			}
			if (buttonPressed == button1) {
				/* Red button 1 is pressed, toggle led */
				TM_DISCO_LedToggle(LED_RED);
				
				if (TM_DISCO_LedIsOn(LED_RED)) {
					/* If led is turned on, enable button 2 and button 3 */
					TM_ILI9341_Button_Enable(button2);
					TM_ILI9341_Button_Enable(button3);
					TM_ILI9341_Puts(25, 220, "Buttons enabled! ", &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_GRAY);
				} else {
					/* otherwise disable both */
					TM_ILI9341_Button_Disable(button2);
					TM_ILI9341_Button_Disable(button3);
					TM_ILI9341_Puts(25, 220, "Buttons disabled!", &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_GRAY);
				}
			} else if (buttonPressed == button2) {
				/* If button 2 is pressed, turn green led on */
				TM_DISCO_LedOn(LED_GREEN);
			} else if (buttonPressed == button3) {
				/* if button 3 is pressed, turn green led off */
				TM_DISCO_LedOff(LED_GREEN);
			}
		}
		TM_ILI9341_Puts(10, 5, str, &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_GRAY);
	}
}
