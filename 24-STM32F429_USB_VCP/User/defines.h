#ifndef DEFINES_H
#define DEFINES_H

/* Activate USB HS in FS mode */
#define USE_USB_OTG_HS
#include "stm32f4xx.h"

//Change SPI5 baudrate prescaler APB2 / 2
#define TM_SPI5_PRESCALER	SPI_BaudRatePrescaler_2
	#define TM_ILI9341_BUTTON_MAX_BUTTONS	13

#endif 
