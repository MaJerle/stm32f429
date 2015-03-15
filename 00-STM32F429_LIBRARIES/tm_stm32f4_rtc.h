/**
 * Internal RTC library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/07/library-19-use-internal-rtc-on-stm32f4xx-devices/
 *	@version 	v1.7
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
 * Version 1.7
 *	- March 15, 2015
 *	- Added support for read/write data to/from RTC backup registers
 *
 * Version 1.6
 *	- February 17, 2015
 *	- Created typedef TM_RTC_t from TM_RTC_Time_t
 *
 * Version 1.5
 *	- December 21, 2014
 *	- Added 2 new functions:
 *		TM_RTC_GetDaysInMonth: returns number of days in specific month and year
 *		TM_RTC_GetDaysInYear: returns number of days in specific year
 *
 * Version 1.4
 *	- December 21, 2014
 *	- TM_RTC_SetDateTime now checks for valid input data before save
 *	- Added function TM_RTC_SetDateTimeString which allows you to set your
 *		date and time using string format
 *
 * Version 1.3
 *	- December 03, 2014
 *	- Fixed bug when reading month in december was set to 0
 *
 * Version 1.2
 *	- October 27, 2014
 *	- Added support to read subseconds from time
 *	- This can be used for stopwatch
 *
 * Version 1.1
 *	- October 20, 2014
 *	- Added support for Alarm set
 *
 * Version 1.0.2
 *	- September 24, 2014
 *	- TM_RTC_RequestHandler function has now "__weak" attribute to prevent errors,
 *	  if function is not implemented by user
 *
 * Version 1.0.1
 *	- September 01, 2014
 *	- Date to unix convert bug fix
 * 
 * Pinout for RTC external 32768Hz crystal
 *
 *	STM32F4XX		Oscillator		Description
 *	
 *	PC14			OSC1			Oscillator terminal 1
 *	PC15			OSC2			Oscillator terminal 2
 */
#ifndef TM_RTC_H
#define TM_RTC_H 170
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx RTC
 * - STM32F4xx PWR
 * - STM32F4xx EXTI
 * - MISC
 * - defines.h
 * - attributes.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "defines.h"
#include "attributes.h"

/* RTC clock is: f_clk = RTCCLK(LSI or LSE) / ((RTC_SYNC_PREDIV + 1) * (RTC_ASYNC_PREDIV + 1)) */
/* Sync pre division for clock */
#ifndef RTC_SYNC_PREDIV
#define RTC_SYNC_PREDIV					0x3FF
#endif
/* Async pre division for clock */
#ifndef RTC_ASYNC_PREDIV
#define RTC_ASYNC_PREDIV				0x1F
#endif
/* NVIC global Priority set */
#ifndef RTC_PRIORITY
#define RTC_PRIORITY					0x04
#endif
/* Sub priority for wakeup trigger */
#ifndef RTC_WAKEUP_SUBPRIORITY
#define RTC_WAKEUP_SUBPRIORITY			0x00
#endif
/* Sub priority for alarm trigger */
#ifndef RTC_ALARM_SUBPRIORITY
#define RTC_ALARM_SUBPRIORITY			0x01
#endif

/* Internal status registers for RTC */
#define RTC_STATUS_REG      			RTC_BKP_DR19 /* Status Register */
#define RTC_STATUS_INIT_OK  			0x1234       /* RTC initialised */
#define RTC_STATUS_TIME_OK  			0x4321       /* RTC time OK */
#define	RTC_STATUS_ZERO					0x0000

/* Internal RTC defines */
#define TM_RTC_LEAP_YEAR(year) 			((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define TM_RTC_DAYS_IN_YEAR(x)			TM_RTC_LEAP_YEAR(x) ? 366 : 365
#define TM_RTC_OFFSET_YEAR				1970
#define TM_RTC_SECONDS_PER_DAY			86400
#define TM_RTC_SECONDS_PER_HOUR			3600
#define TM_RTC_SECONDS_PER_MINUTE		60
#define TM_RTC_BCD2BIN(x)				((((x) >> 4) & 0x0F) * 10 + ((x) & 0x0F))
#define TM_RTC_CHAR2NUM(x)				((x) - '0')
#define TM_RTC_CHARISNUM(x)				((x) >= '0' && (x) <= '9')

/**
 * Struct for date/time
 *
 * Parameters:
 * 	- uint8_t seconds:
 * 		Seconds parameter, from 00 to 59
 * 	- uint16_t subseconds:
 * 		Subsecond downcounter. When it reaches zero, it's reload value is the same as
 *		RTC_SYNC_PREDIV, so in our case 0x3FF = 1023, 1024 steps in one second
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
 *	- uint32_t unix:
 *		Seconds from 	01.01.1970 00:00:00
 */
typedef struct {
	uint8_t seconds;
	uint16_t subseconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint32_t unix;
} TM_RTC_t;
/* Backward compatibility */
typedef TM_RTC_t TM_RTC_Time_t;
/**
 * Result enumeration
 *
 * Parameters:
 * 	- TM_RTC_Result_Ok:
 * 		Everything OK
 * 	- TM_RTC_Result_Error:
 * 		An error occured
 */
typedef enum {
	TM_RTC_Result_Ok,
	TM_RTC_Result_Error
} TM_RTC_Result_t;

/**
 * Set format of date and time
 *
 * Parameters:
 * - TM_RTC_Format_BIN:
 * 		Binary format
 * - TM_RTC_Format_BCD:
 * 		BCD format
 */
typedef enum {
	TM_RTC_Format_BIN = 0x00,
	TM_RTC_Format_BCD
} TM_RTC_Format_t;

/**
 * Select RTC to wakeup MCU every X seconds
 *
 * Parameters:
 * 	- TM_RTC_Int_Disable:
 * 		Disable wakeup interrupt
 * 	- TM_RTC_Int_60s:
 * 		Interrupt every 60 seconds
 * 	- TM_RTC_Int_30s:
 * 		Interrupt every 30 seconds
 * 	- TM_RTC_Int_15s:
 * 		Interrupt every 15 seconds
 * 	- TM_RTC_Int_10s:
 * 		Interrupt every 10 seconds
 * 	- TM_RTC_Int_5s:
 * 		Interrupt every 5 seconds
 * 	- TM_RTC_Int_2s:
 * 		Interrupt every 2 seconds
 * 	- TM_RTC_Int_1s:
 * 		Interrupt every 1 seconds
 * 	- TM_RTC_Int_500ms:
 * 		Interrupt every 500 milliseconds
 * 	- TM_RTC_Int_250ms:
 * 		Interrupt every 250 milliseconds
 * 	- TM_RTC_Int_125ms:
 * 		Interrupt every 125 milliseconds
 */
typedef enum {
	TM_RTC_Int_Disable = 0x00,
	TM_RTC_Int_60s,
	TM_RTC_Int_30s,
	TM_RTC_Int_15s,
	TM_RTC_Int_10s,
	TM_RTC_Int_5s,
	TM_RTC_Int_2s,
	TM_RTC_Int_1s,
	TM_RTC_Int_500ms,
	TM_RTC_Int_250ms,
	TM_RTC_Int_125ms
} TM_RTC_Int_t;

/**
 * Use internal or external clock
 *
 * Parameters:
 * 	- TM_RTC_ClockSource_Internal:
 * 		Use internal clock for RTC, LSI oscillator
 * 	- TM_RTC_ClockSource_External
 * 		Use external, more accurate, clock for RTC, 32768Hz
 *
 * Note: Internal clock is not so accurate
 */
typedef enum {
	TM_RTC_ClockSource_Internal = 0x00,
	TM_RTC_ClockSource_External
} TM_RTC_ClockSource_t;

/**
 * Use alarm type you will use
 *
 * Parameters:
 * 	- TM_RTC_AlarmType_DayInWeek
 * 		Trigger alarm every day in a week,
 * 		days from 1 to 7 (Monday to Sunday)
 * 	- TM_RTC_AlarmType_DayInMonth
 * 		Trigger alarm every month
 */
typedef enum {
	TM_RTC_AlarmType_DayInWeek,
	TM_RTC_AlarmType_DayInMonth
} TM_RTC_AlarmType_t;

/**
 * Alarm identifier you will use
 *
 * Parameters:
 * 	- TM_RTC_Alarm_A
 * 		Work with alarm A
 * 	- TM_RTC_Alarm_B
 * 		Work with alarm B
 */
typedef enum {
	TM_RTC_Alarm_A = 0x00,
	TM_RTC_Alarm_B
} TM_RTC_Alarm_t;

/**
 * Struct for alarm time
 *
 * Parameters:
 *  - TM_RTC_AlarmType_t alarmtype:
 *		Alarm type setting. Read TM_RTC_AlarmType_t typedef comment for more info
 * 	- uint8_t seconds:
 * 		Seconds parameter, from 00 to 59
 * 	- uint8_t minutes:
 * 		Minutes parameter, from 00 to 59
 * 	- uint8_t hours:
 * 		Hours parameter, 24Hour mode, 00 to 23
 * 	- uint8_t day:
 * 		If you select trigger for alarm every week, then this parameter has value between
 * 		1 and 7, representing days in a week, Monday to Sunday
 * 		If you select trigger for alarm every month, then this parameter has value between
 * 		1 - 31, representing days in a month.
 */
typedef struct {
	TM_RTC_AlarmType_t alarmtype;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
} TM_RTC_AlarmTime_t;

/**
 * Initialize RTC
 *
 * Parameters:
 * 	- TM_RTC_ClockSource_t source:
 * 		Select RTC clock source
 *
 * Returns 0 if RTC was initialized first time (power up), so you know when to set date and time
 */
uint32_t TM_RTC_Init(TM_RTC_ClockSource_t source);

/**
 * Get number of seconds from 01.01.1970 00:00:00
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Struct to time data
 *
 * Returns Unix seconds
 */
uint32_t TM_RTC_GetUnixTimeStamp(TM_RTC_t* data);

/**
 * Get formatted time from seconds till 01.01.1970 00:00:00
 * It fills struct with valid data. Valid if year is greater or equal (>=) than 2000
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Pointer to TM_RTC_Time_t struct to store formatted data in
 * 	- uint32_t unix:
 * 		Seconds from 01.01.1970 00:00:00
 */
void TM_RTC_GetDateTimeFromUnix(TM_RTC_t* data, uint32_t unix);

/**
 * Select RTC interrupt
 *
 * Parameters:
 * 	- TM_RTC_Int_t int_value:
 * 		Choose struct member from struct TM_RTC_Int_t
 */
void TM_RTC_Interrupts(TM_RTC_Int_t int_value);

/**
 * Set date and time
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Struct to time data
 * 	- TM_RTC_Format_t format:
 * 		Member of struct TM_RTC_Format_t
 */
TM_RTC_Result_t TM_RTC_SetDateTime(TM_RTC_t* data, TM_RTC_Format_t format);

/**
 * Set date and time using string format
 *
 * Valid string format is:
 *
 *	dd.mm.YY.x;HH:ii:ss
 *
 * - dd: date, 2 digits, decimal
 * - mm: month, 2 digits, decimal
 * - YY: year, last 2 digits, decimal
 * - x: day in a week: 1 digit,
 * - HH: hours, 24-hour mode, 2 digits, decimal
 * - ii: minutes, 2 digits, decimal
 * - ss: seconds, 2 ditigs, decimal
 *
 * Example:
 *	- For date&time Thursday, December 25, 2014 22:05:12, you have to use string below:
 *		25.12.14.4;22:05:12
 *
 * Parameters:
 * 	- char* str:
 *		Date in string format.
 *
 * If date and time are valid, function will return TM_RTC_Result_Ok
 */
TM_RTC_Result_t TM_RTC_SetDateTimeString(char* str);

/**
 * Get date and time
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Struct to time data where to save data
 * 	- TM_RTC_Format_t format:
 * 		Member of struct TM_RTC_Format_t
 */
void TM_RTC_GetDateTime(TM_RTC_t* data, TM_RTC_Format_t format);

/**
 * Get number of days in month
 *
 * Parameters:
 * 	- uint8_t month:
 * 		Month, for which you need number of days (1 - 12)
 * 	- uint8_t year:
 *		Specify year you want to know, last 2 digits only (00-99)
 */
uint8_t TM_RTC_GetDaysInMonth(uint8_t month, uint8_t year);

/**
 * Get number of days in year
 *
 * Parameters:
 * 	- uint8_t year:
 *		Specify year you want to know days in, last 2 digits only (00-99)
 */
uint16_t TM_RTC_GetDaysInYear(uint8_t year);

/**
 * RTC has 20 backup registers where you can store data which will
 * be available all the time RTC is running and has power.
 *
 * Note:
 * 	My library uses register 19 to store info about RTC settings
 * 	and is not available for USER to store data there.
 * 
 * This method allows you to write 32bit value to backup register 0 - 18
 *
 * Note:
 *	RTC HAVE to be initializated first before you can use this method
 *
 * Parameters:
 *	- uint8_t location:
 *		RTC backup register location, 0 to 18 allowed
 *	- uint32_t value:
 *		- Value to store to backup register
 *
 * No return
 */
void TM_RTC_WriteBackupRegister(uint8_t location, uint32_t value);

/**
 * RTC has 20 backup registers where you can store data which will
 * be available all the time RTC is running and has power.
 *
 * Note:
 * 	My library uses register 19 to store info about RTC settings
 * 	and is not available for USER to store data there.
 *
 * Note:
 *	RTC HAVE to be initializated first before you can use this method
 * 
 * This method allows you to read 32bit value from backup register 0 - 18
 *
 * Parameters:
 *	- uint8_t location:
 *		RTC backup register location, 0 to 18 allowed
 *	- uint32_t value:
 *		- Value to store to backup register
 *
 * No return
 */
uint32_t TM_RTC_ReadBackupRegister(uint8_t location);
 
/**
 * Enable Alarm A or Alarm B for RTC
 *
 * Parameters:
 * 	- TM_RTC_Alarm_t Alarm:
 *		Specify alarm A or B
 * 	- TM_RTC_AlarmTime_t* AlarmTime:
 *		Pointer to alarm time struct
 * 	TM_RTC_Format_t format:
 * 		Stored date and time format, can be binary or "binary coded decimal"
 */
void TM_RTC_SetAlarm(TM_RTC_Alarm_t Alarm, TM_RTC_AlarmTime_t* AlarmTime, TM_RTC_Format_t format);

/**
 * Disable specific alarm
 *
 * Parameters:
 * 	- TM_RTC_Alarm_t Alarm:
 * 		Select Alarm A or Alarm B to be disabled
 */
void TM_RTC_DisableAlarm(TM_RTC_Alarm_t Alarm);

/**
 * Custom Request handler for RTC wakeup interrupt
 *
 * Called from main handler when wakeup interrupt occured
 *
 * With __weak parameter to prevent link errors if not defined by user
 */
void TM_RTC_RequestHandler(void);

/**
 * Custom Alarm A interrupt handler
 *
 * Called when Alarm A interrupt is triggered
 *
 * With __weak parameter to prevent link errors if not defined by user
 */
void TM_RTC_AlarmAHandler(void);

/**
 * Custom Alarm B interrupt handler
 *
 * Called when Alarm B interrupt is triggered
 *
 * With __weak parameter to prevent link errors if not defined by user
 */
void TM_RTC_AlarmBHandler(void);

#endif

