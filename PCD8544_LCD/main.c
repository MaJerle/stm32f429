/**
 * PCD8544 LCD controller driver example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.0.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_pcd8544.h"

int main(void) {
	//180MHz
	SystemInit();

	//Initialize LCD with 0x38 software contrast
	PCD8544_Init(0x38);

	//Go to x=14, y=3 position
	PCD8544_GotoXY(14, 3);

	//Print data with Pixel Set mode and Fontsize of 5x7px
	PCD8544_Puts("STM32F429", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_GotoXY(15, 13);
	PCD8544_Puts("Discovery", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_GotoXY(30, 26);
	PCD8544_Puts("2014", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);

	PCD8544_GotoXY(45, 42);
	//Put string with Pixel set mode and Fontsize of 3x5
	PCD8544_Puts("majerle.eu", PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

	//Display data on LCD
	PCD8544_Refresh();
    while (1) {

    }
}
