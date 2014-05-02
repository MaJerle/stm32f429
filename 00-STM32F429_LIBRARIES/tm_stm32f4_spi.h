/**
 *	SPI library for STM32F4(2/3)9.
 *	It support all 6 SPIs in master with 2Line Full Duplex mode
 *
 *	All six spi's work the same principle by default.
 *	- Master
 *	- 8data bits
 *	- data sampled at first edge
 *	- prescaler 32
 *	- firstbit MSB
 *	- software SS pin configure
 *	- direction is full duplex 2 wire
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@link		http://bit.ly/1lgtcjy
 *	@version 	v1.1
 */
#ifndef TM_SPI_H
#define TM_SPI_H 100

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "defines.h"

/**
 * Every SPIx has 2 pins for MISO, MOSI and SCK
 */
typedef enum {
	TM_SPI_PinsPack_1,
	TM_SPI_PinsPack_2
} TM_SPI_PinsPack_t;

//----- SPI1 options start -------
//Options can be overwriten in defines.h file
#ifndef TM_SPI1_PRESCALER
#define TM_SPI1_PRESCALER	SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef TM_SPI1_DATASIZE
#define TM_SPI1_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef TM_SPI1_FIRSTBIT
#define TM_SPI1_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef TM_SPI1_MODE
#define TM_SPI1_MODE 		SPI_Mode_Master
#endif
//Specify idle clock state
#ifndef TM_SPI1_CPOL
#define TM_SPI1_CPOL		SPI_CPOL_Low
#endif
//Specify when data is sampled
#ifndef TM_SPI1_CPHA
#define TM_SPI1_CPHA		SPI_CPHA_1Edge
#endif
//----- SPI1 options end -------

//----- SPI2 options start -------
//Options can be overwriten in defines.h file
#ifndef TM_SPI2_PRESCALER
#define TM_SPI2_PRESCALER	SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef TM_SPI2_DATASIZE
#define TM_SPI2_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef TM_SPI2_FIRSTBIT
#define TM_SPI2_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef TM_SPI2_MODE
#define TM_SPI2_MODE 		SPI_Mode_Master
#endif
//Specify idle clock state
#ifndef TM_SPI2_CPOL
#define	TM_SPI2_CPOL		SPI_CPOL_Low
#endif
//Specify when data is sampled
#ifndef TM_SPI2_CPHA
#define TM_SPI2_CPHA		SPI_CPHA_1Edge
#endif
//----- SPI2 options end -------

//----- SPI3 options start -------
//Options can be overwriten in defines.h file
#ifndef TM_SPI3_PRESCALER
#define TM_SPI3_PRESCALER	SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef TM_SPI3_DATASIZE
#define TM_SPI3_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef TM_SPI3_FIRSTBIT
#define TM_SPI3_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef TM_SPI3_MODE
#define TM_SPI3_MODE 		SPI_Mode_Master
#endif
//Specify idle clock state
#ifndef TM_SPI3_CPOL
#define	TM_SPI3_CPOL		SPI_CPOL_Low
#endif
//Specify when data is sampled
#ifndef TM_SPI3_CPHA
#define TM_SPI3_CPHA		SPI_CPHA_1Edge
#endif
//----- SPI3 options end -------

//----- SPI4 options start -------
//Options can be overwriten in defines.h file
#ifndef TM_SPI4_PRESCALER
#define TM_SPI4_PRESCALER	SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef TM_SPI4_DATASIZE
#define TM_SPI4_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef TM_SPI4_FIRSTBIT
#define TM_SPI4_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef TM_SPI4_MODE
#define TM_SPI4_MODE 		SPI_Mode_Master
#endif
//Specify idle clock state
#ifndef TM_SPI4_CPOL
#define	TM_SPI4_CPOL		SPI_CPOL_Low
#endif
//Specify when data is sampled
#ifndef TM_SPI4_CPHA
#define TM_SPI4_CPHA		SPI_CPHA_1Edge
#endif
//----- SPI4 options end -------

//----- SPI5 options start -------
//Options can be overwriten in defines.h file
#ifndef TM_SPI5_PRESCALER
#define TM_SPI5_PRESCALER	SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef TM_SPI5_DATASIZE
#define TM_SPI5_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef TM_SPI5_FIRSTBIT
#define TM_SPI5_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef TM_SPI5_MODE
#define TM_SPI5_MODE 		SPI_Mode_Master
#endif
//Specify idle clock state
#ifndef TM_SPI5_CPOL
#define	TM_SPI5_CPOL		SPI_CPOL_Low
#endif
//Specify when data is sampled
#ifndef TM_SPI5_CPHA
#define TM_SPI5_CPHA		SPI_CPHA_1Edge
#endif
//----- SPI5 options end -------

//----- SPI6 options start -------
//Options can be overwriten in defines.h file
#ifndef TM_SPI6_PRESCALER
#define TM_SPI6_PRESCALER	SPI_BaudRatePrescaler_32
#endif
//Specify datasize
#ifndef TM_SPI6_DATASIZE
#define TM_SPI6_DATASIZE 	SPI_DataSize_8b
#endif
//Specify which bit is first
#ifndef TM_SPI6_FIRSTBIT
#define TM_SPI6_FIRSTBIT 	SPI_FirstBit_MSB
#endif
//Mode, master or slave
#ifndef TM_SPI6_MODE
#define TM_SPI6_MODE 		SPI_Mode_Master
#endif
//Specify idle clock state
#ifndef TM_SPI6_CPOL
#define	TM_SPI6_CPOL		SPI_CPOL_Low
#endif
//Specify when data is sampled
#ifndef TM_SPI6_CPHA
#define TM_SPI6_CPHA		SPI_CPHA_1Edge
#endif
//----- SPI6 options end -------


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

#ifdef SPI1
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
#endif

#ifdef SPI2
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
#endif

#ifdef SPI3
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
#endif

#ifdef SPI4
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
#endif

#ifdef SPI5
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
#endif

#ifdef SPI6
/**
 * SPI6 uses pins:
 * 	- Pins pack 1:
 * 		- MOSI: PG14
 * 		- MISO: PG12
 * 		- SCK:  PG13
 */
extern void TM_SPI6_Init(void);
#endif

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
