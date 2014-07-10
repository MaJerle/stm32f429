/**
 * Fatfs implementation for STM32F4xx devices
 *
 * This library uses Chan's Fatfs implementation.
 *
 * You can work with SPI or SDIO protocol to interface SDcard.
 *
 * For more details look at http://bit.ly/1k9rJGo
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://bit.ly/1k9rJGo
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_FATFS_
#define TM_FATFS_	100
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

