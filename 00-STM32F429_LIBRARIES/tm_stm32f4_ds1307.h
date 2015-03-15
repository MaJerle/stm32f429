/**
 *  DS1307 Library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/library-15-ds1307-real-time-clock-for-stm32f429-discovery/
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
 *	- September 21, 2014
 * 	- Added support for check if DS1307 module is connected to I2C on initialization
 *
 * Version 1.1
 *	- July 30, 2014
 * 	- Added 2 new functions to work with SQW/OUT pin on DS1307
 * 		TM_DS1307_EnableOutputPin
 * 		TM_DS1307_DisableOutputPin
 * 		
 * Default pinout
 * 
 * 	DS1307	STM32F4XX	DESCRIPTION
 *	VCC		5V			Positive voltage
 *	GND		GND			Ground
 *	SDA		PB7			I2C1, PinsPack 1
 *	SCL		PB6			I2C1, PinsPack 1
 *	
 * Change pinout in your defines.h file
 * 
 *	//Set your I2C, to see corresponding pins, look for TM I2C library
 * 	#define TM_DS1307_I2C				I2C1
 *	#define TM_DS1307_I2C_PINSPACK		TM_I2C_PinsPack_1
 */
#ifndef TM_DS1307_H
#define TM_DS1307_H 120
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx I2C
 * - TM I2C
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_i2c.h"
#include "defines.h"

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
 * Result enumeration
 *
 * Parameters:
 * 	- TM_DS1307_Result_Ok:
 * 		Everything OK
 * 	- TM_DS1307_Result_Error:
 * 		An error occured
 * 	- TM_DS1307_Result_DeviceNotConnected:
 *		Device is not connected
 */
typedef enum {
	TM_DS1307_Result_Ok = 0,
	TM_DS1307_Result_Error,
	TM_DS1307_Result_DeviceNotConnected
} TM_DS1307_Result_t;

/**
 * Struct for date/time
 *
 * Parameters:
 * 	- uint8_t seconds:
 * 		Seconds parameter, from 00 to 59
 * 	- uint8_t minutes:
 * 		Minutes parameter, from 00 to 59
 * 	- uint8_t hours:
 * 		Hours parameter, 24Hour mode, 00 to 23
 * 	- uint8_t day:
 * 		Day in a week, from 1 to 7
 * 	- uint8_t date:
 * 		Date in a month, 1 to 31
 * 	- uint8_t month:
 * 		Month in a year, 1 to 12
 * 	- uint8_t year:
 * 		Year parameter, 00 to 99, 00 is 2000 and 99 is 2099
 */
typedef struct {
	uint8_t seconds;	//Seconds, 			00-59
	uint8_t minutes;	//Minutes, 			00-59
	uint8_t hours;		//Hours,			00-23
	uint8_t day;		//Day in a week, 	1-7
	uint8_t date;		//Day in a month	1-31
	uint8_t month;		//Month,			1-12
	uint8_t year;		//Year				00-99
} TM_DS1307_Time_t;

/**
 * Typedef enumerations for SQW/OUT pin
 *
 * 	- TM_DS1307_OutputFrequency_1Hz:
 * 		Set SQW/OUT pin to 1Hz output frequency
 * 	- TM_DS1307_OutputFrequency_4096Hz
 * 		Set SQW/OUT pin to 4096Hz output frequency
 * 	- TM_DS1307_OutputFrequency_8192Hz
 * 		Set SQW/OUT pin to 8192Hz output frequency
 * 	- TM_DS1307_OutputFrequency_32768Hz
 * 		Set SQW/OUT pin to 32768Hz output frequency
 * 	- TM_DS1307_OutputFrequency_High
 * 		Set SQW/OUT pin high. Because this pin is open-drain, you will need external pull up resistor
 * 	- TM_DS1307_OutputFrequency_Low
 * 		Set SQW/OUT pin low
 */
typedef enum {
	TM_DS1307_OutputFrequency_1Hz = 0,
	TM_DS1307_OutputFrequency_4096Hz,
	TM_DS1307_OutputFrequency_8192Hz,
	TM_DS1307_OutputFrequency_32768Hz,
	TM_DS1307_OutputFrequency_High,
	TM_DS1307_OutputFrequency_Low
} TM_DS1307_OutputFrequency_t;

/**
 * Initialize DS1307 RTC library
 *
 * No parameters
 *
 * No returns
 */
extern TM_DS1307_Result_t TM_DS1307_Init(void);

/**
 * Get seconds from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns seconds from DS1307, 00 to 59
 */
extern uint8_t TM_DS1307_GetSeconds(void);

/**
 * Get minutes from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns minutes from DS1307, 00 to 59
 */
extern uint8_t TM_DS1307_GetMinutes(void);

/**
 * Get hours from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns hours from DS1307, 00 to 23
 */
extern uint8_t TM_DS1307_GetHours(void);

/**
 * Get day in a week from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns day in a week from DS1307, 1 to 7
 */
extern uint8_t TM_DS1307_GetDay(void);

/**
 * Get date from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns date from DS1307, 01 to 31
 */
extern uint8_t TM_DS1307_GetDate(void);

/**
 * Get month from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns month from DS1307, 01 to 12
 */
extern uint8_t TM_DS1307_GetMonth(void);

/**
 * Get year from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns year from DS1307, 00 (2000) to 99 (2099)
 */
extern uint8_t TM_DS1307_GetYear(void);


/**
 * Set seconds to DS1307
 *
 * Parameters:
 * 	- uint8_t seconds
 * 		Seconds to be set to DS1307, 01 to 59
 *
 * No returns
 */
extern void TM_DS1307_SetSeconds(uint8_t seconds);

/**
 * Set minutes to DS1307
 *
 * Parameters:
 * 	- uint8_t minutes
 * 		Minutes to be set to DS1307, 01 to 59
 *
 * No returns
 */
extern void TM_DS1307_SetMinutes(uint8_t minutes);

/**
 * Set hours to DS1307
 *
 * Parameters:
 * 	- uint8_t hours
 * 		Hours to be set to DS1307, 01 to 23
 *
 * No returns
 */
extern void TM_DS1307_SetHours(uint8_t hours);

/**
 * Set day to DS1307
 *
 * Parameters:
 * 	- uint8_t day
 * 		Day to be set to DS1307, 1 to 7
 *
 * No returns
 */
extern void TM_DS1307_SetDay(uint8_t day);

/**
 * Set date to DS1307
 *
 * Parameters:
 * 	- uint8_t date
 * 		Date to be set to DS1307, 01 to 31
 *
 * No returns
 */
extern void TM_DS1307_SetDate(uint8_t date);

/**
 * Set month to DS1307
 *
 * Parameters:
 * 	- uint8_t month
 * 		Month to be set to DS1307, 01 to 12
 *
 * No returns
 */
extern void TM_DS1307_SetMonth(uint8_t month);

/**
 * Set year to DS1307
 *
 * Parameters:
 * 	- uint8_t year
 * 		Year to be set to DS1307, 00 (2000) to 99 (2099)
 *
 * No returns
 */
extern void TM_DS1307_SetYear(uint8_t year);

/**
 * Get full date and time from DS1307
 *
 * Parameters:
 * 	- TM_DS1307_Time_t* time
 * 		Pointer to struct where to set data
 *
 * No returns
 */
extern void TM_DS1307_GetDateTime(TM_DS1307_Time_t* time);

/**
 * Set full date and time to DS1307
 *
 * Parameters:
 * 	- TM_DS1307_Time_t* time
 * 		Pointer to struct where to get data
 *
 * No returns
 */
extern void TM_DS1307_SetDateTime(TM_DS1307_Time_t* time);

/**
 * DS1307 has SQW/OUT pin, which can be enabled in various modes.
 * It can output 32768Hz, 8192Hz, 4096Hz, 1Hz, Low or High state.
 * This is useful if you need interrupts on MCU. 1Hz can be used to increment time by software each time.
 * This is faster than look for date and time each time.
 *
 * Also, this pin is Open-Drain. This means that pin cannot supply positive power supply,
 * for that you need external pull up resistor (or pull up from MCU).
 *
 * Parameters:
 * 	- TM_DS1307_OutputFrequency_t frequency:
 * 		Member of TM_DS1307_OutputFrequency_t typedef
 *
 * No returns
 */
extern void TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_t frequency);

/**
 * Disable SQW/OUT pin.
 *
 * This function basically set pin to high state.
 * To get high state you need external pull up resistor (or use pull up from MCU)
 *
 * No returns
 */
extern void TM_DS1307_DisableOutputPin(void);


/**
 * Convert BCD to BIN data
 *
 * Parameters:
 * 	- uint8_t bcd
 * 		BCD value to be converted
 *
 * Returns BIN value of given BCD
 */
extern uint8_t TM_DS1307_Bcd2Bin(uint8_t bcd);

/**
 * Convert BIN to BCD data
 *
 * Parameters:
 * 	- uint8_t bin
 * 		BIN value to be converted
 *
 * Returns BCD value of given BIN
 */
extern uint8_t TM_DS1307_Bin2Bcd(uint8_t bin);

/**
 * Checks for min and max values, which can be stored to DS1307
 *
 * Parameters:
 * 	- uint8_t val
 * 		Value to be checked
 * 	- uint8_t min
 * 		Minimal allowed value of val
 * 	- uint8_t max
 * 		Maximal allowed value of val
 *
 * Returns value between min and max
 */
extern uint8_t TM_DS1307_CheckMinMax(uint8_t val, uint8_t min, uint8_t max);

#endif

