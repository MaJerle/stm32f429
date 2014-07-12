/**
 * Internal RTC library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://bit.ly/U4wmKW
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_RTC_H
#define TM_RTC_H 100
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - STM32F4xx RTC
 * - STM32F4xx PWR
 * - STM32F4xx EXTI
 * - MISC
 * - defines.h
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

#define RTC_STATUS_REG      			RTC_BKP_DR0  // Status Register
#define RTC_STATUS_INIT_OK  			0x35AC       // RTC initialised
#define RTC_STATUS_TIME_OK  			0xA3C5       // RTC time ok
#define	RTC_STATUS_ZERO					0x00

#define TM_RTC_LEAP_YEAR(year) 			(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
#define TM_RTC_OFFSET_YEAR				1970
#define TM_RTC_DAYS_IN_YEAR				365
#define TM_RTC_SECONDS_PER_DAY			86400
#define TM_RTC_SECONDS_PER_HOUR			3600
#define TM_RTC_SECONDS_PER_MINUTE		60

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
 *	- uint32_t unix:
 *		Seconds from 	01.01.1970 00:00:00
 */
typedef struct {
	uint8_t seconds;	//Seconds, 			00-59
	uint8_t minutes;	//Minutes, 			00-59
	uint8_t hours;		//Hours,			00-23
	uint8_t day;		//Day in a week, 	1-7
	uint8_t date;		//Day in a month	1-31
	uint8_t month;		//Month,			1-12
	uint8_t year;		//Year				00-99
	uint32_t unix;		//Unix time
} TM_RTC_Time_t;

/**
 * Set format of date and time
 *
 * - TM_RTC_Format_BIN
 * 	binary format
 * - TM_RTC_Format_BCD
 * 	BCD format
 *
 */
typedef enum {
	TM_RTC_Format_BIN,
	TM_RTC_Format_BCD
} TM_RTC_Format_t;

/**
 * Select interrupt each x time
 *
 */
typedef enum {
	TM_RTC_Int_Disable,		//Disable wakeup interrupt
	TM_RTC_Int_60s,			//Interrupt every 60sec
	TM_RTC_Int_30s,			//Interrupt every 30s
	TM_RTC_Int_15s,			//Interrupt every 15s
	TM_RTC_Int_10s,			//Interrupt every 10s
	TM_RTC_Int_5s,			//Interrupt every 5s
	TM_RTC_Int_2s,			//Interrupt every 2s
	TM_RTC_Int_1s,			//Interrupt every 1s
	TM_RTC_Int_500ms,		//Interrupt every 500ms
	TM_RTC_Int_250ms,		//Interrupt every 250ms
	TM_RTC_Int_125ms		//Interrupt every 125ms
} TM_RTC_Int_t;

/**
 * Use internal or external clock
 *
 * Note: Internal clock is not so accurate
 */
typedef enum {
	TM_RTC_ClockSource_Internal,	//Internal
	TM_RTC_ClockSource_External		//External
} TM_RTC_ClockSource_t;

/**
 * Get seconds from 01.01.1970 00:00:00
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Struct to time data
 *
 * Returns Unix seconds
 */
extern uint32_t TM_RTC_GetUnixTimeStamp(TM_RTC_Time_t* data);

/**
 * Initialize RTC
 *
 * Parameters:
 * 	- TM_RTC_ClockSource_t source:
 * 		Select RTC clock source
 */
extern uint32_t TM_RTC_Init(TM_RTC_ClockSource_t source);

/**
 * Select RTC interrupt
 *
 * Parameters:
 * 	- TM_RTC_Int_t int_value:
 * 		Choose struct member from struct TM_RTC_Int_t
 */
extern void TM_RTC_Interrupts(TM_RTC_Int_t int_value);

/**
 * Set date and time
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Struct to time data
 * 	- TM_RTC_Format_t format:
 * 		Member of struct TM_RTC_Format_t
 */
extern void TM_RTC_SetDateTime(TM_RTC_Time_t* data, TM_RTC_Format_t format);

/**
 * Get date and time
 *
 * Parameters:
 * 	- TM_RTC_Time_t* data:
 * 		Struct to time data where to save data
 * 	- TM_RTC_Format_t format:
 * 		Member of struct TM_RTC_Format_t
 */
extern void TM_RTC_GetDateTime(TM_RTC_Time_t* data, TM_RTC_Format_t format);

/**
 * Custom Request handler for wakeup
 *
 * Called from main handler
 */
extern void TM_RTC_RequestHandler(void);

/**
 * Internal functions
 */
/**
 * RTC Config
 *
 * Internal
 */
extern void TM_RTC_Config(TM_RTC_ClockSource_t source);

#endif

