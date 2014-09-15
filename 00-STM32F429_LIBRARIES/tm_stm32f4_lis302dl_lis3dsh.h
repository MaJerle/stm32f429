/**
 *	LIS302DL and LIS3DSH library for STM32F4-Discovery board
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-35-lis302dl-or-lis3dsh-accelerometer
 *	@version 	v1.0
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
 * This library supports both devices. I connect both devices, because 
 * old F4-Discovery board has LIS302DL accelerometer,
 * new F4-Discovery board has LIS3DSH acelerometer. New accelerometer
 * has some more features for sensitivity.
 * 
 * Default pinout on F4-Discovery board
 * 
 * 	LIS		STM32F4		Description
 * 
 * 	MOSI	PA7			Master out, Slave in for SPI1
 * 	MISO	PA6			Master in, Slave out for SPI1
 * 	SCK		PA5			Serial clock for SPI1
 * 	CS		PE3			Chip select for LIS
 * 
 * Change default pinout. Open defines.h file and copy lines below and edit them
 * 	
 * 	//Select custom SPI
 * 	//#define LIS302DL_LIS3DSH_SPI				SPI1
 * 	//#define LIS302DL_LIS3DSH_SPI_PINSPACK		TM_SPI_PinsPack_1
 * 	
 * 	//Select custom CS pin for SPI
 * 	//#define LIS302DL_LIS3DSH_CS_RCC			RCC_AHB1Periph_GPIOE
 * 	//#define LIS302DL_LIS3DSH_CS_PORT			GPIOE
 * 	//#define LIS302DL_LIS3DSH_CS_PIN			GPIO_Pin_3
 * 
 */
#ifndef TM_LIS302DL_LIS3DSH_H
#define TM_LIS302DL_LIS3DSH_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM SPI
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_spi.h"

/* SPI on STM32F4-Discovery board */
#ifndef LIS302DL_LIS3DSH_SPI
#define LIS302DL_LIS3DSH_SPI				SPI1
#define LIS302DL_LIS3DSH_SPI_PINSPACK		TM_SPI_PinsPack_1
#endif

/* CS pin on STM32F4-Discovery board */
#ifndef LIS302DL_LIS3DSH_CS_PIN
#define LIS302DL_LIS3DSH_CS_RCC				RCC_AHB1Periph_GPIOE
#define LIS302DL_LIS3DSH_CS_PORT			GPIOE
#define LIS302DL_LIS3DSH_CS_PIN				GPIO_Pin_3
#endif

/* CS pin settings */
#define LIS302DL_LIS3DSH_CS_LOW				LIS302DL_LIS3DSH_CS_PORT->BSRRH = LIS302DL_LIS3DSH_CS_PIN
#define LIS302DL_LIS3DSH_CS_HIGH			LIS302DL_LIS3DSH_CS_PORT->BSRRL = LIS302DL_LIS3DSH_CS_PIN

/* Who I am values */
#define LIS302DL_ID							0x3B
#define LIS3DSH_ID							0x3F

/* Common registers */
#define LIS302DL_LIS3DSH_REG_WHO_I_AM		0x0F

/* ----------------------------------------- */
/* LIS3DSH registers */
/* ----------------------------------------- */
#define LIS3DSH_WHO_AM_I_ADDR				0x0F
#define LIS3DSH_CTRL_REG4_ADDR				0x20
#define LIS3DSH_CTRL_REG1_ADDR				0x21
#define LIS3DSH_CTRL_REG2_ADDR				0x22
#define LIS3DSH_CTRL_REG3_ADDR				0x23
#define LIS3DSH_CTRL_REG5_ADDR				0x24
#define LIS3DSH_CTRL_REG6_ADDR				0x25
#define LIS3DSH_OUT_X_L_ADDR				0x28
#define LIS3DSH_OUT_X_H_ADDR				0x29
#define LIS3DSH_OUT_Y_L_ADDR				0x2A
#define LIS3DSH_OUT_Y_H_ADDR				0x2B
#define LIS3DSH_OUT_Z_L_ADDR				0x2C
#define LIS3DSH_OUT_Z_H_ADDR				0x2D

#define LIS3DSH_SENSITIVITY_0_06G            0.06  /* 0.06 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_12G            0.12  /* 0.12 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_18G            0.18  /* 0.18 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_24G            0.24  /* 0.24 mg/digit*/
#define LIS3DSH_SENSITIVITY_0_73G            0.73  /* 0.73 mg/digit*/

#define LIS3DSH_DATARATE_100				((uint8_t)0x60)

#define LIS3DSH_FULLSCALE_2					((uint8_t)0x00)  /* 2 g  */
#define LIS3DSH_FULLSCALE_4					((uint8_t)0x08)  /* 4 g  */
#define LIS3DSH_FULLSCALE_6					((uint8_t)0x10)  /* 6 g  */
#define LIS3DSH_FULLSCALE_8					((uint8_t)0x18)  /* 8 g  */
#define LIS3DSH_FULLSCALE_16				((uint8_t)0x20)  /* 16 g */
#define LIS3DSH__FULLSCALE_SELECTION		((uint8_t)0x38)

#define LIS3DSH_FILTER_BW_800				((uint8_t)0x00)  /* 800 Hz */
#define LIS3DSH_FILTER_BW_400				((uint8_t)0x40)//((uint8_t)0x08) /* 400 Hz  */
#define LIS3DSH_FILTER_BW_200				((uint8_t)0x80)//((uint8_t)0x10)  /* 200 Hz */
#define LIS3DSH_FILTER_BW_50				((uint8_t)(0x80 | 0x40))//((uint8_t)0x18)  /* 50 Hz  */
#define LIS3DSH_SELFTEST_NORMAL				((uint8_t)0x00)
#define LIS3DSH_XYZ_ENABLE					((uint8_t)0x07)
#define LIS3DSH_SERIALINTERFACE_4WIRE		((uint8_t)0x00)
#define LIS3DSH_SM_ENABLE					((uint8_t)0x01)
#define LIS3DSH_SM_DISABLE					((uint8_t)0x00)

/* ----------------------------------------- */
/* LIS302DL registers                        */
/* ----------------------------------------- */
#define LIS302DL_CTRL_REG1_ADDR							0x20
#define LIS302DL_CTRL_REG2_ADDR							0x21
#define LIS302DL_CTRL_REG3_ADDR							0x22
#define LIS302DL_OUT_X_ADDR								0x29
#define LIS302DL_OUT_Y_ADDR								0x2B
#define LIS302DL_OUT_Z_ADDR								0x2D

#define LIS302DL_SENSITIVITY_2_3G						18  /* 18 mg/digit*/
#define LIS302DL_SENSITIVITY_9_2G						72  /* 72 mg/digit*/

#define LIS302DL_DATARATE_100							((uint8_t)0x00)
#define LIS302DL_DATARATE_400							((uint8_t)0x80)

#define LIS302DL_LOWPOWERMODE_ACTIVE					((uint8_t)0x40)
#define LIS302DL_FULLSCALE_2_3							((uint8_t)0x00)
#define LIS302DL_FULLSCALE_9_2							((uint8_t)0x20)
#define LIS302DL_SELFTEST_NORMAL						((uint8_t)0x00)
#define LIS302DL_XYZ_ENABLE								((uint8_t)0x07)
#define LIS302DL_SERIALINTERFACE_4WIRE					((uint8_t)0x00)
#define LIS302DL_BOOT_NORMALMODE						((uint8_t)0x00)
#define LIS302DL_BOOT_REBOOTMEMORY						((uint8_t)0x40)
#define LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER	((uint8_t)0x20)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_OFF			((uint8_t)0x00)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1				((uint8_t)0x04)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_2				((uint8_t)0x08)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1_2			((uint8_t)0x0C)
#define LIS302DL_HIGHPASSFILTER_LEVEL_0					((uint8_t)0x00)
#define LIS302DL_HIGHPASSFILTER_LEVEL_1					((uint8_t)0x01)
#define LIS302DL_HIGHPASSFILTER_LEVEL_2					((uint8_t)0x02)
#define LIS302DL_HIGHPASSFILTER_LEVEL_3					((uint8_t)0x03)

/**
 * Typedef enumeration for returning from functions
 * 
 * Parameters:
 * 	- TM_LIS302DL_LIS3DSH_Device_Error:
 * 		Returned from function when device is not recognized
 * 	- TM_LIS302DL_LIS3DSH_Device_LIS302DL:
 * 		Returned from functions when device is LIS302DL on old boards
 * 	- TM_LIS302DL_LIS3DSH_Device_LIS3DSH:
 * 		Returned from functions when device is LIS3DSH on new boards
 */
typedef enum {
	TM_LIS302DL_LIS3DSH_Device_Error,
	TM_LIS302DL_LIS3DSH_Device_LIS302DL,
	TM_LIS302DL_LIS3DSH_Device_LIS3DSH
} TM_LIS302DL_LIS3DSH_Device_t;

/**
 * Possible sensitivities for both devices
 * 
 * Parameters:
 * 	- TM_LIS3DSH_Sensitivity_2G:
 * 		2G sensitivity on LIS3DSH device
 * 	- TM_LIS3DSH_Sensitivity_4G:
 * 		4G sensitivity on LIS3DSH device
 * 	- TM_LIS3DSH_Sensitivity_6G:
 * 		6G sensitivity on LIS3DSH device
 * 	- TM_LIS3DSH_Sensitivity_8G:
 * 		8G sensitivity on LIS3DSH device
 * 	- TM_LIS3DSH_Sensitivity_16G:
 * 		16G sensitivity on LIS3DSH device
 * 
 * 	- TM_LIS302DL_Sensitivity_2_3G:
 * 		2.3G  sensitivity on LIS3DSH device
 * 	- TM_LIS302DL_Sensitivity_9_2G:
 * 		9.2G sensitivity on LIS3DSH device
 */
typedef enum {
	/* LIS3DSH */
	TM_LIS3DSH_Sensitivity_2G,
	TM_LIS3DSH_Sensitivity_4G,
	TM_LIS3DSH_Sensitivity_6G,
	TM_LIS3DSH_Sensitivity_8G,
	TM_LIS3DSH_Sensitivity_16G,
	/* LIS302DL */
	TM_LIS302DL_Sensitivity_2_3G,
	TM_LIS302DL_Sensitivity_9_2G
} TM_LIS302DL_LIS3DSH_Sensitivity_t;

/**
 * Filter values for both accelerometers
 * 
 * Parameters:
 * 	- TM_LIS3DSH_Filter_800Hz:
 * 		800 Hz high pass filter on LIS3DSH
 * 	- TM_LIS3DSH_Filter_400Hz:
 * 		400 Hz high pass filter on LIS3DSH
 * 	- TM_LIS3DSH_Filter_200Hz:
 * 		200 Hz high pass filter on LIS3DSH
 * 	- TM_LIS3DSH_Filter_50Hz:
 * 		50 Hz high pass filter on LIS3DSH
 * 	
 * 	- TM_LIS302DL_Filter_2Hz:
 * 		2 Hz filter on LIS302DL
 * 	- TM_LIS302DL_Filter_1Hz:
 * 		1 Hz filter on LIS302DL
 * 	- TM_LIS302DL_Filter_500mHz:
 * 		500 mHz filter on LIS302DL
 * 	- TM_LIS302DL_Filter_250mHz
 * 		250 mHz filter on LIS302DL
 */
typedef enum {
	/* LIS3DSH */
	TM_LIS3DSH_Filter_800Hz,
	TM_LIS3DSH_Filter_400Hz,
	TM_LIS3DSH_Filter_200Hz,
	TM_LIS3DSH_Filter_50Hz,
	/* LIS302DL */
	TM_LIS302DL_Filter_2Hz,
	TM_LIS302DL_Filter_1Hz,
	TM_LIS302DL_Filter_500mHz,
	TM_LIS302DL_Filter_250mHz
} TM_LIS302DL_LIS3DSH_Filter_t;

/**
 * Data structure
 * 
 * Parameters:
 * 	- int16_t X:
 * 		X axis value
 * 	- int16_t Y:
 * 		Y axis value
 * 	- int16_t Z:
 * 		Z axis value
 */
typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
} TM_LIS302DL_LIS3DSH_t;

/**
 * Detect LIS302DL or LIS3DSH device connected on F4-Discovery board
 * 
 * Member of TM_LIS302DL_LIS3DSH_Device_t is returned
 */
extern TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_Detect(void);

/**
 * Initialize proper device
 * 
 * Parameters:
 * 	- TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity:
 * 		Select sensitivity for device. There are separate possible sensitivities for proper device
 * 	- TM_LIS302DL_LIS3DSH_Filter_t Filter:
 * 		Select filter for device. There are separate possible sensitivities for proper device
 * 
 * Member of TM_LIS302DL_LIS3DSH_Device_t is returned
 */
extern TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity, TM_LIS302DL_LIS3DSH_Filter_t Filter);

/**
 * Read axes from device
 * 
 * Parameters:
 * 	- TM_LIS302DL_LIS3DSH_t* Axes_Data
 * 		Pointer to TM_LIS302DL_LIS3DSH_t struct
 * 
 * Member of TM_LIS302DL_LIS3DSH_Device_t is returned
 */
extern TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_ReadAxes(TM_LIS302DL_LIS3DSH_t* Axes_Data);

#endif


