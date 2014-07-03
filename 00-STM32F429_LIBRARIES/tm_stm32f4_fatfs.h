/**
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_FATFS_
#define TM_FATFS_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_delay.h"
#include "ff.h"

#include <stdio.h>
#include <string.h>

#include "fatfs_sd.h"
#ifdef TM_FATFS_SD_SDIO
#include "fatfs_sd_sdio.h"
#endif


#ifndef TM_FATFS_SCANFILES_USART
#define TM_FATFS_SCANFILES_USART 	1
#include "tm_stm32f4_usart.h"
#endif

uint8_t TM_FATFS_DriveSize(uint32_t* total, uint32_t* free);

#endif

