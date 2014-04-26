/**
 *	SPI library for STM32F4(2/3)9.
 *	It support all 6 SPIs in master with 2Line Full Duplex mode
 *
 *	All six spi's work the same principle.
 *	- Master
 *	- 8data bits
 *	- data sampled at first edge
 *	- prescaler 32
 *	- software SS pin configure
 *	- direction is full duplex 2 wire
 *	- firstbit MSB
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */
#ifndef TM_SPI_H
#define TM_SPI_H 100

/**
 * Includes
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

/**
 * Every SPIx has 2 pins for MISO, MOSI and SCK
 */
typedef enum {
	TM_SPI_PinsPack_1,
	TM_SPI_PinsPack_2
} TM_SPI_PinsPack_t;


/**
 * Initialize SPIx separately
 *
 */

/**
 * Initialize SPIx
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx:
 * 		SPI 1 - 6
 * 	- TM_SPI_PinsPack_t pinspack: select pins pack to use
 * 		- TM_SPI_PinsPack_1
 * 		- TM_SPI_PinsPack_2
 */
extern void TM_SPI_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack);

/**
 * SPI1 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PA7
 * 		- MISO: PA6
 * 		- SCK:  PA5
 * 	- Pins pack 2:
 * 		- MOSI: PB5
 * 		- MISO: PB4
 * 		- SCK:  PB3
 */
extern void TM_SPI1_Init(TM_SPI_PinsPack_t pinspack);

/**
 * SPI2 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PC3
 * 		- MISO: PC2
 * 		- SCK:  PB10
 * 	- Pins pack 2:
 * 		- MOSI: PB15
 * 		- MISO: PB14
 * 		- SCK:  PB13
 */
extern void TM_SPI2_Init(TM_SPI_PinsPack_t pinspack);

/**
 * SPI3 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PB5
 * 		- MISO: PB4
 * 		- SCK:  PB3
 * 	- Pins pack 2:
 * 		- MOSI: PC12
 * 		- MISO: PC11
 * 		- SCK:  PC10
 */
extern void TM_SPI3_Init(TM_SPI_PinsPack_t pinspack);

/**
 * SPI4 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PE6
 * 		- MISO: PE5
 * 		- SCK:  PE2
 * 	- Pins pack 2:
 * 		- MOSI: PE14
 * 		- MISO: PE13
 * 		- SCK:  PE12
 */
extern void TM_SPI4_Init(TM_SPI_PinsPack_t pinspack);

/**
 * SPI5 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PF9
 * 		- MISO: PF8
 * 		- SCK:  PF7
 * 	- Pins pack 2:
 * 		- MOSI: PF11
 * 		- MISO: PH7
 * 		- SCK:  PH6
 */
extern void TM_SPI5_Init(TM_SPI_PinsPack_t pinspack);

/**
 * SPI6 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PG14
 * 		- MISO: PG12
 * 		- SCK:  PG13
 */
extern void TM_SPI6_Init(void);

/**
 * Send and receive data over SPI
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 * 	- uint8_t data: data to be sent
 *
 * Returns: Data returned from slave
 */
extern uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data);

#endif
