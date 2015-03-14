/**
 *	SPI library for STM32F4xx.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-05-spi-for-stm32f4xx/
 *	@version 	v1.8
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
 * Version 1.8
 *	- March 10, 2015
 *	- Updated to be mode independent of STD/HAL drivers
 * 
 * Version 1.7
 *	- March 08, 2015
 *	- Added support for my new GPIO settings
 *
 * Version 1.6
 *	- March 05, 2015
 *	- Added 2 new functions, TM_SPI_InitFull and TM_SPI_GetPrescalerFromMaxFrequency()
 *
 * Version 1.5	
 *	- January 13, 2015
 *	- Added function TM_SPI_InitWithMode() to initialize SPI with custom SPI mode on the fly
 *
 * Version 1.4
 *	- November 09, 2014
 *	- Added methods for 16-bit SPI mode
 *
 * Version 1.3
 *	 - September 14, 2014
 *	 - Added additional pins for SPI2
 *
 * It supports all 6 SPIs in master with 2Line Full Duplex mode
 *
 * All six spis work the same principle by default.
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
 *	In case these pins are not good for you, you can use
 *	TM_SPI_PinsPack_Custom in function and callback function will be called,
 *	where you can initialize your custom pinout for your SPI peripheral
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
 */
#ifndef TM_SPI_H
#define TM_SPI_H 180

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
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

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
	TM_SPI_PinsPack_3,
	TM_SPI_PinsPack_Custom
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
 * Initialize SPIx with SPI mode
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx:
 * 		SPI 1 - 6
 * 	- TM_SPI_PinsPack_t pinspack: select pins pack to use
 * 		- TM_SPI_PinsPack_1
 * 		- TM_SPI_PinsPack_2
 *		- TM_SPI_PinsPack_3
 *	- TM_SPI_Mode_t SPI_Mode:
 *		SPI mode to be initialized
 *
 * No return
 */
extern void TM_SPI_InitWithMode(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode);

/**
 * Initialize SPIx with most used SPI features
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx:
 * 		SPI, x valid 1 - 6
 * 	- TM_SPI_PinsPack_t pinspack: select pins pack to use
 * 		- TM_SPI_PinsPack_1
 * 		- TM_SPI_PinsPack_2
 *		- TM_SPI_PinsPack_3
 *	- TM_SPI_Mode_t SPI_Mode:
 *		SPI mode to be initialized, CPOL and CPHA settings, Mode 0 to 3
 *	- uint16_t SPI_BaudRatePrescaler:
 *		SPI baudrate prescaler values. SPI_BaudRatePrescaler_x is valid, valid x is: 2, 4, 8, 16, 32, 64, 128, 256
 *	- uint16_t SPI_Mode:
 *		Master or slave mode: SPI_Mode_Master or SPI_Mode_Slave valid.
 *	- uint16_t SPI_FirstBit:
 *		Select first bit in your transmission, SPI_FirstBit_MSB or SPI_FirstBit_LSB are valid
 *
 * No return
 */
extern void TM_SPI_InitFull(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack, uint16_t SPI_BaudRatePrescaler, TM_SPI_Mode_t SPI_Mode_t, uint16_t SPI_Mode, uint16_t SPI_FirstBit);

/**
 * This function can be used to calculate smaller prescaler for your clock frequency
 * SPI has 8 prescalers available, 2,4,6,...,128,256
 *
 * This function will return you a bits you must set in your CR1 register.
 *
 * Imagine, you can use 20MHz max clock in your system, your system is running on 168MHz, and you use SPI on APB2 bus.
 * On 168 and 180MHz devices, APB2 works on Fclk/2, so 84 and 90MHz.
 * So, if you calculate this, prescaler will need to be 84MHz / 20MHz = 4.xx, but if you use 4 prescaler, then you will be over 20MHz.
 * You need 8 prescaler then. This function will calculate this.
 *
 * Parameters:
 * 	- SPI_TypeDef* SPIx:
 * 		SPI, x valid 1 - 6
 *	- uint32_t MAX_SPI_Frequency:
 *		Maximal frequency you can use on your device on SPI port
 *
 * Returns bits you must set in SPI CR1 register.
 */
extern uint16_t TM_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency);

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
 * No return
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
 * No return
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
 * No return
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
 * No return
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
 * No return
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
 * No return
 */
extern void TM_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint16_t count);

/**
 * Callback for custom pins initialization.
 * 
 * When you call TM_SPI_Init() function, and if you pass TM_SPI_PinsPack_Custom to function,
 * then this function will be called where you can initialize custom pins for SPI peripheral.
 *
 * Parameters:
 *	- SPI_TypeDef* SPIx:
 *		SPI for which initialization will be set
 * 
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern __weak void TM_SPI_InitCustomPinsCallback(SPI_TypeDef* SPIx);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

