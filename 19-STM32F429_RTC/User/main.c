/**
 *	Keil project example for internal RTC
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"

#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"

#include <stdio.h>

char buf[30];
TM_RTC_Time_t datatime;


void TM_LEDS_Init() {
    GPIO_InitTypeDef GPIO_InitDef;
    //Clock for GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
 
    //Alternating functions
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
    
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitDef);
}
 
void TM_TIMER_Init() {
    TIM_TimeBaseInitTypeDef TIM_BaseDef;
    
    //Enable clock for TIM4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    //Clock speed to Sysclk/2 = 90MHz
    TIM_BaseDef.TIM_Prescaler = 0;
    TIM_BaseDef.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseDef.TIM_Period = 2746; //10kHz 
    TIM_BaseDef.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseDef.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_BaseDef);
    TIM_Cmd(TIM4, ENABLE);
}
 
void TM_PWM_Init() {
    TIM_OCInitTypeDef TIM_OCDef;
    
    TIM_OCDef.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCDef.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCDef.TIM_OCPolarity = TIM_OCPolarity_Low;
    
 
    TIM_OCDef.TIM_Pulse = 1373; //25% duty cycle
    TIM_OC1Init(TIM4, &TIM_OCDef);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
    
    
char buffer[30];

int main(void) {
	uint8_t old_sek;
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initiaize button
	TM_DISCO_ButtonInit();
	
	TM_DISCO_LedInit();
	
	TM_LEDS_Init();
    TM_TIMER_Init();
    TM_PWM_Init();
	
	//Initialize USART, TX: PB10, RX: PB11
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 115200);
	
	//Initialize RTC with internal 32768Hz clock
	//It's not very accurate
	TM_RTC_Init(TM_RTC_ClockSource_External);
	
	//Set wakeup interrupt every 1 second
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	datatime.hours = 23;
	datatime.minutes = 59;
	datatime.seconds = 58;
	datatime.year = 14;
	datatime.month = 5;
	datatime.date = 31;
	datatime.day = 6;
	//Set new time
	TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
	
	while (1) {
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
		if (datatime.seconds != old_sek) {
			old_sek = datatime.seconds;
			
			TM_DISCO_LedToggle(LED_RED);
			if (datatime.date == 1) {
				TM_DISCO_LedOn(LED_GREEN);
			} else {
				TM_DISCO_LedOff(LED_GREEN);
			}
			
			TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
			sprintf(buffer, "%02d.%02d.  %02d:%02d:%02d\n", datatime.date, datatime.month, datatime.hours, datatime.minutes, datatime.seconds);
			TM_USART_Puts(USART3, buffer);
		}
		
		if (TM_DISCO_ButtonPressed()) {
			datatime.hours = 23;
			datatime.minutes = 59;
			datatime.seconds = 55;
			datatime.year = 14;
			datatime.month = 5;
			datatime.date = 31;
			datatime.day = 6;
			//Set new time
			TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
		}
	}
}

void TM_RTC_RequestHandler() {

}


