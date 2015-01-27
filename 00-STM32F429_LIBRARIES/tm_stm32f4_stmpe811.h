/**
 *	STMPE811 Touch screen controller library
 * 	It can be used on STM32F429 Discovery board, or any other LCD with 240*320 pixels
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/library-10-stmpe811-touch-screen-driver-for-stm32f429-discovery-board/
 *	@version 	v1.2
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
 * Version 1.2
 *	- January 27, 2015
 *	- Added support for last pressed state
 *
 * Version 1.1
 * 	- August 22, 2014
 * 	- Fixes because on some boards my library for these touch didn't work
 *
 * Default pinout
 *
 * 	STMPE811		STM32F429 Discovery			Description
 * 
 * 	SCL				PA8							Clock pin for I2C
 * 	SDA				PC9							Data pin for I2C
 * 
 * This pinout is used on STM32F429 Discovery board.
 * If you want to change it, you can add lines below in your defines.h file
 * 
 *	//Use custom I2C settings. Check TM I2C library for corresponding pins
 *	#define STMPE811_I2C					I2C3
 *	#define STMPE811_I2C_PINSPACK			TM_I2C_PinsPack_1
 * 
 */
#ifndef TM_STMPE811_H
#define TM_STMPE811_H	120
/**
 * Library dependencies
 * * STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM I2C
 * - TM DELAY
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"

/* Default I2C used, on F429 Discovery board */
#ifndef STMPE811_I2C
#define STMPE811_I2C					I2C3
#define STMPE811_I2C_PINSPACK			TM_I2C_PinsPack_1
#endif

#ifndef STMPE811_I2C_CLOCK
#define STMPE811_I2C_CLOCK				100000
#endif

/* I2C address */
#define STMPE811_ADDRESS				0x82

/* STMPE811 Chip ID on reset */
#define STMPE811_CHIP_ID_VALUE			0x0811	//Chip ID

/* Registers */
#define STMPE811_CHIP_ID				0x00	//STMPE811 Device identification
#define STMPE811_ID_VER					0x02	//STMPE811 Revision number; 0x01 for engineering sample; 0x03 for final silicon
#define STMPE811_SYS_CTRL1				0x03	//Reset control
#define STMPE811_SYS_CTRL2				0x04	//Clock control
#define STMPE811_SPI_CFG				0x08	//SPI interface configuration
#define STMPE811_INT_CTRL				0x09	//Interrupt control register
#define STMPE811_INT_EN					0x0A	//Interrupt enable register
#define STMPE811_INT_STA				0x0B	//Interrupt status register
#define STMPE811_GPIO_EN				0x0C	//GPIO interrupt enable register
#define STMPE811_GPIO_INT_STA			0x0D	//GPIO interrupt status register
#define STMPE811_ADC_INT_EN				0x0E	//ADC interrupt enable register
#define STMPE811_ADC_INT_STA			0x0F	//ADC interface status register
#define STMPE811_GPIO_SET_PIN			0x10	//GPIO set pin register
#define STMPE811_GPIO_CLR_PIN			0x11	//GPIO clear pin register
#define STMPE811_MP_STA					0x12	//GPIO monitor pin state register
#define STMPE811_GPIO_DIR				0x13	//GPIO direction register
#define STMPE811_GPIO_ED				0x14	//GPIO edge detect register
#define STMPE811_GPIO_RE				0x15	//GPIO rising edge register
#define STMPE811_GPIO_FE				0x16	//GPIO falling edge register
#define STMPE811_GPIO_AF				0x17	//alternate function register
#define STMPE811_ADC_CTRL1				0x20	//ADC control
#define STMPE811_ADC_CTRL2				0x21	//ADC control
#define STMPE811_ADC_CAPT				0x22	//To initiate ADC data acquisition
#define STMPE811_ADC_DATA_CHO			0x30	//ADC channel 0
#define STMPE811_ADC_DATA_CH1			0x32	//ADC channel 1
#define STMPE811_ADC_DATA_CH2			0x34	//ADC channel 2
#define STMPE811_ADC_DATA_CH3			0x36	//ADC channel 3
#define STMPE811_ADC_DATA_CH4			0x38	//ADC channel 4
#define STMPE811_ADC_DATA_CH5			0x3A	//ADC channel 5
#define STMPE811_ADC_DATA_CH6			0x3C	//ADC channel 6
#define STMPE811_ADC_DATA_CH7			0x3E	//ADC channel 7
#define STMPE811_TSC_CTRL				0x40	//4-wire touchscreen controller setup
#define STMPE811_TSC_CFG				0x41	//Touchscreen controller configuration
#define STMPE811_WDW_TR_X				0x42	//Window setup for top right X
#define STMPE811_WDW_TR_Y				0x44	//Window setup for top right Y
#define STMPE811_WDW_BL_X				0x46	//Window setup for bottom left X
#define STMPE811_WDW_BL_Y				0x48	//Window setup for bottom left Y
#define STMPE811_FIFO_TH				0x4A	//FIFO level to generate interrupt
#define STMPE811_FIFO_STA				0x4B	//Current status of FIFO
#define STMPE811_FIFO_SIZE				0x4C	//Current filled level of FIFO
#define STMPE811_TSC_DATA_X				0x4D	//Data port for touchscreen controller data access
#define STMPE811_TSC_DATA_Y				0x4F	//Data port for touchscreen controller data access
#define STMPE811_TSC_DATA_Z				0x51	//Data port for touchscreen controller data access
#define STMPE811_TSC_DATA_XYZ			0x52	//Data port for touchscreen controller data access
#define STMPE811_TSC_FRACTION_Z			0x56	//Touchscreen controller FRACTION_Z
#define STMPE811_TSC_DATA				0x57	//Data port for touchscreen controller data access
#define STMPE811_TSC_I_DRIVE			0x58	//Touchscreen controller drivel
#define STMPE811_TSC_SHIELD				0x59	//Touchscreen controller shield
#define STMPE811_TEMP_CTRL				0x60	//Temperature sensor setup
#define STMPE811_TEMP_DATA				0x61	//Temperature data access port
#define STMPE811_TEMP_TH				0x62	//Threshold for temperature controlled interrupt

/**
 * Enum for set how to read x and y from controller
 */
typedef enum {
	TM_STMPE811_Orientation_Portrait_1,
	TM_STMPE811_Orientation_Portrait_2,
	TM_STMPE811_Orientation_Landscape_1,
	TM_STMPE811_Orientation_Landscape_2,
} TM_STMPE811_Orientation_t;

/**
 * Enums for touch pressed or released
 */
typedef enum {
	TM_STMPE811_State_Pressed,
	TM_STMPE811_State_Released,
	TM_STMPE811_State_Ok,
	TM_STMPE811_State_Error
} TM_STMPE811_State_t;

/**
 * Struct, which is passed into TM_STMPE811_ReadTouch function
 *
 * Parameters:
 * 	- uint16_t x: x coordinate for lcd
 * 	- uint16_t y: y coordinate for lcd
 * 	- TM_STMPE811_State_t pressed:
 * 		- TM_STMPE811_State_Pressed when pressed
 * 		- TM_STMPE811_State_Released when released
 * 	- TM_STMPE811_State_t last_pressed:
 * 		- TM_STMPE811_State_Pressed when was pressed
 * 		- TM_STMPE811_State_Released when was released
 * 	- TM_STMPE811_Orientation_t orientation: Set touch screen orientation to read
 */
typedef struct {
	uint16_t x;
	uint16_t y;
	TM_STMPE811_State_t pressed;
	TM_STMPE811_State_t last_pressed;
	TM_STMPE811_Orientation_t orientation;
} TM_STMPE811_TouchData;

/**
 * Initialize STMPE811 Touch driver
 *
 * Returns 0 on success
 */
extern TM_STMPE811_State_t TM_STMPE811_Init(void);

/**
 * Read touch coordinates
 *
 * Parameters:
 * 	- TM_STMPE811_TouchData *structdata: pointer to struct to store data to
 *
 * Returns TM_STMPE811_State_Pressed is pressed, otherwise TM_STMPE811_State_Released
 */
extern TM_STMPE811_State_t TM_STMPE811_ReadTouch(TM_STMPE811_TouchData *structdata);

#define TM_STMPE811_TouchInRectangle(sd, xPos, yPos, w, h)	(((sd)->x >= (xPos)) && ((sd)->x < (xPos + w)) && ((sd)->y >= (yPos)) && ((sd)->y < (yPos + h)))

#endif

