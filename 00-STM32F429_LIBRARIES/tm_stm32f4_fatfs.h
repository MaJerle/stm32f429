/**
 * Fatfs implementation for STM32F4xx devices
 *
 * This library uses Chan's Fatfs implementation.
 * 
 * This library is only for communication. To work with files, you have to look at Chan's FatFs manual, link below:
 * http://elm-chan.org/fsw/ff/00index_e.html
 *
 * You can work with SPI or SDIO protocol to interface SDcard.
 *
 * For more details look at link below
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/07/library-21-read-sd-card-fatfs-stm32f4xx-devices/
 *	@version 	v1.1
 *	@ide		Keil uVision
 *	
 * 
 * Library works with SPI or SDIO mode. Also, when in SDIO mode, you can set to 1- or 4-bit mode.
 * By default, SDIO with 4-bit mode is used, so you will look into right column on table below.
 * 
 * SD CARD PINS
 * 	   _________________
 * 	  / 1 2 3 4 5 6 7 8 |
 * 	 /					|
 *  / 9					|
 *  | 					|
 *	|					|
 *	|					|
 *	|					|
 *	|   SD CARD Pinout	|
 *	|					|
 *	|					|
 *	|					|
 *	|					|
 *	|___________________|
 *	
 *	
 * Default pinout for SD card
 * 
 *	NR	|SDIO INTERFACE										|SPI INTERFACE
 *		|NAME		STM32F4XX		DESCRIPTION				|NAME	STM32F4XX	DESCRIPTION
 *		|			4-BIT	1-BIT							|
 *		|													|
 *	1	|CD/DAT3	PC11	-		Connector data line 3	|CS		PB5			Chip select for SPI
 *	2	|CMD		PD2		PD2		Command/Response line	|MOSI	PA7			Data input for SPI
 *	3	|VSS1		GND		GND		GND						|VSS1	GND			GND
 *	4	|VDD		3.3V	3.3V	3.3V Power supply		|VDD	3.3V		3.3V Power supply
 *	5	|CLK		PC12	PC12	Clock					|SCK	PA5			Clock for SPI
 *	6	|VSS2		GND		GND		GND						|VSS2	GND			GND
 *	7	|DAT0		PC8		PC8		Connector data line 0	|MISO	PA6			Data output for SPI
 *	8	|DAT1		PC9		-		Connector data line 1	|-		-			-
 *	9	|DAT2		PC10	-		Connector data line 2	|-		-			-
 *	
 *	
 * SDIO Communication
 * 
 * By default, SDIO with 4-bit communication is used.
 * If you want to use SDIO 1-bit communication, set defines below in your defines.h file:
 * 
 *	//Set SDIO with 1-bit communication
 * 	#define FATFS_SDIO_4BIT		0
 * 
 * Files needed for SDIO
 * 		- tm_stm32f4_fatfs.h
 *		- tm_stm32f4_fatfs.c
 *		- fatfs/diskio.h
 *		- fatfs/diskio.c
 *		- fatfs/ff.h
 *		- fatfs/ff.c
 *		- fatfs/ffconf.h
 *		- fatfs/integer.h
 *		- fatfs/option/syscall.c
 *		- fatfs/option/unicode.c
 *		- fatfs/drivers/fatfs_sd_sdio.h
 *		- fatfs/drivers/fatfs_sd_sdio.c
 * 
 * SPI Communication
 * 
 * Or, if you want to use SPI communication, you have to add lines below in your defines.h file
 *	
 *	//Enable SPI communication, disable SDIO
 *	#define FATFS_USE_SDIO		0
 *	
 * Files, needed for SPI
 * 		- tm_stm32f4_fatfs.h
 *		- tm_stm32f4_fatfs.c
 *		- fatfs/diskio.h
 *		- fatfs/diskio.c
 *		- fatfs/ff.h
 *		- fatfs/ff.c
 *		- fatfs/ffconf.h
 *		- fatfs/integer.h
 *		- fatfs/option/syscall.c
 *		- fatfs/option/unicode.c
 *		- fatfs/drivers/fatfs_sd.h
 *		- fatfs/drivers/fatfs_sd.c
 * 
 * Overwriting default pinout:
 * 
 * SDIO interface pins are fixed, and can not be changed.
 * If you want to change SPI pins, you have to set this defines in your defines.h file
 * 
 *	//Set your SPI, for corresponsing pins look at TM SPI
 *	#define FATFS_SPI							SPI1
 *	#define FATFS_SPI_PINSPACK					TM_SPI_PinsPack_1
 *	
 *	//Set your CS pin for SPI
 *	#define FATFS_CS_RCC						RCC_AHB1Periph_GPIOB			
 *	#define FATFS_CS_PORT						GPIOB
 *	#define FATFS_CS_PIN						GPIO_Pin_5
 *
 *
 * Also, library has support for Write protect and Card detect pins.
 * This two pins are by default on pins below.
 * They are the same for any communication used, and are disabled by default
 * WP and CD Pinout
 * 
 * 	NAME	STM32F4XX	DESCRIPTION
 * 	
 *	WP		PB7			Write protect pin. Pin low when write is enabled
 *	CD		PB6			Card detect pin. Pin low when card detected
 *	
 * Like I said before, these 2 pins are disabled by default. If you want to use it, you have to add 2 lines in your defines.h file
 *	
 *	//Enable Card detect pin
 *	#define FATFS_USE_DETECT_PIN				1
 *
 *	//Enable Write protect pin
 *	#define FATFS_USE_WRITEPROTECT_PIN			1
 *	
 *	Add lines below to your defines.h file only if you want to overwrite default pin settings
 *	//Default CD pin
 *	#define FATFS_USE_DETECT_PIN_RCC			RCC_AHB1Periph_GPIOB			
 *	#define FATFS_USE_DETECT_PIN_PORT			GPIOB
 *	#define FATFS_USE_DETECT_PIN_PIN			GPIO_Pin_6
 *	//Default WP pin
 *	#define FATFS_USE_WRITEPROTECT_PIN_RCC		RCC_AHB1Periph_GPIOB			
 *	#define FATFS_USE_WRITEPROTECT_PIN_PORT		GPIOB
 *	#define FATFS_USE_WRITEPROTECT_PIN_PIN		GPIO_Pin_7
 *
 * 
 * get_fattime() time function for FatFs
 * 
 * FatFs uses function for time, to set timestamp for files, when they were edited or created.
 * 
 * By default, function returns 0, but if you want to create your own function, you have to set defines in defines.h file:
 * 
 * 	//Use custom get_fattime() function
 * 	#define FATFS_CUSTOM_FATTIME	1
 *
 * And then somewhere in your project, add function like below:
 * 
 * 	//Use custom get_fattime function
 * 	//Implement RTC get time here if you need it
 * 	DWORD get_fattime (void) {
 * 		//Get your time from RTC or something like that
 * 		
 * 		return	  ((DWORD)(2014 - 1980) << 25)	// Year 2014
 * 				| ((DWORD)7 << 21)				// Month 7
 * 				| ((DWORD)10 << 16)				// Mday 10
 * 				| ((DWORD)16 << 11)				// Hour 16
 * 				| ((DWORD)0 << 5)				// Min 0
 * 				| ((DWORD)0 >> 1);				// Sec 0
 * 	}
 *	
 */
#ifndef TM_FATFS_H
#define TM_FATFS_H	110
/**
 * Dependencies
 *	- STM32F4xx
 *	- STM32F4xx RCC
 *	- STM32F4xx GPIO
 *	- STM32Fx44 SPI		(only when SPI)
 *	- STM32F4xx DMA		(only when SDIO)
 *	- STM32Fx44 SDIO	(only when SDIO)
 *	- MISC				(only when SDIO)
 *	- defines.h
 *	- tm_stm32f4_spi	(only when SPI)
 *	- tm_stm32f4_delay	(only when SPI)
 *	- FatFS by chan
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "ff.h"

#if FATFS_USE_SDIO == 1
//If you use SDIO
#include "fatfs_sd_sdio.h"
#else
//If you use SPI
#include "fatfs_sd.h"
#endif

/**
 * Get drive size
 *
 * Parameters:
 * 	- uint32_t* total: pointer to variable to store total size of card
 * 	- uint32_t* free: pointer to variable to store free space on card
 *
 * Returns FRESULT struct members. If data are valid, FR_OK is returned.
 */
FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free);

#endif

