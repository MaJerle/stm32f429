/**
 *	Keil project example for internal RTC on STM32F429 board
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"

#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_disco.h"

#include <stdio.h>

char buf[30];
TM_RTC_Data_t datatime;


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
    
    //Clock speed to Sysclk/2 = 84MHz
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
    
    TIM_OCDef.TIM_Pulse = 1373; //100% duty cycle
    TIM_OC4Init(TIM4, &TIM_OCDef);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}


int main(void) {
	//Initialize system
	SystemInit();
	
	TM_LEDS_Init();
    TM_TIMER_Init();
    TM_PWM_Init();
	
	TM_DISCO_ButtonInit();
	
	//Initialite onboard LCD
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	//Time from LCD
	sprintf(buf, "Time is: %04X", RTC_ReadBackupRegister(TM_RTC_INIT_OK_REG));
	TM_ILI9341_Puts(10, 10, buf, &TM_Font_11x18, 0xFFFF, 0x5461);
	
	//Initialize RTC with internal 32768Hz clock
	TM_RTC_Init(TM_RTC_ClockSource_Internal);
	
	//Set wakeup interrupt every 1 seconds
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	while (1) {
		if (TM_DISCO_ButtonPressed()) {
			datatime.hours = 23;
			datatime.minutes = 59;
			datatime.seconds = 55;
			datatime.year = 14;
			datatime.month = 5;
			datatime.date = 31;
			datatime.weekDay = 6;
			
			TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
			
			datatime.year = 0;
			datatime.month = 0;
			datatime.date = 0;
			datatime.weekDay = 0;
		}
	}
}

void TM_RTC_RequestHandler() {
	static uint8_t count = 0;
	
	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
	
	sprintf(buf, "%02d.%02d.%04d, day: %d\n%02d:%02d:%02d", datatime.date, datatime.month, datatime.year + 2000, datatime.weekDay, datatime.hours, datatime.minutes, datatime.seconds);
	TM_ILI9341_Puts(10, 30, buf, &TM_Font_11x18, 0xFFFF, 0x1234);

	if (count >= 50) {
		TM_RTC_Interrupts(TM_RTC_Int_125ms);
	} else if (count >= 15) {
		TM_RTC_Interrupts(TM_RTC_Int_250ms);
	} else if (count >= 5) {
		TM_RTC_Interrupts(TM_RTC_Int_500ms);
	}
}
