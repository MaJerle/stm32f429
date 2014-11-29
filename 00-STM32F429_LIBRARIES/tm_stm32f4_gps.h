/**
 *	GPS NMEA standard data parser for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-27-gps-stm32f4-devices/
 *	@version 	v1.1
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 * 
 * Version v1.1
 * 	- 22.08.2014
 * 	- Added support for calculating distance between 2 coordinates and bearing in degrees
 *
 * This library parses response from GPS module, in NMEA statements format.
 * 
 * For more, how GPS returns data, look at link below:
 *
 *	http://aprs.gids.nl/nmea/
 *
 * There is a lot of possible sentences, which can be returned from GPS.
 * This library uses only 4. But some GPS don's returns all of this 4 sentences,
 * so I made a possibility to disable them.
 *
 * By default, these statements are in use:
 * 	- GPGGA: Global Positioning System Fix Data
 * 		- Latitude
 * 		- Longitude
 * 		- Altitude
 * 		- Fix
 * 		- Satellites in use
 * 		- Time
 *	- GPRMC: Recommended minimum specific GPS/Transit data
 *		- Date
 *		- Speed
 *		- Validity
 *		- Direction
 *	- GPGSA: GPS DOP and Active Satellites
 *		- HDOP
 *		- VDOP
 *		- PDOP
 *		- FixMode
 *		- Id's of satellites in use
 *	- GPGSV: GPS Satellites in view
 *		- Satellites in view
 *
 * By default, each of this data has to be detected in order to get "VALID" data.
 * If your GPS does not return any of this statement, you can disable option.
 * If you disable any of statements, then you will loose data, corresponding to statement.
 *
 * Add lines below in your defines.h file and uncomment your settings
 *
 *  //#define GPS_DISABLE_GPGGA 	//Disable GPGGA statement
 *  //#define GPS_DISABLE_GPRMC		//Disable GPRMC statement
 *  //#define GPS_DISABLE_GPGSA		//Disable GPGSA statement
 *  //#define GPS_DISABLE_GPGSV		//Disable GPGSV statement
 *
 * To communicate with GPS, USART is commonly used. By default, my library uses USART1,
 * but if you want your own USARTx, add lines below in your defines.h file:
 *
 * 	#define GPS_USART				USART1
 * 	#define GPS_USART_PINSPACK		TM_USART_PinsPack_2
 *
 * With default pinout:
 *
 *	STM32F4xx TX = PB6
 * 	STM32F4xx RX = PB7
 *
 * 	Connect GPS's TX to STM32F4xx's RX.
 *
 */
#ifndef TM_GPS_H
#define TM_GPS_H	110
/**
 * Dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - TM USART
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_disco.h"
#include "defines.h"
#include "math.h"

/* Default GPS USART used */
#ifndef GPS_USART
#define GPS_USART				USART1
#define GPS_USART_PINSPACK		TM_USART_PinsPack_2
#endif

/* Is character a digit */
#define GPS_IS_DIGIT(x)			((x) >= '0' && (x) <= '9')

/* Char 2 digit conversions */
#define GPS_C2N(a)				(((a) - 48))
#define GPS_C2NM(a, x)			C2N(a) * (x)
#define GPS_CONCAT(x, y)		((x) << 5 | (y))

/* NMEA statements */
#define GPS_GPGGA				0
#define GPS_GPRMC				1
#define GPS_GPGSA				2
#define GPS_GPGSV				3
#define GPS_ERR					4

/* GPGGA Flags */
#define GPS_FLAG_LATITUDE		0x00000001	//GPGGA
#define GPS_FLAG_LONGITUDE		0x00000002	//GPGGA
#define GPS_FLAG_SATS			0x00000004	//GPGGA
#define GPS_FLAG_FIX			0x00000008	//GPGGA
#define GPS_FLAG_ALTITUDE		0x00000010	//GPGGA
#define GPS_FLAG_EW				0x00000020	//GPGGA
#define GPS_FLAG_NS				0x00000040	//GPGGA
#define GPS_FLAG_TIME			0x00000080	//GPGGA
/* GPRMC Flags */
#define GPS_FLAG_SPEED			0x00000100	//GPRMC
#define GPS_FLAG_DATE			0x00000200	//GPRMC
#define GPS_FLAG_VALIDITY		0x00000400	//GPRMC
#define GPS_FLAG_DIRECTION		0x00008000	//GPRMC
/* GPGSA Flags */
#define GPS_FLAG_HDOP			0x00000800	//GPGSA
#define GPS_FLAG_VDOP			0x00001000	//GPGSA
#define GPS_FLAG_PDOP			0x00002000	//GPGSA
#define GPS_FLAG_FIXMODE		0x00004000	//GPGSA
#define GPS_FLAG_SATS1_12		0x00020000	//GPGSA
/* GPGSV Flags */
#define GPS_FLAG_SATSINVIEW		0x00010000	//GPGSV

/* GPGGA Positions */
#define GPS_POS_LATITUDE		GPS_CONCAT(GPS_GPGGA, 2)	//
#define GPS_POS_LONGITUDE		GPS_CONCAT(GPS_GPGGA, 4)	//
#define GPS_POS_SATS			GPS_CONCAT(GPS_GPGGA, 7)	//
#define GPS_POS_FIX				GPS_CONCAT(GPS_GPGGA, 6)	//
#define GPS_POS_ALTITUDE		GPS_CONCAT(GPS_GPGGA, 9)	//
#define GPS_POS_TIME			GPS_CONCAT(GPS_GPGGA, 1)	//
#define GPS_POS_EW				GPS_CONCAT(GPS_GPGGA, 5)	//
#define GPS_POS_NS				GPS_CONCAT(GPS_GPGGA, 3)	//
/* GPRMC Positions */
#define GPS_POS_SPEED			GPS_CONCAT(GPS_GPRMC, 7)	//
#define GPS_POS_DATE			GPS_CONCAT(GPS_GPRMC, 9)	//
#define GPS_POS_VALIDITY		GPS_CONCAT(GPS_GPRMC, 2)	//
#define GPS_POS_DIRECTION		GPS_CONCAT(GPS_GPRMC, 8)	//
/* GPGSA Positions */
#define GPS_POS_PDOP			GPS_CONCAT(GPS_GPGSA, 15)	//
#define GPS_POS_HDOP			GPS_CONCAT(GPS_GPGSA, 16)	//
#define GPS_POS_VDOP			GPS_CONCAT(GPS_GPGSA, 17)	//
#define GPS_POS_FIXMODE			GPS_CONCAT(GPS_GPGSA, 2)	//

#define GPS_POS_SAT1			GPS_CONCAT(GPS_GPGSA, 3)	//
#define GPS_POS_SAT2			GPS_CONCAT(GPS_GPGSA, 4)	//
#define GPS_POS_SAT3			GPS_CONCAT(GPS_GPGSA, 5)	//
#define GPS_POS_SAT4			GPS_CONCAT(GPS_GPGSA, 6)	//
#define GPS_POS_SAT5			GPS_CONCAT(GPS_GPGSA, 7)	//
#define GPS_POS_SAT6			GPS_CONCAT(GPS_GPGSA, 8)	//
#define GPS_POS_SAT7			GPS_CONCAT(GPS_GPGSA, 9)	//
#define GPS_POS_SAT8			GPS_CONCAT(GPS_GPGSA, 10)	//
#define GPS_POS_SAT9			GPS_CONCAT(GPS_GPGSA, 11)	//
#define GPS_POS_SAT10			GPS_CONCAT(GPS_GPGSA, 12)	//
#define GPS_POS_SAT11			GPS_CONCAT(GPS_GPGSA, 13)	//
#define GPS_POS_SAT12			GPS_CONCAT(GPS_GPGSA, 14)	//

/* GPGSV Positions */
#define GPS_POS_SATSINVIEW		GPS_CONCAT(GPS_GPGSV, 3)	//

/* Earth radius */
#define GPS_EARTH_RADIUS		6371

/* Degrees to radians converter */
#define GPS_DEGREES2RADIANS(x)	((x) * (float)0.01745329251994)
/* Radians to degrees */
#define GPS_RADIANS2DEGREES(x)	((x) * (float)57.29577951308232)

/**
 * Possible return statements
 *
 * 	- TM_GPS_Result_NewData:
 * 		We received completely new data, which were never read.
 * 	- TM_GPS_Result_OldData:
 * 		We already read this data, no new data.
 * 	- TM_GPS_Result_FirstDataWaiting:
 * 		We never receive any useful data.
 * 		Returned at the beginning of the program
 */
typedef enum {
	TM_GPS_Result_NewData,
	TM_GPS_Result_OldData,
	TM_GPS_Result_FirstDataWaiting,
} TM_GPS_Result_t;

/**
 * Enumeration for speed conversion.
 *
 * GPS returns speed in knots, if you want to display a human value,
 * use this enumeration with TM_GPS_ConvertSpeed function
 */
typedef enum {
	/* Metric */
	TM_GPS_Speed_KilometerPerSecond,	
	TM_GPS_Speed_MeterPerSecond,
	TM_GPS_Speed_KilometerPerHour,
	TM_GPS_Speed_MeterPerMinute,
	/* Imperial */
	TM_GPS_Speed_MilePerSecond,
	TM_GPS_Speed_MilePerHour,
	TM_GPS_Speed_FootPerSecond,
	TM_GPS_Speed_FootPerMinute,
	/* For Runners and Joggers */
	TM_GPS_Speed_MinutePerKilometer,
	TM_GPS_Speed_SecondPerKilometer,
	TM_GPS_Speed_SecondPer100Meters,
	TM_GPS_Speed_MinutePerMile,
	TM_GPS_Speed_SecondPerMile,
	TM_GPS_Speed_SecondPer100Yards,
	/* Nautical */
	TM_GPS_Speed_SeaMilePerHour,
} TM_GPS_Speed_t;

/**
 * GPS has longitude, latitude, altitude, etc
 * in float format.
 * But, if you want to make a tracker and save data to SD card, there is a problem,
 * because FATFS library breaks %f with sprintf() function.
 * For that purpose I made a new function TM_GPS_ConvertFloat() which converts float number
 * to an integer and decimal part.
 *
 * Parameters:
 * 	- int32_t Integer:
 * 		- Integer part of float number
 * 	- uint32_t Decimal:
 * 		- Decimal part of float number in integer format
 */
typedef struct {
	int32_t Integer;
	uint32_t Decimal;
} TM_GPS_Float_t;

/**
 * Date struct for GPS
 *
 * Parameters:
 * 	- uint8_t Date:
 * 	- uint8_t Month:
 * 	- uint8_t Year:
 */
typedef struct {
	uint8_t Date;					/* GPGMC */
	uint8_t Month;					/* GPGMC */
	uint8_t Year;					/* GPGMC */
} TM_GPS_Date_t;

/**
 * Time struct for GPS
 *
 * Parameters:
 * 	- uint8_t Hours:
 * 	- uint8_t Minutes:
 * 	- uint8_t Seconds:
 * 	- uint8_t Hundredths:
 */
typedef struct {
	uint8_t Hours;					/* GPGGA */
	uint8_t Minutes;				/* GPGGA */			
	uint8_t Seconds;				/* GPGGA */
	uint8_t Hundredths;				/* GPGGA */
} TM_GPS_Time_t;

/**
 * Main struct to work with GPS
 *
 * Parameters in this struct depends on which GPS NMEA statements you have activated.
 * By default all parameters are active. These are.
 *
 *  - float Latitude:
 *  	Latitude. From -90 to 90 degrees response
 *  - float Longitude:
 *  	Longitude, from -180 to 180 degrees response
 *  - uint8_t Satellites:
 *  	Number of satellites in use
 *  - uint8_t Fix:
 *  	Fix.
 *  	0 = Invalid
 *  	1 = GPS Fix
 *  	2 = DGPS Fix
 *  - float Altitude:
 *  	Altitude above the sea
 *  - TM_GPS_Time_t Time:
 *  	Time struct
 *  - TM_GPS_Date_t Date:
 *  	Date struct
 *  - float Speed:
 *  	Speed in knots
 *  - uint8_t Validity:
 *  	GPS Signal valid?
 *  	1 = Valid
 *  	0 = Invalid
 *  - float Direction:
 *  	Course on the ground
 *  - float HDOP:
 *  	Horizontal dilution of precision
 *  - float PDOP:
 *  	Position dilution od precision
 *  - float VDOP:
 *  	Vertical dilution of precision
 *  - uint8_t FixMode:
 *  	Current fix mode in use:
 *		1 = Fix not available
 *		2 = 2D
 *		3 = 3D
 *  - uint8_t SatelliteIDs[12]:
 *  	Array with IDs of satellites in use.
 *  	Only first data are valid, so if you have 5 satellites in use, only SatelliteIDs[4:0] are valid
 *  - uint8_t SatellitesInView:
 *  	Number of satellites in view
 */
typedef struct {
#ifndef GPS_DISABLE_GPGGA
	float Latitude;					/* GPGGA */
	float Longitude;				/* GPGGA */
	uint8_t Satellites;				/* GPGGA */
	uint8_t Fix;					/* GPGGA */
	float Altitude;					/* GPGGA */
	TM_GPS_Time_t Time;				/* GPGGA */
#endif
#ifndef GPS_DISABLE_GPRMC
	TM_GPS_Date_t Date;				/* GPRMC */
	float Speed;					/* GPRMC */
	uint8_t Validity;				/* GPRMC */
	float Direction;				/* GPRMC */
#endif
#ifndef GPS_DISABLE_GPGSA
	float HDOP;						/* GPGSA */
	float PDOP;						/* GPGSA */
	float VDOP;						/* GPGSA */
	uint8_t FixMode;				/* GPGSA */
	uint8_t SatelliteIDs[12];		/* GPGSA */
#endif
#ifndef GPS_DISABLE_GPGSV	
	uint8_t SatellitesInView;		/* GPGSV */
#endif
	TM_GPS_Result_t Status;			/* GPS Data status */
} TM_GPS_Data_t;

/**
 * Added in version v1.1
 *
 * GPS Distance struct
 *
 * Parameters:
 * 	- float Latitude1:
 * 		Latitude of starting point
 * 	- float Longitude1:
 * 		Longitude of starting point
 * 	- float Latitude2:
 * 		Latitude of starting point
 * 	- float Longitude2:
 * 		Longitude of starting point
 * 	- float Distance:
 * 		Distance returned in meters
 * 	- float Bearing:
 * 		Bearing returned in degrees
 */
typedef struct {
	float Latitude1;
	float Longitude1;
	float Latitude2;
	float Longitude2;
	float Distance;
	float Bearing;
} TM_GPS_Distance_t;

/* Public */
/**
 * Initialize GPS
 *
 * Parameters:
 * 	- TM_GPS_Data_t* GPS_Data:
 * 		Pointer to TM_GPS_Data_t struct variable
 * 	- uint32_t baudrate:
 * 		Baudrate used for USART
 */
extern void TM_GPS_Init(TM_GPS_Data_t* GPS_Data, uint32_t baudrate);

/**
 * Update GPS data.
 * This function must be called periodically, as fast as possible.
 * It basically checks if data is available on USART and parse it to useful value.
 *
 * Parameters:
 *	- TM_GPS_Data_t* GPS_Data:
 *		Pointer to TM_GPS_Data_t struct variable
 *
 * When you first call this function and there is not available data from GPS, this function will return TM_GPS_Result_FirstTimeWaiting.
 * This will be returning all the time we don't have any useful data.
 * When first time useful data is received from GPS (everything parsed), TM_GPS_Result_NewData will be returned.
 * When we have already new data, next time we call this function, TM_GPS_Result_OldData will be returning until we don't receive new packet of useful data.
 */
extern TM_GPS_Result_t TM_GPS_Update(TM_GPS_Data_t* GPS_Data);

/**
 * Convert speed from knots to desired value
 *
 * Parameters:
 * 	- double SpeedInKnots:
 * 		Speed from GPS, in knots
 * 	- TM_GPS_Speed_t toSpeed:
 * 		Parameter of TM_GPS_Speed_t enumeration to convert data to
 *
 * Returns speed in desired format
 */
extern float TM_GPS_ConvertSpeed(float SpeedInKnots, TM_GPS_Speed_t toSpeed);

/**
 * Converts float number into Integer and Decimal parts.
 *
 * Parameters:
 *  - float num:
 *  	Float number to convert
 *  - TM_GPS_Float_t* Float_Data:
 *  	Pointer to TM_GPS_Float_t struct to store data into
 *  - uint8_t decimals:
 *  	Number of decimals places
 */
extern void TM_GPS_ConvertFloat(float num, TM_GPS_Float_t* Float_Data, uint8_t decimals);

/**
 * Added in version v1.1
 *
 * Calculates distance between 2 coordinates on earth and bearing in relation to the north
 *
 * Distance is returned in meters and bearing in degrees
 *
 * Parameters:
 * 	- TM_GPS_Distance_t* Distance_Data
 * 		Pointer to TM_GPS_Distance_t struct to get data and calculate back
 */
extern void TM_GPS_DistanceBetween(TM_GPS_Distance_t* Distance_Data);

#endif

