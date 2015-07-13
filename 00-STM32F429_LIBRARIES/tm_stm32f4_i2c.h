/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/05/library-09-i2c-for-stm32f4xx/
 * @version v1.6.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   I2C library for STM32F4xx
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
#ifndef TM_I2C_H
#define TM_I2C_H 161
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_I2C
 * @brief    I2C library for STM32F4xx - http://stm32f4-discovery.com/2014/05/library-09-i2c-for-stm32f4xx/
 * @{
 *
 *	\par Pinout
 *	
@verbatim
       |PINSPACK 1   |PINSPACK 2   |PINSPACK 3
I2CX   |SCL   SDA    |SCL   SDA    |SCL   SDA
       |             |             |
I2C1   |PB6   PB7    |PB8   PB9    |PB6   PB9
I2C2   |PB10  PB11   |PF1   PF0    |PH4   PH5
I2C3   |PA8   PC9    |PH7   PH8    |-     -
@endverbatim
 *
 * \par Custom pinout
 *
 * In case these pins are not good for you, you can use
 * @ref TM_I2C_PinsPack_Custom in @ref TM_I2C_Init() function and callback function will be called,
 * where you can initialize your custom pinout for your I2C peripheral
 *
 * Possible changes in your defines.h file:
 * Change x to your I2C used, 1-3
 *	
@verbatim
//By default library support only 7bit long address
#define TM_I2Cx_ACKNOWLEDGED_ADDRESS   I2C_AcknowledgedAddress_7bit
//Library supports I2C mode
#define TM_I2Cx_MODE                   I2C_Mode_I2C
//Own address, if slave mode
#define TM_I2Cx_OWN_ADDRESS            0x00
//By default, disable ack
#define TM_I2Cx_ACK                    I2C_Ack_Disable
//Duty cycle 2, 50%
#define TM_I2Cx_DUTY_CYCLE             I2C_DutyCycle_2
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.6.1
  - March 31, 2015
  - Fixed I2C issue when sometime it didn't send data
  
 Version 1.6
  - March 13, 2015
  - Added new function to write multi bytes to device without specify register address

 Version 1.5
  - March 10, 2015
  - Updated to be more independent of HAL/STD drivers. 

 Version 1.4
  - March 08, 2015
  - Added support for new GPIO settings

 Version 1.3
  - December 22, 2014
  - Added option to read multi bytes from device without setting register from where

 Version 1.2
  - August 14, 2014
  - If you connect more devices on one I2C with different max SCL speed, low speed will be always selected.
  - Added some additional pins for I2C

 Version 1.1
  - September 08, 2014
  - Added support to check if device is connected to I2C bus

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx I2C
 - defines.h
 - attributes.h
 - TM GPIO
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "attributes.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_I2C_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Timeout for I2C
 */
#ifndef TM_I2C_TIMEOUT
#define TM_I2C_TIMEOUT					20000
#endif

/* I2C1 settings, change them in defines.h project file */
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

/* I2C2 settings, change them in defines.h project file */
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

/* I2C3 settings, change them in defines.h project file */
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

#define TM_I2C_CLOCK_STANDARD			100000  /*!< I2C Standard speed */
#define TM_I2C_CLOCK_FAST_MODE			400000  /*!< I2C Fast mode speed */
#define TM_I2C_CLOCK_FAST_MODE_PLUS		1000000 /*!< I2C Fast mode plus speed */
#define TM_I2C_CLOCK_HIGH_SPEED			3400000 /*!< I2C High speed */

 /**
 * @}
 */
 
/**
 * @defgroup TM_I2C_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  I2C pinspack enumeration
 */
typedef enum {
	TM_I2C_PinsPack_1,      /*!< Use Pinspack1 from Pinout table for I2Cx */
	TM_I2C_PinsPack_2,      /*!< Use Pinspack2 from Pinout table for I2Cx */
	TM_I2C_PinsPack_3,      /*!< Use Pinspack3 from Pinout table for I2Cx */
	TM_I2C_PinsPack_Custom  /*!< Use custom pins for I2Cx */
} TM_I2C_PinsPack_t;

/**
 * @}
 */

/**
 * @defgroup TM_I2C_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes I2C
 * @param  *I2Cx: I2C used
 * @param  pinspack: Pins used. This parameter can have a value of @ref TM_I2C_PinsPack_t enumeration
 * @param  clockSpeed: Clock speed for SCL in Hertz
 * @retval None
 */
void TM_I2C_Init(I2C_TypeDef* I2Cx, TM_I2C_PinsPack_t pinspack, uint32_t clockSpeed);

/**
 * @brief  Reads single byte from slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to read from
 * @retval Data from slave
 */
uint8_t TM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);

/**
 * @brief  Reads multi bytes from slave
 * @param  *I2Cx: I2C used
 * @param  uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  uint8_t reg: register to read from
 * @param  uint8_t *data: pointer to data array to store data from slave
 * @param  uint8_t count: how many bytes will be read
 * @retval None
 */
void TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * @brief  Reads byte from slave without specify register address
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Data from slave
 */
uint8_t TM_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * @brief  Reads multi bytes from slave without setting register from where to start read
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to store data from slave
 * @param  count: how many bytes will be read
 * @retval None
 */
void TM_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void TM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
 * @brief  Writes byte to slave without specify register address
 *
 *         Useful if you have I2C device to read like that:
 *            - I2C START
 *            - SEND DEVICE ADDRESS
 *            - SEND DATA BYTE
 *            - I2C STOP
 *
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  data: data byte which will be send to device
 * @retval None
 */
void TM_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data);

/**
 * @brief  Writes multi bytes to slave without setting register from where to start write
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to write data to slave
 * @param  count: how many bytes you want to write
 * @retval None
 */
void TM_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

/**
 * @brief  Checks if device is connected to I2C bus
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Device status:
 *            - 0: Device is not connected
 *            - > 0: Device is connected
 */
uint8_t TM_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address);

/**
 * @brief  I2C Start condition
 * @param  *I2Cx: I2C used
 * @param  address: slave address
 * @param  direction: master to slave or slave to master
 * @param  ack: ack enabled or disabled
 * @retval Start condition status
 * @note   For private use
 */
int16_t TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);

/**
 * @brief  Stop condition on I2C
 * @param  *I2Cx: I2C used
 * @retval Stop condition status
 * @note   For private use
 */
uint8_t TM_I2C_Stop(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte without ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t TM_I2C_ReadNack(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte with ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t TM_I2C_ReadAck(I2C_TypeDef* I2Cx);

/**
 * @brief  Writes to slave
 * @param  *I2Cx: I2C used
 * @param  data: data to be sent
 * @retval None
 * @note   For private use
 */
void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);

/**
 * @brief  Callback for custom pins initialization.
 * 
 *         When you call TM_I2C_Init() function, and if you pass TM_I2C_PinsPack_Custom to function,
 *         then this function will be called where you can initialize custom pins for I2C peripheral.
 * @param  *I2Cx: I2C for which initialization will be set
 * @param  AlternateFunction: Alternate function which should be used for GPIO initialization
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_I2C_InitCustomPinsCallback(I2C_TypeDef* I2Cx, uint16_t AlternateFunction);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

#endif

