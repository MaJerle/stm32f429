/**
 *	I2C library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/library-09-i2c-for-stm32f4xx/
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
 *	- 08.09.2014
 *	- Added support to check if device is connected to I2C bus
 *
 *
 *	Pinout on STM32F4 devices
 *	
 *			|PINSPACK 1		|PINSPACK 2	
 *	I2CX	|SCL	SDA		|SCL	SDA
 *			|				|
 *	I2C1	|PB6	PB7		|PB8	PB9
 *	I2C2	|PB10	PB11	|PF1	PF0
 *	I2C3	|PA8	PC9		|PH7	PH8
 *	
 *	Possible changes in your defines.h file:
 *	Change x to your I2C used, 1-3
 *	
 *	//By default library support only 7bit long address
 *	#define TM_I2Cx_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
 *	//Library supports I2C mode
 *	#define TM_I2Cx_MODE					I2C_Mode_I2C
 *	//Own address, if slave mode
 *	#define TM_I2Cx_OWN_ADDRESS				0x00
 *	//By default, disable ack
 *	#define TM_I2Cx_ACK						I2C_Ack_Disable
 *	//Duty cycle 2, 50%
 *	#define TM_I2Cx_DUTY_CYCLE				I2C_DutyCycle_2
 *
 */
#ifndef TM_I2C_H
#define TM_I2C_H 110
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx I2C
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "defines.h"

// Timeout
#ifndef TM_I2C_TIMEOUT
#define TM_I2C_TIMEOUT					20000
#endif

//I2C1 settings, change them in defines.h project file
#ifndef TM_I2C1_ACKNOWLEDGED_ADDRESS
#define TM_I2C1_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#endif
#ifndef TM_I2C1_MODE
#define TM_I2C1_MODE					I2C_Mode_I2C
#endif
#ifndef TM_I2C1_OWN_ADDRESS
#define TM_I2C1_OWN_ADDRESS				0x00
#endif
#ifndef TM_I2C1_ACK
#define TM_I2C1_ACK						I2C_Ack_Disable
#endif
#ifndef TM_I2C1_DUTY_CYCLE
#define TM_I2C1_DUTY_CYCLE				I2C_DutyCycle_2
#endif

//I2C2 settings, change them in defines.h project file
#ifndef TM_I2C2_ACKNOWLEDGED_ADDRESS
#define TM_I2C2_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#endif
#ifndef TM_I2C2_MODE
#define TM_I2C2_MODE					I2C_Mode_I2C
#endif
#ifndef TM_I2C2_OWN_ADDRESS
#define TM_I2C2_OWN_ADDRESS				0x00
#endif
#ifndef TM_I2C2_ACK
#define TM_I2C2_ACK						I2C_Ack_Disable
#endif
#ifndef TM_I2C2_DUTY_CYCLE
#define TM_I2C2_DUTY_CYCLE				I2C_DutyCycle_2
#endif

//I2C3 settings, change them in defines.h project file
#ifndef TM_I2C3_ACKNOWLEDGED_ADDRESS
#define TM_I2C3_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#endif
#ifndef TM_I2C3_MODE
#define TM_I2C3_MODE					I2C_Mode_I2C
#endif
#ifndef TM_I2C3_OWN_ADDRESS
#define TM_I2C3_OWN_ADDRESS				0x00
#endif
#ifndef TM_I2C3_ACK
#define TM_I2C3_ACK						I2C_Ack_Disable
#endif
#ifndef TM_I2C3_DUTY_CYCLE
#define TM_I2C3_DUTY_CYCLE				I2C_DutyCycle_2
#endif

//I2C speed modes
#define TM_I2C_CLOCK_STANDARD					100000
#define TM_I2C_CLOCK_FAST_MODE				400000
#define TM_I2C_CLOCK_FAST_MODE_PLUS		1000000
#define TM_I2C_CLOCK_HIGH_SPEED				3400000

/**
 * Pins used
 */
typedef enum {
	TM_I2C_PinsPack_1,
	TM_I2C_PinsPack_2
} TM_I2C_PinsPack_t;

/**
 * Initialize I2C
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 		I2C1, I2C2, I2C3
 * 	- TM_I2C_PinsPack_t pinspack: Pins used
 * 		- TM_I2C_Pinspack_1:
 * 			- I2C1: SCL: PB6, SDA: PB7
 * 			- I2C2: SCL: PB10, SDA: PB11
 * 			- I2C3: SCL: PA8, SDA: PC9
 * 		- TM_I2C_Pinspack_2:
 * 			- I2C1: SCL: PB8, SDA: PB9
 * 			- I2C2: SCL: PF1, SDA: PF0
 * 			- I2C3: SCL: PH7, SDA: PH8
 */
extern void TM_I2C_Init(I2C_TypeDef* I2Cx, TM_I2C_PinsPack_t pinspack, uint32_t clockSpeed);

#ifdef I2C1
/**
 * Initialize I2C1
 *
 * Parameters:
 * 	- TM_I2C_PinsPack_t pinspack
 * 		- TM_I2C_PinsPack_1:
 * 			- SCL: PB6
 * 			- SDA: PB7
 * 		- TM_I2C_PinsPack_2:
 * 			- SCL: PB8
 * 			- SDA: PB9
 */
extern void TM_I2C1_InitPins(TM_I2C_PinsPack_t pinspack);
#endif

#ifdef I2C2
/**
 * Initialize I2C2
 *
 * Parameters:
 * 	- TM_I2C_PinsPack_t pinspack
 * 		- TM_I2C_PinsPack_1:
 * 			- SCL: PB10
 * 			- SDA: PB11
 * 		- TM_I2C_PinsPack_2:
 * 			- SCL: PF1
 * 			- SDA: PF0
 */
extern void TM_I2C2_InitPins(TM_I2C_PinsPack_t pinspack);
#endif

#ifdef I2C3
/**
 * Initialize I2C3
 *
 * Parameters:
 * 	- TM_I2C_PinsPack_t pinspack
 * 		- TM_I2C_PinsPack_1:
 * 			- SCL: PA8
 * 			- SDA: PC9
 * 		- TM_I2C_PinsPack_2:
 * 			- SCL: PH7
 * 			- SDA: PH8
 */
extern void TM_I2C3_InitPins(TM_I2C_PinsPack_t pinspack);
#endif

/**
 * Read single byte from slave
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 *	- uint8_t reg: register to read from
 *
 * Data from slave returned
 */
extern uint8_t TM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);

/**
 * Read single byte from slave
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 *	- uint8_t reg: register to write to
 *	- uint8_t data: data to be written
 */
extern void TM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);

/**
 * Read multi bytes from slave
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 *	- uint8_t reg: register to read from
 *	- uint8_t *data: pointer to data array to store data from slave
 *	- uint8_t count: how many bytes will be read
 */
extern void TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * Write multi bytes from slave
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 *	- uint8_t reg: register to write to
 *	- uint8_t *data: pointer to data array to write it to slave
 *	- uint8_t count: how many bytes will be written
 */
extern void TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * Checks if device is connected to I2C bus
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * 
 * Returns 1 if device is connected, or 0 if not
 */
extern uint8_t TM_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * I2C Start condition
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t address: slave address
 *	- uint8_t direction: master to slave or slave to master
 *	- uint8_t ack: ack enabled or disabled
 *
 * Private use
 */
extern int16_t TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);

/**
 * Stop condition
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 *
 * Private use
 */
extern uint8_t TM_I2C_Stop(I2C_TypeDef* I2Cx);

/**
 * Read byte without ack
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 *
 * Return: data read
 *
 * Private use
 */
extern uint8_t TM_I2C_ReadNack(I2C_TypeDef* I2Cx);

/**
 * Read byte with ack
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 *
 * Return: data read
 *
 * Private use
 */
extern uint8_t TM_I2C_ReadAck(I2C_TypeDef* I2Cx);

/**
 * Write to slave
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 * 	- uint8_t data: data to be sent
 *
 * Private use
 */
extern void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);

#endif

