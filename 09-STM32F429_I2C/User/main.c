/**
 *	Keil project for I2C peripheral
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_i2c.h"

/* Slave address */
#define ADDRESS		0xD0 // 1101 000 0 - left aligned 7-bit address

int main(void) {
	uint8_t data[] = {0, 1, 2};
	
	/* Initialize system */
	SystemInit();

	/* Initialize I2C, SCL: PB6 and SDA: PB7 with 100kHt serial clock */
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_1, 100000);

	/* Write "5" at location 0x00 to slave with address ADDRESS */
	TM_I2C_Write(I2C1, ADDRESS, 0x00, 5);
	
	/**
	 * Write multi bytes to slave with address ADDRESS
	 * Write to registers starting from 0x00, get data in variable "data" and write 3 bytes
	 */
	TM_I2C_WriteMulti(I2C1, ADDRESS, 0x00, data, 3);
	
	/* Read single byte from slave with 0xD0 (1101 000 0) address and register location 0x00 */
	data[0] = TM_I2C_Read(I2C1, ADDRESS, 0x00);
	
	/**
	 * Read 3 bytes of data from slave with 0xD0 address
	 * First register to read from is at 0x00 location 
	 * Store received data to "data" variable
	 */
	TM_I2C_ReadMulti(I2C1, 0xD0, 0x00, data, 3);
	
	while (1) {

	}
}
