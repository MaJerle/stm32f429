#include "tm_stm32f4_rtc.h"

uint32_t TM_RTC_Status = 0;
uint8_t TM_RTC_noResetFlag = 0;
uint16_t uwSynchPrediv = 0xFF, uwAsynchPrediv = 0x7F;

void TM_RTC_Init(TM_RTC_ClockSource_t source) {
	uint32_t status;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Clear Wakeup flag */
	PWR_ClearFlag(PWR_FLAG_WU);
	
	status = RTC_ReadBackupRegister(TM_RTC_INIT_OK_REG);
	if (status != TM_RTC_INIT_OK_VAL) {
		//Initialize RTC
		TM_RTC_Config(source);
	} else {
		// Enable the PWR clock
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		//Already initialized
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		
		if (source == TM_RTC_ClockSource_Internal) {
			TM_RTC_Config(source);
		}
		
		// Allow access to RTC
		PWR_BackupAccessCmd(ENABLE);

		// Wait for RTC APB registers synchronisation
		RTC_WaitForSynchro();
	}
}

void TM_RTC_SetDateTime(TM_RTC_Data_t* data, TM_RTC_Format_t format) {
	RTC_InitTypeDef RTC_InitStruct;
	
	TM_RTC_SetTime(data, format);
	TM_RTC_SetDate(data, format);
	
	// Configure the RTC data register and RTC prescaler
	RTC_InitStruct.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStruct.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStruct);
}

void TM_RTC_SetDate(TM_RTC_Data_t* data, TM_RTC_Format_t format) {
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_DateStruct.RTC_Date = data->date;
	RTC_DateStruct.RTC_Month = data->month;
	RTC_DateStruct.RTC_Year = data->year;
	RTC_DateStruct.RTC_WeekDay = data->weekDay;
	
	if (format == TM_RTC_Format_BCD) {
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStruct);
	} else {
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
	}
}
	
void TM_RTC_SetTime(TM_RTC_Data_t* data, TM_RTC_Format_t format) {
	RTC_TimeTypeDef RTC_TimeStruct;
	
	if (data->hours >= 12) {
		RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
	} else {
		RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
	}
	RTC_TimeStruct.RTC_Hours = data->hours;
	RTC_TimeStruct.RTC_Minutes = data->minutes;
	RTC_TimeStruct.RTC_Seconds = data->seconds;
	
	if (format == TM_RTC_Format_BCD) {
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
	} else {
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
	}
}

void TM_RTC_GetDateTime(TM_RTC_Data_t* data, TM_RTC_Format_t format) {
	/*TM_RTC_noResetFlag = 1;
	TM_RTC_GetDate(data, format);
	TM_RTC_GetTime(data, format);
	TM_RTC_noResetFlag = 0;*/
	
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	
	if (format == TM_RTC_Format_BCD) {
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStruct);
	} else {
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	}
	
	data->year = RTC_DateStruct.RTC_Year;
	data->month = RTC_DateStruct.RTC_Month;
	data->date = RTC_DateStruct.RTC_Date;
	data->weekDay = RTC_DateStruct.RTC_WeekDay;
	
	if (format == TM_RTC_Format_BCD) {
		RTC_GetTime(RTC_Format_BCD, &RTC_TimeStruct);
	} else {
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	}
	
	data->hours = RTC_TimeStruct.RTC_Hours;
	data->minutes = RTC_TimeStruct.RTC_Minutes;
	data->seconds = RTC_TimeStruct.RTC_Seconds;
}

void TM_RTC_GetTime(TM_RTC_Data_t* data, TM_RTC_Format_t format) {
	RTC_TimeTypeDef RTC_TimeStruct;
	/*
	if (TM_RTC_noResetFlag == 0) {
		data->year = 0;
		data->month = 0;
		data->date = 0;
		data->weekDay = 0;
	}*/
	
	if (format == TM_RTC_Format_BCD) {
		RTC_GetTime(RTC_Format_BCD, &RTC_TimeStruct);
	} else {
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	}
	
	data->hours = RTC_TimeStruct.RTC_Hours;
	data->minutes = RTC_TimeStruct.RTC_Minutes;
	data->seconds = RTC_TimeStruct.RTC_Seconds;
}

void TM_RTC_GetDate(TM_RTC_Data_t* data, TM_RTC_Format_t format) {
	RTC_DateTypeDef RTC_DateStruct;
	/*
	if (TM_RTC_noResetFlag == 0) {
		data->hours = 0;
		data->minutes = 0;
		data->seconds = 0;
	}
	*/
	if (format == TM_RTC_Format_BCD) {
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStruct);
	} else {
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	}
	
	data->year = RTC_DateStruct.RTC_Year;
	data->month = RTC_DateStruct.RTC_Month;
	data->date = RTC_DateStruct.RTC_Date;
	data->weekDay = RTC_DateStruct.RTC_WeekDay;
}

void TM_RTC_Config(TM_RTC_ClockSource_t source) {
	RTC_InitTypeDef RTC_InitStruct;

	// Enable the PWR clock
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	// Allow access to RTC
	//PWR_BackupAccessCmd(ENABLE);

	if (source == TM_RTC_ClockSource_Internal) {
		// Enable the LSI OSC 
		RCC_LSICmd(ENABLE);

		// Wait till LSI is ready 
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

		// Select the RTC Clock Source
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	} else if (source == TM_RTC_ClockSource_External) {
		// Enable the LSE OSC
		RCC_LSEConfig(RCC_LSE_ON);

		// Wait till LSE is ready 
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

		// Select the RTC Clock Source
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	}

	// Enable the RTC Clock
	RCC_RTCCLKCmd(ENABLE);

	// Wait for RTC APB registers synchronisation
	RTC_WaitForSynchro();

	// Configure the RTC data register and RTC prescaler
	RTC_InitStruct.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStruct.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStruct);

	// Indicator for the RTC configuration
	RTC_WriteBackupRegister(TM_RTC_INIT_OK_REG, TM_RTC_INIT_OK_VAL);
}

void TM_RTC_Interrupts(TM_RTC_Int_t int_value) {
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	uint32_t int_val;

	//NVIC init for 
	NVIC_InitStruct.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	//RTC connected to EXTI_Line22
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStruct.EXTI_Line = EXTI_Line22;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	if (int_value == TM_RTC_Int_Disable) {
		NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStruct); 
	} else {
		//Enable NVIC
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct); 
		//Enable EXT1 interrupt
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStruct);

		//First disable wake up command
		RTC_WakeUpCmd(DISABLE);

		//Clock divided by 8, 32768 / 8 = 4068
		//4096 ticks for 1second interrupt
		RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);

		if (int_value == TM_RTC_Int_60s) {
			int_val = 0x3BFFF; 		// 60 seconds = 60 * 4096 / 1 = 245760
		} else if (int_value == TM_RTC_Int_30s) {
			int_val = 0x1DFFF;		// 30 seconds
		} else if (int_value == TM_RTC_Int_15s) {
			int_val = 0xEFFF;		// 15 seconds
		} else if (int_value == TM_RTC_Int_10s) {
			int_val = 0x9FFF;		// 10 seconds
		} else if (int_value == TM_RTC_Int_5s) {
			int_val = 0x4FFF;		// 5 seconds
		} else if (int_value == TM_RTC_Int_2s) {
			int_val = 0x1FFF;		// 2 seconds
		} else if (int_value == TM_RTC_Int_1s) {
			int_val = 0x0FFF;		// 1 second
		} else if (int_value == TM_RTC_Int_500ms) {
			int_val = 0x7FF;		// 500 ms
		} else if (int_value == TM_RTC_Int_250ms) {
			int_val = 0x3FF;		// 250 ms
		} else if (int_value == TM_RTC_Int_125ms) {
			int_val = 0x1FF;		// 125 ms
		}		

		//Set RTC wakeup counter
		RTC_SetWakeUpCounter(int_val);
		//Enable wakeup interrupt
		RTC_ITConfig(RTC_IT_WUT, ENABLE);
		//Enable wakeup command
		RTC_WakeUpCmd(ENABLE);
	}
}

void RTC_WKUP_IRQHandler(void) {
	if (RTC_GetITStatus(RTC_IT_WUT) != RESET) {
		//Clear interrupt flags
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		
		//Call user function
		TM_RTC_RequestHandler();
	}
}

