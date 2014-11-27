/**
 *	Keil project example for PCD8544 LCD controller
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
#include "tm_stm32f4_pcd8544.h"

int main(void) {
	/* Initialize system */
	SystemInit();

	/* Initialize LCD with 0x38 software contrast */
	PCD8544_Init(0x40);
	
	/* Go to x=14, y=3 position */
	PCD8544_GotoXY(14, 3);
	
	/* Print data with Pixel Set mode and Fontsize of 5x7px */
	PCD8544_Puts("STM32F4xx", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	/* Go to new location and write new string */
	PCD8544_GotoXY(15, 13);
	PCD8544_Puts("Discovery", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	/* Go to new location and write new string */
	PCD8544_GotoXY(30, 26);
	PCD8544_Puts("2014", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);

	/* Go to x=45, y=42 position */
	PCD8544_GotoXY(45, 42);
	
	/* Put string with Pixel set mode and Fontsize of 3x5 */
	PCD8544_Puts("majerle.eu", PCD8544_Pixel_Set, PCD8544_FontSize_3x5);
	
	/* Put data to LCD */
	PCD8544_Refresh();

    while (1) {

    }
}

