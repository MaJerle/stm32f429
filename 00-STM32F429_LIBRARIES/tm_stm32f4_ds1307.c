#include "tm_stm32f4_ds1307.h"

uint8_t TM_DS1307_Months[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

void TM_DS1307_Init(void) {
	TM_I2C_Init(TM_DS1307_I2C, TM_DS1307_I2C_PINSPACK, 50000);
}

void TM_DS1307_GetDateTime(TM_DS1307_Time_t* time) {
	uint8_t data[7];
	TM_I2C_ReadMulti(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_SECONDS, data, 7);
	
	time->seconds = TM_DS1307_Bcd2Bin(data[TM_DS1307_SECONDS]);
	time->minutes = TM_DS1307_Bcd2Bin(data[TM_DS1307_MINUTES]);
	time->hours = TM_DS1307_Bcd2Bin(data[TM_DS1307_HOURS]);
	time->day = TM_DS1307_Bcd2Bin(data[TM_DS1307_DAY]);
	time->date = TM_DS1307_Bcd2Bin(data[TM_DS1307_DATE]);
	time->month = TM_DS1307_Bcd2Bin(data[TM_DS1307_MONTH]);
	time->year = TM_DS1307_Bcd2Bin(data[TM_DS1307_YEAR]);
}

void TM_DS1307_SetDateTime(TM_DS1307_Time_t* time) {
	uint8_t data[7];
	
	data[TM_DS1307_SECONDS] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->seconds, 0, 59));
	data[TM_DS1307_MINUTES] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->minutes, 0, 59));
	data[TM_DS1307_HOURS] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->hours, 0, 23));
	data[TM_DS1307_DAY] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->day, 1, 7));
	data[TM_DS1307_DATE] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->date, 1, 31));
	data[TM_DS1307_MONTH] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->month, 1, 12));
	data[TM_DS1307_YEAR] = TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(time->year, 0, 99));
	
	TM_I2C_WriteMulti(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_SECONDS, data, 7);
}

uint8_t TM_DS1307_GetSeconds(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_SECONDS));
}

uint8_t TM_DS1307_GetMinutes(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_MINUTES));
}

uint8_t TM_DS1307_GetHours(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_HOURS));
}

uint8_t TM_DS1307_GetDay(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_DAY));
}

uint8_t TM_DS1307_GetDate(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_DATE));
}

uint8_t TM_DS1307_GetMonth(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_MONTH));
}

uint8_t TM_DS1307_GetYear(void) {
	return TM_DS1307_Bcd2Bin(TM_I2C_Read(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_YEAR));
}


void TM_DS1307_SetSeconds(uint8_t seconds) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_SECONDS, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(seconds, 0, 59)));
}

void TM_DS1307_SetMinutes(uint8_t minutes) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_MINUTES, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(minutes, 0, 59)));
}
void TM_DS1307_SetHours(uint8_t hours) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_HOURS, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(hours, 0, 23)));
}

void TM_DS1307_SetDay(uint8_t day) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_DAY, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(day, 1, 7)));
}

void TM_DS1307_SetDate(uint8_t date) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_DATE, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(date, 1, 31)));
}

void TM_DS1307_SetMonth(uint8_t month) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_MONTH, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(month, 1, 12)));
}

void TM_DS1307_SetYear(uint8_t year) {
	TM_I2C_Write(TM_DS1307_I2C, TM_DS1307_I2C_ADDR, TM_DS1307_YEAR, TM_DS1307_Bin2Bcd(TM_DS1307_CheckMinMax(year, 0, 99)));
}


uint8_t TM_DS1307_Bcd2Bin(uint8_t bcd) {
	uint8_t dec = 10 * (bcd >> 4);
	dec += bcd & ~(0xF0);
	return dec;
}

uint8_t TM_DS1307_Bin2Bcd(uint8_t bin) {
	uint8_t low = 0;
	uint8_t high = 0;
	
	//High nibble
	high = bin / 10;
	//Low nibble
	low = bin - (high * 10);
	
	return high << 4 | low;
}

uint8_t TM_DS1307_CheckMinMax(uint8_t val, uint8_t min, uint8_t max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}

