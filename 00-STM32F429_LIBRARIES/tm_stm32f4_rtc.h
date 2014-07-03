/**
 * Internal RTC library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://bit.ly/1oxFaDK
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_RTC_H
#define TM_RTC_H 100

#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "defines.h"

#ifndef TM_RTC_NVIC_
#define TM_RTC_NVIC_
#endif
#ifndef TM_RTC_NVIC_2
#define TM_RTC_NVIC_2
#endif


#define  RTC_STATUS_REG      	RTC_BKP_DR0  // Status Register
#define  RTC_STATUS_INIT_OK  	0x35AC       // RTC initialised
#define  RTC_STATUS_TIME_OK  	0xA3C5       // RTC time ok

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
} TM_RTC_Time_t;

typedef enum {
	TM_RTC_Format_BIN,
	TM_RTC_Format_BCD
} TM_RTC_Format_t;

typedef enum {
	TM_RTC_Int_Disable,
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

typedef enum {
	TM_RTC_ClockSource_Internal,
	TM_RTC_ClockSource_External
} TM_RTC_ClockSource_t;

extern uint32_t TM_RTC_Init(TM_RTC_ClockSource_t source);

extern void TM_RTC_Interrupts(TM_RTC_Int_t int_value);

extern void TM_RTC_Config(TM_RTC_ClockSource_t source);

extern void TM_RTC_SetDateTime(TM_RTC_Time_t* data, TM_RTC_Format_t format);

extern void TM_RTC_GetTime(TM_RTC_Time_t* data, TM_RTC_Format_t format);

extern void TM_RTC_GetDate(TM_RTC_Time_t* data, TM_RTC_Format_t format);
	
extern void TM_RTC_GetDateTime(TM_RTC_Time_t* data, TM_RTC_Format_t format);

extern void TM_RTC_RequestHandler(void);

#endif

