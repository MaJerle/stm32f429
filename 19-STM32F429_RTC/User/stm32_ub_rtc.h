//--------------------------------------------------------------
// File     : stm32_ub_rtc.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_RTC_H
#define __STM32F4_UB_RTC_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rtc.h"


//--------------------------------------------------------------
// Status Register Defines
//--------------------------------------------------------------
#define  RTC_STATUS_REG      RTC_BKP_DR0  // Status Register
#define  RTC_STATUS_INIT_OK  0x35AC       // RTC initialisiert
#define  RTC_STATUS_TIME_OK  0xA3C5       // RTC eingestellt


//--------------------------------------------------------------
// Aktivieren, Deaktivieren von einer Funktion
// 1=aktiv, 0=inaktiv
//--------------------------------------------------------------
#define  RTC_USE_WAKEUP_ISR      1  // WakeUp-Interrupt per RTC

#if RTC_USE_WAKEUP_ISR==1
  // inlcudes bei Interrupt
  #include "stm32f4xx_exti.h"
  #include "misc.h"
#endif


//--------------------------------------------------------------
// WakeUp-Interrupt
//--------------------------------------------------------------
#if RTC_USE_WAKEUP_ISR==1
typedef enum {
  RTC_WAKEUP_STOP =0, // Wakeup Interrupt disable
  RTC_WAKEUP_30s,     // Timer auf 30sec intervall
  RTC_WAKEUP_10s,     // Timer auf 10sec intervall
  RTC_WAKEUP_5s,      // Timer auf 5sec intervall
  RTC_WAKEUP_1s,      // Timer auf 1sec intervall
  RTC_WAKEUP_500ms,   // Timer auf 500msec intervall
  RTC_WAKEUP_250ms,   // Timer auf 250msec intervall
  RTC_WAKEUP_125ms    // Timer auf 125msec intervall
}RTC_WAKEUP_t;
#endif


//--------------------------------------------------------------
// Zahlendarstellung der RTC-Struktur
//--------------------------------------------------------------
typedef enum {
  RTC_DEC = 0,  // Dezimal [8,9,10,11,12 usw]
  RTC_HEX,      // Hex     [0x08,0x09,0x10,0x11,0x12 usw]
}RTC_FORMAT_t;


//--------------------------------------------------------------
// Status der RTC beim init
//--------------------------------------------------------------
typedef enum {
  RTC_UNDEFINED =0, // RTC war noch nicht initialisiert
  RTC_INIT_OK,      // RTC war schon initialisiert
  RTC_TIME_OK       // RTC war schon eingestellt
}RTC_STATUS_t;



//--------------------------------------------------------------
// Struktur der RTC
//--------------------------------------------------------------
typedef struct {
  RTC_STATUS_t status;
  uint8_t std;     // studen   [0...23]
  uint8_t min;     // minuten  [0...59]
  uint8_t sek;     // sekunden [0...59]
  uint8_t tag;     // tag      [1...31]
  uint8_t monat;   // monat    [1...12]
  uint8_t jahr;    // jahr     [0...99]
  uint8_t wotag;   // wochentag [1...7] 1=Montag
}RTC_t;
RTC_t UB_RTC;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
RTC_STATUS_t UB_RTC_Init(void);
void UB_RTC_SetClock(RTC_FORMAT_t format);
void UB_RTC_GetClock(RTC_FORMAT_t format);
#if RTC_USE_WAKEUP_ISR==1
void UB_RTC_SetWakeUpInterrupt(RTC_WAKEUP_t wakeup);
#endif




//--------------------------------------------------------------
#endif // __STM32F4_UB_RTC_H
