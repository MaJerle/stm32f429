#include "tm_stm32f4_rtc.h"

uint32_t TM_RTC_Status = RTC_STATUS_ZERO;
uint16_t uwSynchPrediv = 0xFF, uwAsynchPrediv = 0x7F;

RTC_TimeTypeDef RTC_TimeStruct;
RTC_InitTypeDef RTC_InitStruct;
RTC_DateTypeDef RTC_DateStruct;

uint8_t TM_RTC_Months[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},	//Not leap year
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}	//Leap year
};

uint32_t TM_RTC_Init(TM_RTC_ClockSource_t source) {
	uint32_t status;
	uint8_t stat = 1;
	TM_RTC_Time_t datatime;
	
	// Enable RTC Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	// Allow access to BKP Domain
	PWR_BackupAccessCmd(ENABLE);
	
	// Get RTC status
	status = RTC_ReadBackupRegister(RTC_STATUS_REG);
	
	if (status == RTC_STATUS_TIME_OK) {
		TM_RTC_Status = RTC_STATUS_TIME_OK;
		
		//Start internal clock if we choose internal clock
		if (source == TM_RTC_ClockSource_Internal) {
			TM_RTC_Config(TM_RTC_ClockSource_Internal);
		}
		
		// Wait for RTC APB registers synchronisation (needed after start-up from Reset)
		RTC_WaitForSynchro();
		
		// Clear interrupt flags
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
	} else if (status == RTC_STATUS_INIT_OK) {
		TM_RTC_Status = RTC_STATUS_INIT_OK;
		
		//Start internal clock if we choose internal clock
		if (source == TM_RTC_ClockSource_Internal) {
			TM_RTC_Config(TM_RTC_ClockSource_Internal);
		}
		
		// Wait for RTC APB registers synchronisation (needed after start-up from Reset)
		RTC_WaitForSynchro();
		
		//Clear interrupt flags
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
	} else {
		TM_RTC_Status = RTC_STATUS_ZERO;
		//Return status = 0 > RTC Never initialized before
		stat = RTC_STATUS_ZERO;
		// Config RTC
		TM_RTC_Config(source);
		
		//Set date and time
		datatime.date = 1;
		datatime.day = 1;
		datatime.month = 1;
		datatime.year = 0;
		datatime.hours = 0;
		datatime.minutes = 0;
		datatime.seconds = 0;
		TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
		
		// Initialized OK
		TM_RTC_Status = RTC_STATUS_INIT_OK;
	}
	//If first time initialized
	if (stat == RTC_STATUS_ZERO) {
		return 0;
	}
	return TM_RTC_Status;
}

void TM_RTC_SetDateTime(TM_RTC_Time_t* data, TM_RTC_Format_t format) {	
	// Fill time
	RTC_TimeStruct.RTC_Hours = data->hours;
	RTC_TimeStruct.RTC_Minutes = data->minutes;
	RTC_TimeStruct.RTC_Seconds = data->seconds;
	// Fill date
	RTC_DateStruct.RTC_Date = data->date;
	RTC_DateStruct.RTC_Month = data->month;
	RTC_DateStruct.RTC_Year = data->year;
	RTC_DateStruct.RTC_WeekDay = data->day;
	
	// Set the RTC time base to 1s and hours format to 24h
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
	RTC_InitStruct.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStruct.RTC_SynchPrediv = uwSynchPrediv;
	RTC_Init(&RTC_InitStruct);

	if (format == TM_RTC_Format_BCD) {
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
	} else {
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
	}
	
	if (format == TM_RTC_Format_BCD) {
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStruct);
	} else {
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
	}	
	
	if (TM_RTC_Status != RTC_STATUS_ZERO) {
		// Write backup registers
		RTC_WriteBackupRegister(RTC_STATUS_REG, RTC_STATUS_TIME_OK);
	}
}

void TM_RTC_GetDateTime(TM_RTC_Time_t* data, TM_RTC_Format_t format) {
	uint32_t unix;

	//Get time
	if (format == TM_RTC_Format_BIN) {
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	} else {
		RTC_GetTime(RTC_Format_BCD, &RTC_TimeStruct);
	}
	
	data->hours = RTC_TimeStruct.RTC_Hours;
	data->minutes = RTC_TimeStruct.RTC_Minutes;
	data->seconds = RTC_TimeStruct.RTC_Seconds;
	
	//Get date
	if (format == TM_RTC_Format_BIN) {
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	} else {
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStruct);
	}
	
	data->year = RTC_DateStruct.RTC_Year;
	data->month = RTC_DateStruct.RTC_Month;
	data->date = RTC_DateStruct.RTC_Date;
	data->day = RTC_DateStruct.RTC_WeekDay;
	
	//Calculate unix offset
	unix = TM_RTC_GetUnixTimeStamp(data);
	data->unix = unix;
}

void TM_RTC_Config(TM_RTC_ClockSource_t source) {
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
	
	// Wait for register synchronization
	RTC_WaitForSynchro();

	// Write status
	RTC_WriteBackupRegister(RTC_STATUS_REG, RTC_STATUS_INIT_OK);
}

void TM_RTC_Interrupts(TM_RTC_Int_t int_value) {
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	uint32_t int_val;
	
	// Clear pending bit
	EXTI_ClearITPendingBit(EXTI_Line22);

	// NVIC init for RTC
	NVIC_InitStruct.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	// RTC connected to EXTI_Line22
	EXTI_InitStruct.EXTI_Line = EXTI_Line22;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	
	if (int_value == TM_RTC_Int_Disable) {
		// Disable wakeup interrupt
		RTC_WakeUpCmd(DISABLE);
		// Disable RTC interrupt flag
		RTC_ITConfig(RTC_IT_WUT, DISABLE);
		
		// Disable NVIC
		NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStruct); 
		// Disable EXT1 interrupt
		EXTI_InitStruct.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStruct);
	} else {
		// Enable NVIC
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct); 
		//Enable EXT1 interrupt
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStruct);

		// First disable wake up command
		RTC_WakeUpCmd(DISABLE);

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

		// Clock divided by 8, 32768 / 8 = 4068
		// 4096 ticks for 1second interrupt
		RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);
		
		// Set RTC wakeup counter
		RTC_SetWakeUpCounter(int_val);
		// Enable wakeup interrupt
		RTC_ITConfig(RTC_IT_WUT, ENABLE);
		// Enable wakeup command
		RTC_WakeUpCmd(ENABLE);
	}
}

uint32_t TM_RTC_GetUnixTimeStamp(TM_RTC_Time_t* data) {
	uint32_t days = 0, seconds = 0;
	uint16_t i;
	uint16_t year = (uint16_t) (data->year + 2000);
	//Year is below offset year
	if (year < TM_RTC_OFFSET_YEAR) {
		return 0;
	}
	//Days in back years
	for (i = TM_RTC_OFFSET_YEAR; i < year; i++) {
		days += TM_RTC_DAYS_IN_YEAR(i);
	}
	//Days in current year
	data->month = data->month % 12;
	for (i = 1; i < data->month; i++) {
		days += TM_RTC_Months[TM_RTC_LEAP_YEAR(year)][i];
	}
	//Day starts with 1
	days += data->date - 1;
	seconds = days * TM_RTC_SECONDS_PER_DAY;
	seconds += data->hours * TM_RTC_SECONDS_PER_HOUR;
	seconds += data->minutes * TM_RTC_SECONDS_PER_MINUTE;
	seconds += data->seconds;
	
	//seconds = days * 86400;
	return seconds;
}

uint16_t TM_RTC_DaysInYear(uint16_t year) {
	return TM_RTC_DAYS_IN_YEAR(year);
}

void TM_RTC_GetDateTimeFromUnix(TM_RTC_Time_t* data, uint32_t unix) {
	uint16_t year;
	
	//Store unix time to unix in struct
	data->unix = unix;
	//Get seconds from unix
	data->seconds = unix % 60;
	//Go to minutes
	unix /= 60;
	//Get minutes
	data->minutes = unix % 60;
	//Go to hours
	unix /= 60;
	//Get hours
	data->hours = unix % 24;
	//Go to days
	unix /= 24;
	
	//Get week day
	//Monday is day one
	data->day = (unix + 3) % 7 + 1;

	//Get year
	year = 1970;
	while (1) {
		if (TM_RTC_LEAP_YEAR(year) && unix >= 366) {
			unix -= 366;
		} else if (unix >= 365) {
			unix -= 365;
		} else {
			break;
		}
		year++;
	}
	//Get year in xx format
	data->year = (uint8_t) (year - 2000);
	//Get month
	for (data->month = 0; data->month < 12; data->month++) {
		if (TM_RTC_LEAP_YEAR(year) && unix >= (uint32_t)TM_RTC_Months[1][data->month]) {
			unix -= TM_RTC_Months[1][data->month];
		} else if (unix >= (uint32_t)TM_RTC_Months[0][data->month]) {
			unix -= TM_RTC_Months[0][data->month];
		} else {
			break;
		}
	}
	//Get month
	//Month starts with 1
	data->month++;
	//Get date
	//Date starts with 1
	data->date = unix + 1;
}

void RTC_WKUP_IRQHandler(void) {
	if (RTC_GetITStatus(RTC_IT_WUT) != RESET) {
		// Clear interrupt flags
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		
		// Call user function
		TM_RTC_RequestHandler();
	}
}

