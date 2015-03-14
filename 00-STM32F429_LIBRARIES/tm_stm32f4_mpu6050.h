/**
 * 	MPU6050 library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-43-mpu-6050-6-axes-gyro-accelerometer-stm32f4/
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
 * Default pinout
 * 
 * MPU6050		STM32F4xx	Descrption
 * 
 *  SCL			PA8			Clock line for I2C
 *  SDA			PC9			Data line for I2C
 *	VCC			3.3V
 *	GND			GND
 *	AD0			-			If pin is low, address is 0xD0, if pin is high, the address is 0xD2
 */
#ifndef TM_MPU6050_H
#define TM_MPU6050_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx I2C
 * - defines.h
 * - TM I2C
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "defines.h"
#include "tm_stm32f4_i2c.h"

/* Default I2C used */
#ifndef MPU6050_I2C
#define	MPU6050_I2C					I2C3
#define MPU6050_I2C_PINSPACK		TM_I2C_PinsPack_1
#endif

/* Default I2C clock */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK			400000
#endif

/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

/* Who I am register value */
#define MPU6050_I_AM				0x68

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75

/* Gyro sensitivities in °/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Acce sensitivities in g */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)

/**
 * MPU6050 can have 2 different slave addresses, depends on it's input AD0 pin
 * This feature allows you to use 2 different sensors with this library at the same time
 * 
 * Parameters:
 * 	- TM_MPU6050_Device_0:
 * 		AD0 pin is set to low
 * 	- TM_MPU6050_Device_1:
 * 		AD0 pin is set to high
 */
typedef enum {
	TM_MPU6050_Device_0 = 0,
	TM_MPU6050_Device_1 = 0x02
} TM_MPU6050_Device_t;

/**
 * Result enumeration
 * 
 * ParameterS:
 * 	- TM_MPU6050_Result_Ok:
 * 		Everything OK
 * 	- TM_MPU6050_Result_DeviceNotConnected:
 * 		There is no device with valid slave address
 * 	- TM_MPU6050_Result_DeviceInvalid:
 * 		Connected device with address is not MPU6050
 */
typedef enum {
	TM_MPU6050_Result_Ok = 0x00,
	TM_MPU6050_Result_DeviceNotConnected,
	TM_MPU6050_Result_DeviceInvalid
} TM_MPU6050_Result_t;

/**
 * Set parameters for accelerometer range
 * 
 * Parameters:
 * 	- TM_MPU6050_Accelerometer_2G:
 * 		Range is +- 2G
 * 	- TM_MPU6050_Accelerometer_4G:
 * 		Range is +- 4G
 * 	- TM_MPU6050_Accelerometer_8G:
 * 		Range is +- 8G
 * 	- TM_MPU6050_Accelerometer_16G:
 * 		Range is +- 16G
 */
typedef enum {
	TM_MPU6050_Accelerometer_2G = 0x00,
	TM_MPU6050_Accelerometer_4G = 0x01,
	TM_MPU6050_Accelerometer_8G = 0x02,
	TM_MPU6050_Accelerometer_16G = 0x03
} TM_MPU6050_Accelerometer_t;

/**
 * Set parameters for gyroscope range
 * 
 * Parameters:
 * 	- TM_MPU6050_Gyroscope_250s:
 * 		Range is +- 250°/s
 * 	- TM_MPU6050_Gyroscope_500s:
 * 		Range is +- 500°/s
 * 	- TM_MPU6050_Gyroscope_1000s:
 * 		Range is +- 1000°/s
 * 	- TM_MPU6050_Gyroscope_2000s:
 * 		Range is +- 20000°/s
 */
typedef enum {
	TM_MPU6050_Gyroscope_250s = 0x00,
	TM_MPU6050_Gyroscope_500s = 0x01,
	TM_MPU6050_Gyroscope_1000s = 0x02,
	TM_MPU6050_Gyroscope_2000s = 0x03
} TM_MPU6050_Gyroscope_t;

/**
 * Main MPU6050 struct
 * 
 * Parameters:
 * 	- uint8_t Address:
 * 		I2C address of device
 * 		Only for private use
 * 	- float Gyro_Mult:
 * 		Gyroscope corrector from raw data to "degrees/s"
 *		Only for private use
 * 	- float Acce_Mult:
 * 		Accelerometer corrector from raw data to "g"
 *		Only for private use
 *
 * 	- int16_t Accelerometer_X:
 * 		Accelerometer value X axis
 * 	- int16_t Accelerometer_Y:
 * 		Accelerometer value Y axis
 * 	- int16_t Accelerometer_Z:
 * 		Accelerometer value Z axis
 * 	- int16_t Gyroscope_X:
 * 		Gyroscope value X axis
 * 	- int16_t Gyroscope_Y:
 * 		Gyroscope value Y axis
 * 	- int16_t Gyroscope_Z:
 * 		Gyroscope value Z axis
 * 	- float Temperature:
 * 		Temperature in degrees
 */
typedef struct {
	/* Private */
	uint8_t Address;
	float Gyro_Mult;
	float Acce_Mult;
	/* Public */
	int16_t Accelerometer_X;
	int16_t Accelerometer_Y;
	int16_t Accelerometer_Z;
	int16_t Gyroscope_X;
	int16_t Gyroscope_Y;
	int16_t Gyroscope_Z;
	float Temperature;
} TM_MPU6050_t;


/**
 * Initialize MPU6050
 * 
 * Parameters:
 * 	- TM_MPU6050_t* DataStruct:
 * 		Pointer to TM_MPU6050_t empty working struct
 * 	- TM_MPU6050_Device_t DeviceNumber:
 * 		Device number. MPU6050 has one pin, AD0 which can be used to set address of device.
 * 		This feature allows you to use 2 different sensors on the same board with same library.
 * 		If you set AD0 pin to low, then this parameter should be TM_MPU6050_Device_0,
 * 		but if AD0 pin is high, then you should use TM_MPU6050_Device_1
 * 	- TM_MPU6050_Accelerometer_t AccelerometerSensitivity:
 * 		Set accelerometer sensitivity
 * 	- TM_MPU6050_Gyroscope_t GyroscopeSensitivity:
 * 		Set gyroscope sensitivity
 * 
 * Returns TM_MPU6050_Result_Ok if everything is OK
 */
extern TM_MPU6050_Result_t TM_MPU6050_Init(TM_MPU6050_t* DataStruct, TM_MPU6050_Device_t DeviceNumber, TM_MPU6050_Accelerometer_t AccelerometerSensitivity, TM_MPU6050_Gyroscope_t GyroscopeSensitivity);

/**
 * Read accelerometer data from sensor
 * 
 * Parameters:
 * 	- TM_MPU6050_t* DataStruct:
 * 		Pointer to TM_MPU6050_t struct to store accelerometer data
 * 
 * Returns TM_MPU6050_Result_Ok if everything is OK
 */
extern TM_MPU6050_Result_t TM_MPU6050_ReadAccelerometer(TM_MPU6050_t* DataStruct);

/**
 * Read gyroscope data from sensor
 * 
 * Parameters:
 * 	- TM_MPU6050_t* DataStruct:
 * 		Pointer to TM_MPU6050_t struct to store gyroscope data
 * 
 * Returns TM_MPU6050_Result_Ok if everything is OK
 */
extern TM_MPU6050_Result_t TM_MPU6050_ReadGyroscope(TM_MPU6050_t* DataStruct);

/**
 * Read temperature data from sensor
 * 
 * Parameters:
 * 	- TM_MPU6050_t* DataStruct:
 * 		Pointer to TM_MPU6050_t struct to store temperature data
 * 
 * Returns TM_MPU6050_Result_Ok if everything is OK
 */
extern TM_MPU6050_Result_t TM_MPU6050_ReadTemperature(TM_MPU6050_t* DataStruct);

/**
 * Read accelerometer, gyrscope and temperature data from sensor
 * 
 * Parameters:
 * 	- TM_MPU6050_t* DataStruct:
 * 		Pointer to TM_MPU6050_t struct to store data
 * 
 * Returns TM_MPU6050_Result_Ok if everything is OK
 */
extern TM_MPU6050_Result_t TM_MPU6050_ReadAll(TM_MPU6050_t* DataStruct);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
