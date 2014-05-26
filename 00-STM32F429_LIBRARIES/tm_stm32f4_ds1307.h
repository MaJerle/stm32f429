/**
 * DS1307 Library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_DS1307_
#define TM_DS1307_ 100
/**
 * Library dependencies
 *
 * TM I2C library
 * defines.h
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_i2c.h"
#include "defines.h"

//I2C settings for I2C library
#ifndef TM_DS1307_I2C
#define TM_DS1307_I2C				I2C1
#define TM_DS1307_I2C_PINSPACK		TM_I2C_PinsPack_1
#endif

//I2C slave address
#define TM_DS1307_I2C_ADDR	0xD0

//Registers location
#define TM_DS1307_SECONDS	0x00
#define TM_DS1307_MINUTES	0x01
#define TM_DS1307_HOURS		0x02
#define TM_DS1307_DAY		0x03
#define TM_DS1307_DATE		0x04
#define TM_DS1307_MONTH		0x05
#define TM_DS1307_YEAR		0x06
#define TM_DS1307_CONTROL	0x07

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
} TM_DS1307_Time_t;

/**
 * Initialize DS1307 RTC library
 *
 * No parameters
 *
 * No returns
 */
extern void TM_DS1307_Init(void);

/**
 * Get seconds from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns seconds from DS1307, 00 to 59
 */
extern uint8_t TM_DS1307_GetSeconds(void);

/**
 * Get minutes from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns minutes from DS1307, 00 to 59
 */
extern uint8_t TM_DS1307_GetMinutes(void);

/**
 * Get hours from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns hours from DS1307, 00 to 23
 */
extern uint8_t TM_DS1307_GetHours(void);

/**
 * Get day in a week from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns day in a week from DS1307, 1 to 7
 */
extern uint8_t TM_DS1307_GetDay(void);

/**
 * Get date from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns date from DS1307, 01 to 31
 */
extern uint8_t TM_DS1307_GetDate(void);

/**
 * Get month from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns month from DS1307, 01 to 12
 */
extern uint8_t TM_DS1307_GetMonth(void);

/**
 * Get year from DS1307
 *
 * Parameters:
 * 	None
 *
 * Returns year from DS1307, 00 (2000) to 99 (2099)
 */
extern uint8_t TM_DS1307_GetYear(void);



/**
 * Set seconds to DS1307
 *
 * Parameters:
 * 	- uint8_t seconds
 * 		Seconds to be set to DS1307, 01 to 59
 *
 * No returns
 */
extern void TM_DS1307_SetSeconds(uint8_t seconds);

/**
 * Set minutes to DS1307
 *
 * Parameters:
 * 	- uint8_t minutes
 * 		Minutes to be set to DS1307, 01 to 59
 *
 * No returns
 */
extern void TM_DS1307_SetMinutes(uint8_t minutes);

/**
 * Set hours to DS1307
 *
 * Parameters:
 * 	- uint8_t hours
 * 		Hours to be set to DS1307, 01 to 23
 *
 * No returns
 */
extern void TM_DS1307_SetHours(uint8_t hours);

/**
 * Set day to DS1307
 *
 * Parameters:
 * 	- uint8_t day
 * 		Day to be set to DS1307, 1 to 7
 *
 * No returns
 */
extern void TM_DS1307_SetDay(uint8_t day);

/**
 * Set date to DS1307
 *
 * Parameters:
 * 	- uint8_t date
 * 		Date to be set to DS1307, 01 to 31
 *
 * No returns
 */
extern void TM_DS1307_SetDate(uint8_t date);

/**
 * Set month to DS1307
 *
 * Parameters:
 * 	- uint8_t month
 * 		Month to be set to DS1307, 01 to 12
 *
 * No returns
 */
extern void TM_DS1307_SetMonth(uint8_t month);

/**
 * Set year to DS1307
 *
 * Parameters:
 * 	- uint8_t year
 * 		Year to be set to DS1307, 00 (2000) to 99 (2099)
 *
 * No returns
 */
extern void TM_DS1307_SetYear(uint8_t year);

/**
 * Get full date and time from DS1307
 *
 * Parameters:
 * 	- TM_DS1307_Time_t* time
 * 		Pointer to struct where to set data
 *
 * No returns
 */
extern void TM_DS1307_GetDateTime(TM_DS1307_Time_t* time);

/**
 * Set full date and time to DS1307
 *
 * Parameters:
 * 	- TM_DS1307_Time_t* time
 * 		Pointer to struct where to get data
 *
 * No returns
 */
extern void TM_DS1307_SetDateTime(TM_DS1307_Time_t* time);

/**
 * Convert BCD to BIN data
 *
 * Parameters:
 * 	- uint8_t bcd
 * 		BCD value to be converted
 *
 * Returns BIN value of given BCD
 */
extern uint8_t TM_DS1307_Bcd2Bin(uint8_t bcd);

/**
 * Convert BIN to BCD data
 *
 * Parameters:
 * 	- uint8_t bin
 * 		BIN value to be converted
 *
 * Returns BCD value of given BIN
 */
extern uint8_t TM_DS1307_Bin2Bcd(uint8_t bin);

/**
 * Checks for min and max values, which can be stored to DS1307
 *
 * Parameters:
 * 	- uint8_t val
 * 		Value to be checked
 * 	- uint8_t min
 * 		Minimal allowed value of val
 * 	- uint8_t max
 * 		Maximal allowed value of val
 *
 * Returns value between min and max
 */
extern uint8_t TM_DS1307_CheckMinMax(uint8_t val, uint8_t min, uint8_t max);

#endif

