/**
 *	USART 1 example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_usart.h"

int main(void) {
	//Initialize system
	SystemInit();

	//Initialize USART1 at 9600 baud
	TM_USART_Init(9600);
	//Put string to terminal
	TM_USART_Puts("Hello world\n\r");

	uint8_t c;
	while (1) {
		//Get character from internal buffer
		c = TM_USART_Getc();
		if (c) {
			//If anything received, put it back to terminal
			TM_USART_Putc(c);
		}
	}
	return 0;
}
