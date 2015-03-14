/**
 *	L3GD20 library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-28-l3gd20-3-axis-gyroscope
 *	@version 	v1.1
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
 * Version 1.1
 *	- March 14, 2015
 *	- Support for new GPIO system
 *
 * L3GD20 3-axis Gyroscope is connected on STM32F429 Discovery board.
 * It works with SPI, has 3 selectable resolutions.
 *
 * By default, on board is connected to pins:
 *
 *  L3GD20			STM32F4			Description
 *
 *  MOSI			PF9				SPI5 is used by default
 *  MISO			PF8				SPI5 is used by default
 *  SCK				PF7				SPI5 is used by default
 *  CS				PC1				Chip select for SPI
 *
 * If you want to change SPI, use these two lines below in your defines.h file and edit them:
 *
 *  //Select custom SPI for sensor
 *  #define L3GD20_SPI					SPI5
 *  #define L3GD20_SPI_PINSPACK			TM_SPI_PinsPack_1
 *
 * If you want to change CS pin, add these lines below in defines.h file and edit them:
 *
 *  //Select CS pin
 * 	#define L3GD20_CS_PORT				GPIOC
 *  #define L3GD20_CS_PIN				GPIO_Pin_1
 */
#ifndef TM_L3GD20_H
#define TM_L3GD20_H	110
/**
 * Dependencies:
 * 	- STM32F4xx
 * 	- STM32F4xx RCC
 * 	- STM32F4xx GPIO
 * 	- defines.h
 * 	- TM SPI
 *	- TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_gpio.h"

/* Default SPI, used on STM32F429 Discovery board */
#ifndef L3GD20_SPI
#define L3GD20_SPI					SPI5
#define L3GD20_SPI_PINSPACK			TM_SPI_PinsPack_1
#endif

/* Default CS pin on STM32F429 Discovery board */
#ifndef L3GD20_CS_PIN
#define L3GD20_CS_PORT				GPIOC
#define L3GD20_CS_PIN				GPIO_PIN_1
#endif

/* Pin macros */
#define L3GD20_CS_LOW				TM_GPIO_SetPinLow(L3GD20_CS_PORT, L3GD20_CS_PIN)
#define L3GD20_CS_HIGH				TM_GPIO_SetPinHigh(L3GD20_CS_PORT, L3GD20_CS_PIN)

/* Identification number */
#define L3GD20_WHO_AM_I				0xD4

/* Registers addresses */
#define L3GD20_REG_WHO_AM_I			0x0F
#define L3GD20_REG_CTRL_REG1		0x20
#define L3GD20_REG_CTRL_REG2		0x21
#define L3GD20_REG_CTRL_REG3		0x22
#define L3GD20_REG_CTRL_REG4		0x23
#define L3GD20_REG_CTRL_REG5		0x24
#define L3GD20_REG_REFERENCE		0x25
#define L3GD20_REG_OUT_TEMP			0x26
#define L3GD20_REG_STATUS_REG		0x27
#define L3GD20_REG_OUT_X_L			0x28
#define L3GD20_REG_OUT_X_H			0x29
#define L3GD20_REG_OUT_Y_L			0x2A
#define L3GD20_REG_OUT_Y_H			0x2B
#define L3GD20_REG_OUT_Z_L			0x2C
#define L3GD20_REG_OUT_Z_H			0x2D
#define L3GD20_REG_FIFO_CTRL_REG	0x2E
#define L3GD20_REG_FIFO_SRC_REG		0x2F
#define L3GD20_REG_INT1_CFG			0x30
#define L3GD20_REG_INT1_SRC			0x31
#define L3GD20_REG_INT1_TSH_XH		0x32
#define L3GD20_REG_INT1_TSH_XL		0x33
#define L3GD20_REG_INT1_TSH_YH		0x34
#define L3GD20_REG_INT1_TSH_YL		0x35
#define L3GD20_REG_INT1_TSH_ZH		0x36
#define L3GD20_REG_INT1_TSH_ZL		0x37
#define L3GD20_REG_INT1_DURATION	0x38

/* Sensitivity factors, datasheet pg. 9 */
#define L3GD20_SENSITIVITY_250		8.75	/* 8.75 mdps/digit */
#define L3GD20_SENSITIVITY_500		17.5	/* 17.5 mdps/digit */
#define L3GD20_SENSITIVITY_2000		70		/* 70 mdps/digit */

/**
 * Main working struct
 *
 * Parameters:
 * 	- int16_t X
 * 		X axis rotation
 * 	- int16_t Y
 * 		Y axis rotation
 * 	- int16_t Z
 * 		Z axis rotation
 */
typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
} TM_L3GD20_t;

/**
 * Result enumerations
 *
 * Parameters:
 * 	- TM_L3GD20_Result_Ok
 * 		Everything ok
 * 	- TM_L3GD20_Result_Error
 * 		Error occured
 */
typedef enum {
	TM_L3GD20_Result_Ok,
	TM_L3GD20_Result_Error
} TM_L3GD20_Result_t;

/**
 * Enumeration for scale select
 *
 * Parameters:
 * 	- TM_L3GD20_Scale_250
 * 		Set full scale to 250 mdps
 * 	- TM_L3GD20_Scale_500
 * 		Set full scale to 500 mdps
 * 	- TM_L3GD20_Scale_2000
 * 		Set full scale to 2000 mdps
 */
typedef enum {
	TM_L3GD20_Scale_250,
	TM_L3GD20_Scale_500,
	TM_L3GD20_Scale_2000
} TM_L3GD20_Scale_t;

/**
 * Initialize L3GD20 sensor
 *
 * Parameters:
 * 	- TM_L3GD20_Scale_t scale
 * 		Working scale used
 *
 * Returns TM_L3GD20_Result_Ok is sensor detected, otherwise TM_L3GD20_Result_Error
 */
extern TM_L3GD20_Result_t TM_L3GD20_Init(TM_L3GD20_Scale_t scale);

/**
 * Read rotation data from sensor
 *
 * Parameters:
 * 	- TM_L3GD20_t* L3DG20_Data
 * 		Pointer to TM_L3GD20_t struct
 *
 * Returns TM_L3GD20_Result_Ok
 */
extern TM_L3GD20_Result_t TM_L3GD20_Read(TM_L3GD20_t* L3DG20_Data);

#endif

