/**
 *	BMP180 pressure sensor library for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-37-bmp180-pressure-sensor-stm32f4
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
 * You can read temperature and pressure with this sensor.
 * 
 * First you need to read temperature, then you are able to read pressure.
 * This is quite inefficient, because you can not read both at the same time.
 * Sensor uses the same register for data for temperature and pressure.
 * 
 * Not good, but it can be used in various small projects.
 * 
 * Default pinout:
 * 	
 * 	BMP180		STM32F4		DESCRIPTION
 * 	
 * 	SCL			PA8			I2C3 Serial clock
 * 	SDA			PC9			I2C3 Serial data
 * 
 * Make sure, that you connect VCC to 3.3V and not to 5V, because you can blow device.
 * If you want to change default I2C settings, open defines.h file and edit lines below:
 * 
 *	//Select custom I2C
 *	//#define BMP180_I2C				I2C3
 *	//#define BMP180_I2C_PINSPACK		TM_I2C_PinsPack_1
 * 	
 */
#ifndef TM_BMP180_H
#define TM_BMP180_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM I2C
 * - math.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_i2c.h"
#include "math.h"

/* Default I2C pin */
#ifndef BMP180_I2C
#define BMP180_I2C					I2C3
#define BMP180_I2C_PINSPACK			TM_I2C_PinsPack_1
#endif

/* Default I2C speed */
#ifndef BMP180_I2C_SPEED
#define BMP180_I2C_SPEED			100000
#endif

/* BMP180 I2C address */
#ifndef BMP180_I2C_ADDRESS
#define BMP180_I2C_ADDRESS			0xEE
#endif

/* Registers */
#define	BMP180_REGISTER_CONTROL 	0xF4
#define	BMP180_REGISTER_RESULT 		0xF6
#define BMP180_REGISTER_EEPROM		0xAA

/* Commands */
#define	BMP180_COMMAND_TEMPERATURE 	0x2E
#define	BMP180_COMMAND_PRESSURE_0 	0x34
#define	BMP180_COMMAND_PRESSURE_1 	0x74
#define	BMP180_COMMAND_PRESSURE_2 	0xB4
#define	BMP180_COMMAND_PRESSURE_3 	0xF4

/* Minimum waiting delay, in microseconds */
#define BMP180_TEMPERATURE_DELAY	4500
#define BMP180_PRESSURE_0_DELAY		4500
#define BMP180_PRESSURE_1_DELAY		7500
#define BMP180_PRESSURE_2_DELAY		13000
#define BMP180_PRESSURE_3_DELAY		25000

/**
 * BMP180 result typedef enumerations
 * Used for result from functions
 * 
 * Parameters:
 * 	- TM_BMP180_Result_Ok:
 * 		Everything is OK
 * 	- TM_BMP180_Result_DeviceNotConnected:
 * 		Device is not connected to I2C
 * 	- TM_BMP180_Result_LibraryNotInitialized
 * 		Library is not initialized
 */
typedef enum {
	TM_BMP180_Result_Ok = 0x00,
	TM_BMP180_Result_DeviceNotConnected,
	TM_BMP180_Result_LibraryNotInitialized
} TM_BMP180_Result_t;

/**
 * Select pressure oversampling.
 * This settings differs in samples for one result 
 * and sample time for one result
 * 
 * Parameters:
 * 	- TM_BMP180_Oversampling_UltraLowPower:
 * 		1 sample for result
 * 	- TM_BMP180_Oversampling_Standard:
 * 		2 samples for result
 * 	- TM_BMP180_Oversampling_HighResolution:
 * 		4 samples for result
 * 	- TM_BMP180_Oversampling_UltraHighResolution:
 * 		8 samples for result
 */
typedef enum {
	TM_BMP180_Oversampling_UltraLowPower = 0,
	TM_BMP180_Oversampling_Standard = 1,
	TM_BMP180_Oversampling_HighResolution = 2,
	TM_BMP180_Oversampling_UltraHighResolution = 3
} TM_BMP180_Oversampling_t;

/**
 * BMP180 working struct
 * 
 * Parameters:
 * 	- float Altitude:
 * 		Calculated altitude at given read pressure
 * 	- uint32_t Pressure:
 * 		Pressure in pascals
 * 	- float Temperature:
 * 		Temperature in degrees
 * 	- uint16_t Delay:
 * 		Number of microseconds, that sensor needs to calculate data that you request to
 * 	- TM_BMP180_Oversampling_t Oversampling:
 * 		Oversampling for pressure calculation
 */
typedef struct {
	float Altitude;
	uint32_t Pressure;
	float Temperature;
	uint16_t Delay;
	TM_BMP180_Oversampling_t Oversampling;
} TM_BMP180_t;

/**
 * Initialize BMP180 pressure sensor
 * 
 * Parameters:
 * 	- TM_BMP180_t* BMP180_Data:
 * 		Pointer to TM_BMP180_t structure
 * 
 * Member of TM_BMP180_Result_t is returned
 */
extern TM_BMP180_Result_t TM_BMP180_Init(TM_BMP180_t* BMP180_Data);

/**
 * Start temperature sensor on BMP180
 * 
 * Parameters:
 * 	- TM_BMP180_t* BMP180_Data:
 * 		Pointer to TM_BMP180_t structure
 * 
 * Member of TM_BMP180_Result_t is returned
 */
extern TM_BMP180_Result_t TM_BMP180_StartTemperature(TM_BMP180_t* BMP180_Data);

/**
 * Read temperature from BMP180 sensor
 * Temperature has 0.1°C degrees resolution
 * 
 * Parameters:
 * 	- TM_BMP180_t* BMP180_Data:
 * 		Pointer to TM_BMP180_t structure
 * 
 * Member of TM_BMP180_Result_t is returned
 */
extern TM_BMP180_Result_t TM_BMP180_ReadTemperature(TM_BMP180_t* BMP180_Data);

/**
 * Start pressure measurement on BMP180 sensor
 * 
 * Parameters:
 * 	- TM_BMP180_t* BMP180_Data:
 * 		Pointer to TM_BMP180_t structure
 * 	- TM_BMP180_Oversampling_t Oversampling:
 * 		Oversampling settings. Different settings have different measurement time
 * 
 * Member of TM_BMP180_Result_t is returned
 */
extern TM_BMP180_Result_t TM_BMP180_StartPressure(TM_BMP180_t* BMP180_Data, TM_BMP180_Oversampling_t Oversampling);

/**
 * Read pressure from BMP180 sensor and calculate it
 * 
 * Parameters:
 * 	- TM_BMP180_t* BMP180_Data:
 * 		Pointer to TM_BMP180_t structure
 * 
 * Member of TM_BMP180_Result_t is returned
 */
extern TM_BMP180_Result_t TM_BMP180_ReadPressure(TM_BMP180_t* BMP180_Data);

/**
 * Calculate pressure above sea level in pascals
 * 
 * This is good, if you read pressure from sensor at known altitude, not altitude provided from sensor.
 * Altitude from sensor is calculated in fact, that pressure above the sea is 101325 Pascals.
 * So, if you know your pressure, and you use calculated altitude, you will not get real pressure above the sea.
 * 
 * You need calculated pressure from sensor, and known altitude (from other sensor or GPS data, or whatever)
 * and then you are able to calculate pressure above the sea level.
 * 
 * Parameters:
 * 	- uint32_t pressure
 * 		Pressure at known altitude in pascals
 * 	- float altitude
 * 		Known altitude in meters. Do not use altitude from sensor, because then your result is inefficient
 * 
 * Pressure above the sea is returned in pascals unit
 */
extern uint32_t TM_BMP180_GetPressureAtSeaLevel(uint32_t pressure, float altitude);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
