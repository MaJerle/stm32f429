/**
 *	USB VCP for STM32F4xx example.
 *	
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision
 *
 * Add line below to use this example with F429 Discovery board (in defines.h file)
 *
 * #define USE_USB_OTG_HS
 */
#include "tm_stm32f4_usb_vcp.h"
#include "tm_stm32f4_disco.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include <math.h>

#define BUTTON_WIDTH 60
#define BUTTON_HEIGHT 67 
#define BUTTON_NWIDE 3


void  sendStringViaUSB( 	char *str ) {
	
    int i;
    int string_len;

    string_len = strlen(str);

    for(i = 0; i < string_len; i++) {
					TM_USB_VCP_Putc(str[i]);
    }

}	

int8_t makeButton(int n, char* label) {
	TM_ILI9341_Button_t button;
	
	button.x = 10 + (n % BUTTON_NWIDE) * (BUTTON_WIDTH + 10 ) ;
	button.y = 10 + floor(n / BUTTON_NWIDE) * (BUTTON_HEIGHT +10);
	button.width = BUTTON_WIDTH;
	button.height = BUTTON_HEIGHT;
	button.background = ILI9341_COLOR_RED;
	button.borderColor = ILI9341_COLOR_BLACK;
	button.label = label;
	button.color = ILI9341_COLOR_BLACK;
	button.font = &TM_Font_11x18;
	return (TM_ILI9341_Button_Add(&button));
}


int main(void) {
	uint8_t c, i;
	uint16_t number;
	char str[30];
	char *strs[12] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "D", "0", "E" };
	int8_t buttonPressed, buttons[12];
	TM_STMPE811_TouchData touchData;

	/* System Init */
	SystemInit();
	
	/* Initialize LED's. Make sure to check settings for your board in tm_stm32f4_disco.h file */
	TM_DISCO_LedInit();
	
	/* Initialize USB VCP */	
	TM_USB_VCP_Init();
	
	//Initialize LCD
	TM_ILI9341_Init();
	//Fill LCD with gray color
	TM_ILI9341_Fill(ILI9341_COLOR_GRAY);
	//Select orientation
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	//Select touch screen orientation
	touchData.orientation = TM_STMPE811_Orientation_Portrait_2;
	
	//Initialize Touch
	TM_STMPE811_Init();
	for(i=0;  i<12; i++) {
		buttons[i] = makeButton(i, strs[i]);
	}
	TM_ILI9341_Button_DrawAll();

/* algorithm
 * draw a grid
 * wait for touchdown
 * while touchdown
 * draw to next point
 * if point near button - light button, stash button N
 * end while
 * process touchup
 * if match, fire match to pi
 * delay 
 * redraw screen
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */


	while (1) {
		if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {
			buttonPressed = TM_ILI9341_Button_Touch(&touchData);
			if (buttonPressed >= 0) {
				//Any button pressed
				sprintf(str, "Pressed: Button %d", (buttonPressed + 1));
		sendStringViaUSB(str);
			}
		}
	}
}
	
