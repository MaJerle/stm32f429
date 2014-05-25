#ifndef DEFINES_H
#define DEFINES_H

//Files used
#include "stm32f4xx.h"

//Change SPI5 baudrate prescaler APB2 / 2
#define TM_SPI5_PRESCALER	SPI_BaudRatePrescaler_2

#define TM_SPI5_CPOL		SPI_CPOL_High
#define TM_SPI5_CPHA		SPI_CPHA_1Edge

#endif 
