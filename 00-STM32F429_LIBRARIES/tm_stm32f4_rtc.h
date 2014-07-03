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

#define TM_RTC_INIT_OK_REG		RTC_BKP_DR0
#define TM_RTC_INIT_OK_VAL		0x32F2

typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t weekDay;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} TM_RTC_Data_t;

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

extern void TM_RTC_Init(TM_RTC_ClockSource_t source);

extern void TM_RTC_Interrupts(TM_RTC_Int_t int_value);

extern void TM_RTC_Config(TM_RTC_ClockSource_t source);

extern void TM_RTC_SetDateTime(TM_RTC_Data_t* data, TM_RTC_Format_t format);

extern void TM_RTC_SetDate(TM_RTC_Data_t* data, TM_RTC_Format_t format);
	
extern void TM_RTC_SetTime(TM_RTC_Data_t* data, TM_RTC_Format_t format);

extern void TM_RTC_GetTime(TM_RTC_Data_t* data, TM_RTC_Format_t format);

extern void TM_RTC_GetDate(TM_RTC_Data_t* data, TM_RTC_Format_t format);
	
extern void TM_RTC_GetDateTime(TM_RTC_Data_t* data, TM_RTC_Format_t format);

extern void TM_RTC_RequestHandler(void);

#endif

