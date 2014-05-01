/**
 *	I2C library for STM32F429
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@link		http://bit.ly/1o4TS4q
 *	@version 	v1.0
 */
#ifndef TM_I2C_H
#define TM_I2C_H 100

/**
 * I2C pins
 *
 * I2Cx		Pinspack1		Pinspack2
 * 			SCL		SDA		SCL		SDA
 * I2C1		PB6		PB7		PB8		PB9
 * I2C2		PB10	PB11	PF1		PF0
 * I2C3		PA8		PC9		PH7		PH8
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
#define TM_I2C_TIMEOUT		20000
#endif

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
extern void TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint8_t count);

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
extern void TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint8_t count);

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
extern void TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);

/**
 * Stop condition
 *
 * Parameters:
 * 	- I2C_TypeDef* I2Cx: I2C used
 *
 * Private use
 */
extern void TM_I2C_Stop(I2C_TypeDef* I2Cx);

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

