/**
 *	SPI library for STM32F4xx.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-05-spi-for-stm32f4xx/
 *	@version 	v1.4
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 *	
 *	Version 1.4
 *	 - November 09, 2014
 *	 - Added methods for 16-bit SPI mode
 *
 *	Version 1.3
 *	 - September 14, 2014
 *	 - Added additional pins for SPI2
 *
 *	It support all 6 SPIs in master with 2Line Full Duplex mode
 *
 *	All six spis work the same principle by default.
 *	- Master
 *	- 8data bits
 *	- data sampled at first edge
 *	- prescaler 32
 *	- firstbit MSB
 *	- software SS pin configure
 *	- direction is full duplex 2 wire
 *	
 *	Pinout
 *				|PINS PACK 1			|PINS PACK 2			|PINS PACK 3
 *		SPIX	|MOSI	MISO	SCK		|MOSI	MISO	SCK		|MOSI	MISO	SCK
 *				|			
 *		SPI1	|PA7	PA6		PA5		|PB5	PB4		PB3		|
 *		SPI2	|PC3	PC2		PB10	|PB15	PB14	PB13	|PI3	PI2		PI0
 *		SPI3	|PB5	PB4		PB3		|PC12	PC11	PC10	|
 *		SPI4	|PE6	PE5		PE2		|PE14	PE13	PE12	|
 *		SPI5	|PF9	PF8		PF7		|PF11	PH7		PH6		|
 *		SPI6	|PG14	PG12	PG13	|
 *	
 *	Possible changes to each SPI. Set this defines in your defines.h file.
 *	
 *	Change x with 1-6, to match your SPI
 *	
 *	//Default prescaler
 *	#define TM_SPIx_PRESCALER	SPI_BaudRatePrescaler_32
 *	//Specify datasize
 *	#define TM_SPIx_DATASIZE 	SPI_DataSize_8b
 *	//Specify which bit is first
 *	#define TM_SPIx_FIRSTBIT 	SPI_FirstBit_MSB
 *	//Mode, master or slave
 *	#define TM_SPIx_MASTERSLAVE	SPI_Mode_Master
 *	//Specify mode of operation, clock polarity and clock phase
 *	#define TM_SPIx_MODE		TM_SPI_Mode_0
 *	
 */
#ifndef TM_SPI_H
#define TM_SPI_H 140

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx SPI
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "defines.h"

/**
 * Every SPIx can work in 4 different modes
 */
typedef enum {
	TM_SPI_Mode_0,	//Clock polarity low, clock phase 1st edge
	TM_SPI_Mode_1,	//Clock polarity low, clock phase 2nd edge
	TM_SPI_Mode_2,	//Clock polarity high, clock phase 1st edge
	TM_SPI_Mode_3	//Clock polarity high, clock phase 2nd edge
} TM_SPI_Mode_t;

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
#ifndef TM_SPI1_MASTERSLAVE
#define TM_SPI1_MASTERSLAVE	SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef TM_SPI1_MODE
#define TM_SPI1_MODE		TM_SPI_Mode_0
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
#ifndef TM_SPI2_MASTERSLAVE
#define TM_SPI2_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef TM_SPI2_MODE
#define TM_SPI2_MODE		TM_SPI_Mode_0
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
#ifndef TM_SPI3_MASTERSLAVE
#define TM_SPI3_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef TM_SPI3_MODE
#define TM_SPI3_MODE		TM_SPI_Mode_0
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
#ifndef TM_SPI4_MASTERSLAVE
#define TM_SPI4_MASTERSLAVE	SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef TM_SPI4_MODE
#define TM_SPI4_MODE		TM_SPI_Mode_0
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
#ifndef TM_SPI5_MASTERSLAVE
#define TM_SPI5_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef TM_SPI5_MODE
#define TM_SPI5_MODE		TM_SPI_Mode_0
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
#ifndef TM_SPI6_MASTERSLAVE
#define TM_SPI6_MASTERSLAVE SPI_Mode_Master
#endif
//Specify mode of operation, clock polarity and clock phase
#ifndef TM_SPI6_MODE
#define TM_SPI6_MODE		TM_SPI_Mode_0
#endif
//----- SPI6 options end -------

/**
 * Every SPIx has 3 pins for MISO, MOSI and SCK
 */
typedef enum {
	TM_SPI_PinsPack_1,
	TM_SPI_PinsPack_2,
	TM_SPI_PinsPack_3
} TM_SPI_PinsPack_t;

/**
 * Initialize SPIx
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx:
 * 		SPI 1 - 6
 * 	- TM_SPI_PinsPack_t pinspack: select pins pack to use
 * 		- TM_SPI_PinsPack_1
 * 		- TM_SPI_PinsPack_2
 *		- TM_SPI_PinsPack_3
 */
extern void TM_SPI_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack);

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

/**
 * Send and receive multiple data bytes over SPI
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 * 	- uint8_t dataOut: pointer to data to be sent out
 *	- uint8_t dataIn: pointer to received data
 *	- uint16_t count: number of bytes to send
 *
 * No returns
 */
extern void TM_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint16_t count);

/**
 * Write multiple data via SPI
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 * 	- uint8_t dataOut: pointer to data to be sent out
 *	- uint16_t count: number of bytes to send
 *
 * No returns
 */
extern void TM_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint16_t count);

/**
 * Send and receive multiple data bytes over SPI
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 *	- uint8_t dataIn: pointer to received data
 *	- uint8_t dummy: dummy byte to be sent to SPI
 *	- uint16_t count: number of bytes to receive
 *
 * No returns
 */
extern void TM_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint16_t count);

/**
 * Send and receive data over SPI in 16-bit SPI mode
 * Selected SPI must be set in 16-bit mode
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 * 	- uint16_t data: data to be sent
 *
 * Returns: Data returned from slave
 */
extern uint16_t TM_SPI_Send16(SPI_TypeDef* SPIx, uint16_t data);

/**
 * Send and receive multiple data bytes over SPI in 16-bit SPI mode
 * Selected SPI must be set in 16-bit mode
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 * 	- uint16_t dataOut: pointer to data to be sent out
 *	- uint16_t dataIn: pointer to received data
 *	- uint16_t count: number of bytes to send
 *
 * No returns
 */
extern void TM_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint16_t count);

/**
 * Write multiple data via SPI in 16-bit SPI mode
 * Selected SPI must be set in 16-bit mode
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 * 	- uint16_t dataOut: pointer to data to be sent out
 *	- uint16_t count: number of bytes to send
 *
 * No returns
 */
extern void TM_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t count);

/**
 * Send and receive multiple data bytes over SPI in 16-bit SPI mode
 * Selected SPI must be set in 16-bit mode
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx: Select SPI which will operate with data
 *	- uint16_t dataIn: pointer to received data
 *	- uint16_t dummy: dummy 16bit to be sent to SPI
 *	- uint16_t count: number of bytes to receive
 *
 * No returns
 */
extern void TM_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint16_t count);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

