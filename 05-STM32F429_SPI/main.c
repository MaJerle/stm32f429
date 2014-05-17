/**
 * SPI example.
 *
 * In example we have one slave and it's SS pin is connected to ground
 *
 * Note that if you want to use SS pin, you have to manually use pin you wish
 * and make sure, you put it low, before data is sent, otherwise data will
 * not be detected by slave.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_spi.h"

int main(void) {
	//180MHz clock
	SystemInit();

	//Initialize SPI
	TM_SPI1_Init(TM_SPI_PinsPack_1);
	//Send 0x15 over SPI1
	TM_SPI_Send(SPI1, 0x15);
    while(1) {

    }
}
