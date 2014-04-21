/**
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#include "stm32f4xx.h"
#include "stm32_ub_lcd_ili9341.h"
#include "stm32_ub_font.h"
#include "stm32_ub_adc1_dma.h"
#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

caddr_t _sbrk(int incr) {
	extern char _ebss; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &_ebss;
	}
	prev_heap_end = heap_end;
	char * stack = (char*) __get_MSP();
	if (heap_end + incr > stack) {
		errno = ENOMEM;
		return (caddr_t) -1;
	}
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int main(void) {
	//180MHz clock
	SystemInit();

	  // Init vom LCD
	  UB_LCD_Init();
	  // Init vom ADC (im DMA-Mode)

	  // Init der Layer
	  UB_LCD_LayerInit_Fullscreen();
	  // auf Hintergrund schalten
	  UB_LCD_SetLayer_1();
	  // Hintergrund komplett mit einer Farbe füllen
	  UB_LCD_FillLayer(RGB_COL_WHITE);
	  // auf Vordergrund schalten
	  UB_LCD_SetLayer_2();
	  // Vordergrund komplett mit einer Farbe füllen
	  UB_LCD_FillLayer(0x1234);

	  // Ueberschrift
	  UB_Font_DrawString(10,10,"Demo_10 :  ",&Arial_11x18,RGB_COL_WHITE,RGB_COL_RED);
	  UB_Font_DrawString(10,30,"ADC-DMA    ",&Arial_11x18,RGB_COL_WHITE,RGB_COL_RED);

	  char buf[100];

	  while(1) {
		  // ADC-Wert auslesen
		  sprintf(buf,"PA5 = %4d", 150);
		  UB_Font_DrawString(10,100,buf, &Arial_11x18,RGB_COL_BLACK,RGB_COL_GREEN);

		  // ADC-Wert auslesen
		  sprintf(buf,"PC3 = %4d", 150);
		  UB_Font_DrawString(10,150,buf,&Arial_11x18,RGB_COL_BLACK,RGB_COL_GREEN);
	  }
}
