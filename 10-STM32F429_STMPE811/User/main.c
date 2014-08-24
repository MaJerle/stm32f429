/**
 *	Keil project for STMPE811 touch driver controller
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@ide		Keil uVision 5
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include <stdio.h>

int main(void) {
	char str[30];
	//Create TouchData struct
	TM_STMPE811_TouchData touchData;
	
	//Initialize system
	SystemInit();
	
	//Initialize LCD
	TM_ILI9341_Init();
	//Fill with orange color
	TM_ILI9341_Fill(ILI9341_COLOR_ORANGE);
	//Rotate LCD
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	//Initialize Touch
	if (TM_STMPE811_Init() != TM_STMPE811_State_Ok) {
		TM_ILI9341_Puts(20, 20, "STMPE811 Error", &TM_Font_11x18, ILI9341_COLOR_ORANGE, ILI9341_COLOR_BLACK);
		
		while (1);
	}
	
	//Select touch screen orientation
	touchData.orientation = TM_STMPE811_Orientation_Portrait_2;
	
	//Print some text
	TM_ILI9341_Puts(20, 20, "Press on LCD", &TM_Font_11x18, ILI9341_COLOR_ORANGE, ILI9341_COLOR_BLACK);
	TM_ILI9341_Puts(93, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);
	
	while (1) {
		if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {
			//Touch valid
			sprintf(str, "Pressed    \n\nX: %03d\nY: %03d", touchData.x, touchData.y);
			TM_ILI9341_Puts(20, 80, str, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
			
			TM_ILI9341_DrawPixel(touchData.x, touchData.y, 0x0000);
		}/* else {
			sprintf(str, "Not Pressed\n\n       \n      ");
			TM_ILI9341_Puts(20, 80, str, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
		}*/
	}
}

