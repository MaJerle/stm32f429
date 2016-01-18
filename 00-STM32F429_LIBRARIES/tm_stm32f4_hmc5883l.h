/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library template 
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_HMC5883L_H
#define TM_HMC5883L_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_HMC5883L
 * @brief    Library description here
 * @{
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_exti.h"

/**
 * @defgroup TM_HMC5883L_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @defgroup TM_HMC5883L_I2C_DRDY
 * @brief    I2C and data ready settings
 * @{
 */

/* Default I2C for sensor */
#ifndef HMC5883L_I2C
#define HMC5883L_I2C            I2C3
#define HMC5883L_I2C_PINSPACK   TM_I2C_PinsPack_1
#endif

/* I2C clock speed */
#ifndef HMC5883L_I2C_CLOCK
#define HMC5883L_I2C_CLOCK      400000
#endif

/* I2C address for HMC5883L */
#define HMC5883L_I2C_ADDR       0x3C

/* Data ready for HMC5883L */
#ifndef HMC5883L_DRDY_PIN
#define HMC5883L_DRDY_PORT      GPIOC
#define HMC5883L_DRDY_PIN       GPIO_PIN_3
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_HMC5883L_Registers
 * @brief    Register settings for HMC5883L
 * @{
 */

#define HMC5883L_CONFA          0x00 /*!< Configuration Register A */
#define HMC5883L_CONFB          0x01 /*!< Configuration Register B */
#define HMC5883L_MODE           0x02 /*!< Mode Register */
#define HMC5883L_DATAXM         0x03 /*!< Data Output X MSB Register */
#define HMC5883L_DATAXL         0x04 /*!< Data Output X LSB Register */
#define HMC5883L_DATAZM         0x05 /*!< Data Output Z MSB Register */
#define HMC5883L_DATAZL         0x06 /*!< Data Output Z LSB Register */
#define HMC5883L_DATAYM         0x07 /*!< Data Output Y MSB Register */
#define HMC5883L_DATAYL         0x08 /*!< Data Output Y MSB Register */
#define HMC5883L_STATUS         0x09 /*!< Status register */
#define HMC5883L_IDA            0x0A /*!< Identification Register A */
#define HMC5883L_IDB            0x0B /*!< Identification Register B */
#define HMC5883L_IDC            0x0C /*!< Identification Register C */

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @defgroup TM_HMC5883L_Typedefs
 * @brief    Library Typedefs
 * @{
 */

typedef enum {
	TM_HMC5883L_Result_Ok = 0x00,
	TM_HMC5883L_Result_Error,
	TM_HMC5883L_Result_DeviceNotConnected
} TM_HMC5883L_Result_t;

typedef enum {
	TM_HMC5883L_Gain_0_88 = 0x00,
	TM_HMC5883L_Gain_1_3 = 0x20,
	TM_HMC5883L_Gain_1_9 = 0x40,
	TM_HMC5883L_Gain_2_5 = 0x60,
	TM_HMC5883L_Gain_4_0 = 0x80,
	TM_HMC5883L_Gain_4_7 = 0xA0,
	TM_HMC5883L_Gain_5_6 = 0xC0,
	TM_HMC5883L_Gain_8_1 = 0xE0
} TM_HMC5883L_Gain_t;

typedef enum {
	TM_HMC5883L_OutputRate_0_75Hz = 0x00,
	TM_HMC5883L_OutputRate_1_5Hz = 0x04,
	TM_HMC5883L_OutputRate_3Hz = 0x08,
	TM_HMC5883L_OutputRate_7_5Hz = 0x0C,
	TM_HMC5883L_OutputRate_15Hz = 0x10,
	TM_HMC5883L_OutputRate_30Hz = 0x14,
	TM_HMC5883L_OutputRate_75Hz = 0x18
} TM_HMC5883L_OutputRate_t;

typedef struct {
	uint8_t ID[3];
	float X;
	float Y;
	float Z;
} TM_HMC5883L_t;

/**
 * @}
 */

/**
 * @defgroup TM_HMC5883L_Functions
 * @brief    Library Functions
 * @{
 */

TM_HMC5883L_Result_t TM_HMC5883L_Init(TM_HMC5883L_t* HMC5883L, TM_HMC5883L_Gain_t Gain, TM_HMC5883L_OutputRate_t OutputRate);

TM_HMC5883L_Result_t TM_HMC5883L_DataReady(TM_HMC5883L_t* HMC5883L);

TM_HMC5883L_Result_t TM_HMC5883L_Read(TM_HMC5883L_t* HMC5883L);

TM_HMC5883L_Result_t TM_HMC5883L_ProcessInterrupt(TM_HMC5883L_t* HMC5883L);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
