/**
 *	STMPE811 Touch screen controller library
 * 	It can be used on STM32F429 Discovery board, or any other LCD with 240*320 pixels
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/library-10-stmpe811-touch-screen-driver-for-stm32f429-discovery-board/ *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_STMPE811_H
#define TM_STMPE811_H
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM_I2C
 * - TM_DELAY
 * - TM_STMPE811
 */
/**
 * Connection (it can be changed in defines.h file)
 * 
 * I2Cx		I2C3
 * SCL		PA8
 * SDA		PC9
 * 
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

#ifndef STMPE811_I2C
//I2C used
#define STMPE811_I2C					I2C3
#define STMPE811_I2C_PINSPACK			TM_I2C_PinsPack_1
#endif

#define STMPE811_ADDRESS				0x82	//I2C address

//Registers
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

//STMPE811 Chip ID on reset
#define STMPE811_CHIP_ID_VALUE			0x0811	//Chip ID

//Register STMPE811_SYS_CTRL1 bits
#define STMPE811_SYS_CTRL1_SOFTRESET 	0x02	//Reset the STMPE811 using the serial communication interface
#define STMPE811_SYS_CTRL1_HIBERNATE	0x01	//Force the device into hibernation mode

//Register STMPE811_SYS_CTRL2 bits
#define STMPE811_CTRL2_TS_OFF			0x08	//Switch off the clock supply to the temperature sensor.
#define STMPE811_CTRL2_GPIO_OFF			0x04	//Switch off the clock the supply to the GPIO.
#define STMPE811_CTRL2_TSC_OFF  		0x02	//Switch off the clock supply to the touchscreen controller.
#define STMPE811_CTRL2_ADC_OFF			0x01	//Switch off the clock supply to the ADC.

//Register STMPE811_INT_EN bits
#define STMPE811_INT_EN_GPIO    		0x80	//Any enabled GPIO interrupts
#define STMPE811_INT_EN_ADC     		0x40	//Any enabled ADC interrupts
#define STMPE811_INT_EN_TEMP_SENS		0x20	//Temperature threshold triggering
#define STMPE811_INT_EN_FIFOEMPTY		0x10	//FIFO is empty
#define STMPE811_INT_EN_FIFOFULL		0x08	//FIFO is full
#define STMPE811_INT_EN_FIFOOFLOW		0x04	//FIFO is overflowed
#define STMPE811_INT_EN_TH      		0x02	//FIFO is equal or above threshold value
#define STMPE811_INT_EN_TOUCHDET		0x01	//Touch is detected

//Register STMPE811_ADC_CTRL1 bits
#define STMPE811_ADC_CTRL1_SAMPLE_M		0x07	//ADC conversion time (in number of clock) mask
#define STMPE811_ADC_CTRL1_SAMPLE_S		4		//ADC conversion time (in number of clock) shift
#define STMPE811_ADC_CTRL1_MOD			0x08	//Selects 10 or 12-bit ADC operation
#define STMPE811_ADC_CTRL1_SEL			0x02	//Select between internal or external reference for the ADC

//Register STMPE811_ADC_CTRL2 bits
#define STMPE811_ADC_CTRL2_CLK1			0x00	//Selects the clock speed of ADC is 1.625 MHz typ
#define STMPE811_ADC_CTRL2_CLK2 		0x01	//Selects the clock speed of ADC is 3.25 MHz typ
#define STMPE811_ADC_CTRL2_CLK13		0x02	//Selects the clock speed of ADC is 6.5 MHz typ
#define STMPE811_ADC_CTRL2_CLK4			0x03	//Selects the clock speed of ADC is 6.5 MHz typ 

//Register STMPE811_TSC_CFG bits
#define STMPE811_TSC_CFG_AVE_1  		0x00	//1 sample
#define STMPE811_TSC_CFG_AVE_2			0x40	//2 sample
#define STMPE811_TSC_CFG_AVE_4			0x80	//4 sample
#define STMPE811_TSC_CFG_AVE_8  		0xC0	//8 sample
#define STMPE811_TSC_CFG_DELAY_M		0x07	//Touch detect delay mask
#define STMPE811_TSC_CFG_DELAY_S		3		//Touch detect delay shift
#define STMPE811_TSC_CFG_SETTING_M		0x07	//Panel driver setting time mask
#define STMPE811_TSC_CFG_SETTING_S		0		//Panel driver setting time shift

//Register STMPE811_FIFO_STA FIFO bits
#define STMPE811_FIFO_STA_OFLOW			0x80	//Reads 1 if FIFO is overflow
#define STMPE811_FIFO_STA_FULL 			0x40	//Reads 1 if FIFO is full
#define STMPE811_FIFO_STA_EMPTY			0x20	//Reads 1 if FIFO is empty
#define STMPE811_FIFO_STA_TRIG  		0x10	//FIFO size is at or beyond the threshold value
#define STMPE811_FIFO_STA_RESET 		0x01	//Resets FIFO, all data in FIFO are cleared

//Register STMPE811_TSC_FRACTION_Z bits
#define STMPE811_FRACTION_Z_M			0x7		//Pressure measurement accuracy value mask
#define STMPE811_FRACTION_Z_S			0		//Pressure measurement accuracy value mask

//Register STMPE811_TSC_I_DRIVE bits.
#define STMPE811_TSC_I_DRIVE_20			0x00	//Set maximum current on the TSC driving channel is 20 mA, 35 mA max
#define STMPE811_TSC_I_DRIVE_50 		0x01	//Set maximum current on the TSC driving channel is 50 mA, 80 mA max

//Register STMPE811_TSC_CTRL bits
#define STMPE811_TSC_CTRL_STA   		0x80	//TSC status
#define STMPE811_TSC_CTRL_TRACK_M		0x70	//Tracking index mask
#define STMPE811_TSC_CTRL_TRACK_S		4		//Tracking index mask
#define STMPE811_TSC_CTRL_XYZ   		0x00	//TSC operating mode: X, Y, Z acquisition
#define STMPE811_TSC_CTRL_XY    		0x02	//TSC operating mode: only X, Y
#define STMPE811_TSC_CTRL_X     		0x04	//TSC operating mode: only X
#define STMPE811_TSC_CTRL_Y     		0x06	//TSC operating mode: only Y
#define STMPE811_TSC_CTRL_Z     		0x08	//TSC operating mode: only Z
#define STMPE811_TSC_CTRL_EN    		0x01	//Enable TSC

//Register STMPE811_INT_STA bits
#define STMPE811_INT_STA_GPIO   		0x80	//Any enable GPIO interrupts
#define STMPE811_INT_STA_ADC    		0x40	//Any enable ADC interrupts
#define STMPE811_INT_STA_SENS   		0x20	//Temperature threshold triggering
#define STMPE811_INT_STA_EMPTY  		0x10	//FIFO is empty
#define STMPE811_INT_STA_FULL   		0x08	//FIFO is full
#define STMPE811_INT_STA_OFLOW  		0x04	//FIFO is overflowed
#define STMPE811_INT_STA_TH     		0x02	//FIFO is equal or above threshold value
#define STMPE811_INT_STA_DET    		0x01	//Touch is detected

//Register STMPE811_INT_CTRL bits
#define STMPE811_INT_POLARITY   		0x04	//Sets the INT pin polarity
#define STMPE811_INT_TYPE       		0x02	//Sets the type of interrupt signal required by the host
#define STMPE811_INT_GLOBAL     		0x01	//Master enable for the interrupt system

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
 * 	- TM_STMPE811_Orientation_t orientation: Set touch screen orientation to read
 */
typedef struct {
	uint16_t x;
	uint16_t y;
	TM_STMPE811_State_t pressed;
	TM_STMPE811_Orientation_t orientation;
} TM_STMPE811_TouchData;

/**
 * Initialize STMPE811 Touch driver
 *
 * Returns 0 on success
 */
uint8_t TM_STMPE811_Init(void);

/**
 * Read touch coordinates
 *
 * Parameters:
 * 	- TM_STMPE811_TouchData *structdata: pointer to struct to store data to
 *
 * Returns TM_STMPE811_State_Pressed is pressed, otherwise TM_STMPE811_State_Released
 */
TM_STMPE811_State_t TM_STMPE811_ReadTouch(TM_STMPE811_TouchData *structdata);

/**
 * Internal functions
 */
/**
 * Read from STMPE811 register
 *
 * Parameters:
 * 	- uint8_t reg: register address
 *
 * Returns register data
 */
uint8_t TM_STMPE811_Read(uint8_t reg);

/**
 * Read X coordinate
 *
 * Parameters:
 * 	- uint16_t x: last x read coordinate
 */
uint16_t TM_STMPE811_ReadX(uint16_t x);

/**
 * Read Y coordinate
 *
 * Parameters:
 * 	- uint16_t y: last y read coordinate
 */
uint16_t TM_STMPE811_ReadY(uint16_t y);

#endif

