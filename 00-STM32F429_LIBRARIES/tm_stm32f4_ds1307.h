/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/05/library-15-ds1307-real-time-clock-for-stm32f429-discovery/ 
 * @version v1.2
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   DS1307 Library for STM32F4xx
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
#ifndef TM_DS1307_H
#define TM_DS1307_H 120
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_DS1307
 * @brief    DS1307 Library for STM32F4xx - http://stm32f4-discovery.com/2014/05/library-15-ds1307-real-time-clock-for-stm32f429-discovery/
 * @note     Not recommended for future projects, use internal RTC inside STM32F4xx instead.
 *@{
 *
 * \par Default pinout
 * 
@verbatim
DS1307	STM32F4XX	DESCRIPTION
VCC		5V			Positive voltage
GND		GND			Ground
SDA		PB7			I2C1, PinsPack 1
SCL		PB6			I2C1, PinsPack 1
@endverbatim
 *
 * Change pinout in your defines.h file
 * 
@verbatim
//Set your I2C, to see corresponding pins, look for TM I2C library
#define TM_DS1307_I2C				I2C1
#define TM_DS1307_I2C_PINSPACK		TM_I2C_PinsPack_1 
@endverbatim
 *
 *
 * \par Changelog
 *
@verbatim
 Version 1.2
  - September 21, 2014
  - Added support for check if DS1307 module is connected to I2C on initialization

 Version 1.1
  - July 30, 2014
  - Added 2 new functions to work with SQW/OUT pin on DS1307
     TM_DS1307_EnableOutputPin
     TM_DS1307_DisableOutputPin
	 
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx I2C
 - defines.h
 - TM I2C
 - TM GPIO
@endverbatim
 */

#include "stm32f4xx.h"
#include "tm_stm32f4_i2c.h"
#include "defines.h"

/**
 * @defgroup TM_DS1307_Macros
 * @brief    Library defines
 * @{
 */
 
/* I2C settings for I2C library */
#ifndef DS1307_I2C
#define DS1307_I2C					I2C1
#define DS1307_I2C_PINSPACK			TM_I2C_PinsPack_1
#endif

/* DS1307 I2C clock */
#ifndef DS1307_I2C_CLOCK
#define DS1307_I2C_CLOCK			100000
#endif

/* I2C slave address for DS1307 */
#define DS1307_I2C_ADDR				0xD0

/* Registers location */
#define DS1307_SECONDS				0x00
#define DS1307_MINUTES				0x01
#define DS1307_HOURS				0x02
#define DS1307_DAY					0x03
#define DS1307_DATE					0x04
#define DS1307_MONTH				0x05
#define DS1307_YEAR					0x06
#define DS1307_CONTROL				0x07

/* Bits in control register */
#define DS1307_CONTROL_OUT			7
#define DS1307_CONTROL_SQWE			4
#define DS1307_CONTROL_RS1			1
#define DS1307_CONTROL_RS0			0

/**
 * @}
 */
 
/**
 * @defgroup TM_DS1307_Typedefs
 * @brief    Library Typedefs
 * @{
 */
 
/**
 * @brief  Result enumeration
 */
typedef enum {
	TM_DS1307_Result_Ok = 0x00,         /*!< Everything OK */
	TM_DS1307_Result_Error,             /*!< An error occurred */
	TM_DS1307_Result_DeviceNotConnected /*!< Device is not connected */
} TM_DS1307_Result_t;

/**
 * @brief  Structure for date/time
 */
typedef struct {
	uint8_t seconds; /*!< Seconds parameter, from 00 to 59 */
	uint8_t minutes; /*!< Minutes parameter, from 00 to 59 */
	uint8_t hours;   /*!< Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t day;     /*!< Day in a week, from 1 to 7 */
	uint8_t date;    /*!< Date in a month, 1 to 31 */
	uint8_t month;   /*!< Month in a year, 1 to 12 */
	uint8_t year;    /*!< Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
} TM_DS1307_Time_t;

/**
 * @brief  Enumeration for SQW/OUT pin
 */
typedef enum {
	TM_DS1307_OutputFrequency_1Hz = 0x00, /*!< Set SQW/OUT pin to 1Hz output frequency */
	TM_DS1307_OutputFrequency_4096Hz,     /*!< Set SQW/OUT pin to 4096Hz output frequency */
	TM_DS1307_OutputFrequency_8192Hz,     /*!< Set SQW/OUT pin to 8192Hz output frequency */
	TM_DS1307_OutputFrequency_32768Hz,    /*!< Set SQW/OUT pin to 32768Hz output frequency */
	TM_DS1307_OutputFrequency_High,       /*!< Set SQW/OUT pin high. Because this pin is open-drain, you will need external pull up resistor */
	TM_DS1307_OutputFrequency_Low         /*!< Set SQW/OUT pin low */
} TM_DS1307_OutputFrequency_t;

/**
 * @}
 */

/**
 * @defgroup TM_DS1307_Functions
 * @brief    Library Functions
 * @{
 */
 
/**
 * @brief  Initializes DS1307 RTC library
 * @param  None
 * @retval None
 */
TM_DS1307_Result_t TM_DS1307_Init(void);

/**
 * @brief  Gets seconds from DS1307
 * @param  None
 * @retval Seconds from DS1307, 00 to 59
 */
uint8_t TM_DS1307_GetSeconds(void);

/**
 * @brief  Gets minutes from DS1307
 * @param  None
 * @retval Minutes from DS1307, 00 to 59
 */
uint8_t TM_DS1307_GetMinutes(void);

/**
 * @brief  Gets hours from DS1307
 * @param  None
 * @retval Hours from DS1307, 00 to 23
 */
uint8_t TM_DS1307_GetHours(void);

/**
 * @brief  Gets day in a week from DS1307
 * @param  None
 * @retval Day in a week from DS1307, 1 to 7
 */
uint8_t TM_DS1307_GetDay(void);

/**
 * @brief  Gets date from DS1307
 * @param  None
 * @retval Date from DS1307, 01 to 31
 */
uint8_t TM_DS1307_GetDate(void);

/**
 * @brief  Gets month from DS1307
 * @param  None
 * @retval Month from DS1307, 01 to 12
 */
uint8_t TM_DS1307_GetMonth(void);

/**
 * @brief  Gets year from DS1307
 * @param  None
 * Returns year from DS1307, 00 (2000) to 99 (2099)
 */
uint8_t TM_DS1307_GetYear(void);


/**
 * @brief  Sets seconds to DS1307
 * @param  seconds: Seconds to be set to DS1307, 01 to 59
 * @retval None
 */
void TM_DS1307_SetSeconds(uint8_t seconds);

/**
 * @brief  Sets minutes to DS1307
 * @param  minutes: Minutes to be set to DS1307, 01 to 59
 * @retval None
 */
void TM_DS1307_SetMinutes(uint8_t minutes);

/**
 * @brief  Sets hours to DS1307
 * @param  hours: Hours to be set to DS1307, 01 to 23
 * @retval None
 */
void TM_DS1307_SetHours(uint8_t hours);

/**
 * @brief  Sets day to DS1307
 * @param  day: Day to be set to DS1307, 1 to 7
 * @retval None
 */
void TM_DS1307_SetDay(uint8_t day);

/**
 * @brief  Sets date to DS1307
 * @param  date: Date to be set to DS1307, 01 to 31
 * @retval None
 */
void TM_DS1307_SetDate(uint8_t date);

/**
 * @brief  Sets month to DS1307
 * @param  month: Month to be set to DS1307, 01 to 12
 * @retval None
 */
void TM_DS1307_SetMonth(uint8_t month);

/**
 * @brief  Sets year to DS1307
 * @param  year: Year to be set to DS1307, 00 (2000) to 99 (2099)
 * @retval None
 */
void TM_DS1307_SetYear(uint8_t year);

/**
 * @brief  Gets full date and time from DS1307
 * @param  *time: Pointer to @ret TM_DS1307_Time_t structure where to set data
 * @retval None
 */
void TM_DS1307_GetDateTime(TM_DS1307_Time_t* time);

/**
 * @brief  Sets full date and time to DS1307
 * @param  *time: Pointer to @ret TM_DS1307_Time_t structure where to get data
 * @retval None
 */
void TM_DS1307_SetDateTime(TM_DS1307_Time_t* time);

/**
 * @brief  DS1307 has SQW/OUT pin, which can be enabled in various modes.
 *         It can output 32768Hz, 8192Hz, 4096Hz, 1Hz, Low or High state.
 *         This is useful if you need interrupts on MCU. 1Hz can be used to increment time by software each time.
 *         This is faster than look for date and time each time.
 *
 * @note   Also, this pin is Open-Drain. This means that pin cannot supply positive power supply,
 *         for that you need external pull up resistor (or pull up from MCU).
 *
 * @param  frequency: SQW frequency. This parameter can be a value of @ref TM_DS1307_OutputFrequency_t enumeration
 * @retval None
 */
void TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_t frequency);

/**
 * @brief  Disables SQW/OUT pin
 *
 * This function basically set pin to high state.
 * To get high state you need external pull up resistor (or use pull up from MCU)
 * @retval None
 */
void TM_DS1307_DisableOutputPin(void);


/**
 * @brief  Converts BCD to BIN data
 * @param  bcd: BCD value to be converted
 * @retval BIN value from given BCD
 */
uint8_t TM_DS1307_Bcd2Bin(uint8_t bcd);

/**
 * @brief  Converts BIN to BCD data
 * @param  bin: BIN value to be converted
 * @retval BCD value from given BIN
 */
uint8_t TM_DS1307_Bin2Bcd(uint8_t bin);

/**
 * @brief  Checks for min and max values, which can be stored to DS1307
 * @param  val: Value to be checked
 * @param  min: Minimal allowed value of val
 * @param  max: Maximal allowed value of val
 * @retval Value between min and max
 */
uint8_t TM_DS1307_CheckMinMax(uint8_t val, uint8_t min, uint8_t max);

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

