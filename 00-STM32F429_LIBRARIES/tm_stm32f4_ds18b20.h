/**
 * Library for interfacing DS18B20 temperature sensor from Dallas semiconductors.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/13-reading-temperature-with-dallas-ds18b20-on-stm32f429-discovery-board/
 *	@version 	v2.0
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
 * Version 2.0
 *	- January 04, 2015
 *	- New system, supporting OneWire library 2.0
 *
 * Version 1.1
 *	- December 06, 2014
 *	- Now CRC is calculated and checked if data are valid
 *	- New version of OneWire library is required, download already available on stm32f4-discovery.com
 *
 * With this you can read temperature, set and get temperature resolution from 9 to 12 bits
 * and check if device is DS18B20
 * 
 * Pin for STM32F4xx is the same as set with TM ONEWIRE library.
 */
#ifndef TM_DS18B20_H
#define TM_DS18B20_H 200
/**
 * Library dependencies
 * - STM32F4xx
 * - TM_ONEWIRE
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_onewire.h"
#include "defines.h"

/* OneWire version check */
#if TM_ONEWIRE_H < 200
#error "Please update TM ONEWIRE LIB, minimum required version is 2.0.0"
#endif

/* Every onewire chip has different ROM code, but all the same chips has same family code */
/* in case of DS18B20 this is 0x28 and this is first byte of ROM address */
#define DS18B20_FAMILY_CODE				0x28
#define DS18B20_CMD_ALARMSEARCH			0xEC

/* DS18B20 read temperature command */
#define DS18B20_CMD_CONVERTTEMP			0x44 	/* Convert temperature */
#define DS18B20_DECIMAL_STEPS_12BIT		0.0625
#define DS18B20_DECIMAL_STEPS_11BIT		0.125
#define DS18B20_DECIMAL_STEPS_10BIT		0.25
#define DS18B20_DECIMAL_STEPS_9BIT		0.5

/* Bits locations for resolution */
#define DS18B20_RESOLUTION_R1			6
#define DS18B20_RESOLUTION_R0			5

#ifdef DS18B20_USE_CRC	
#define DS18B20_DATA_LEN				9
#else
#define DS18B20_DATA_LEN				2
#endif

/**
 * Resolutions available
 *
 * Parameters:
 *	- TM_DS18B20_Resolution_9bits:
 *		DS18B20 9 bits
 *	- TM_DS18B20_Resolution_10bits:
 *		DS18B20 10 bits
 *	- TM_DS18B20_Resolution_11bits:
 *		DS18B20 11 bits
 *	- TM_DS18B20_Resolution_12bits:
 *		DS18B20 12 bits
 */
typedef enum {
	TM_DS18B20_Resolution_9bits = 9,
	TM_DS18B20_Resolution_10bits = 10,
	TM_DS18B20_Resolution_11bits = 11,
	TM_DS18B20_Resolution_12bits = 12
} TM_DS18B20_Resolution_t;

/**
 * Start temperature conversion for specific DS18B20 on specific onewire channel
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t *ROM:
 *		Pointer to first byte of ROM address
 * 	
 * Returns 1 if device is DS18B20 or 0 if not
 */
extern uint8_t TM_DS18B20_Start(TM_OneWire_t* OneWireStruct, uint8_t* ROM);

/**
 * Start temperature conversion for all DS18B20 devices on specific onewire channel
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 *	
 * No return
 */
extern void TM_DS18B20_StartAll(TM_OneWire_t* OneWireStruct);

/**
 * Read temperature from DS18B20
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t* ROM:
 *		Pointer to first byte of ROM address
 * 	- float* destination:
 *		Pointer to float variable to store temperature
 * 	
 * Returns 1 if temperature is read OK or 0 if device is not DS18B20 or conversion is not done yet or CRC failed
 */
extern uint8_t TM_DS18B20_Read(TM_OneWire_t* OneWireStruct, uint8_t* ROM, float* destination);

/**
 * Get resolution for temperature conversion
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t* ROM: 
 *		Pointer to first byte of ROM address
 * 
 * Returns 0 if device is not DS18B20 or bits number (9 - 12)
 */
extern uint8_t TM_DS18B20_GetResolution(TM_OneWire_t* OneWireStruct, uint8_t* ROM);

/**
 * Set resolution for specific DS18B20 device
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t* ROM:
 *		Pointer to first byte of ROM address
 * 	- TM_DS18B20_Resolution_t resolution:
 *		Set resolution for DS18B20 device:
 * 			- TM_DS18B20_Resolution_9bit: 9bits resolution
 * 			- TM_DS18B20_Resolution_10bit: 10bits resolution
 * 			- TM_DS18B20_Resolution_11bit: 11bits resolution
 * 			- TM_DS18B20_Resolution_12bit: 12bits resolution
 * 		
 * Returns 0 if device is not DS18B20 or 1 if resolution is set
 */
extern uint8_t TM_DS18B20_SetResolution(TM_OneWire_t* OneWireStruct, uint8_t* ROM, TM_DS18B20_Resolution_t resolution);

/**
 * Check if device is DS18B20 with specific ROM number
 * 
 * Parameters:
 * 	- uint8_t* ROM:
 *		Pointer to first byte of ROM address
 * 	
 * Returns 1 if device is DS18B20, otherwise 0
 */
extern uint8_t TM_DS18B20_Is(uint8_t* ROM);

/**
 * Set high alarm temperature to specific DS18B20 sensor
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t* ROM:
 *		Pointer to first byte of ROM address
 * 	- int8_t temp:
 *		Temperature between -55 and 125°C
 * 
 * Returns 0 if device is not DS18B20, otherwise 1
 */
extern uint8_t TM_DS18B20_SetAlarmHighTemperature(TM_OneWire_t* OneWireStruct, uint8_t* ROM, int8_t temp);

/**
 * Set low alarm temperature to specific DS18B20 sensor
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t* ROM:
 *		Pointer to first byte of ROM address
 * 	- int8_t temp:
 *		Temperature between -55 and 125°C
 * 
 * Returns 0 if device is not DS18B20, otherwise 1
 */
extern uint8_t TM_DS18B20_SetAlarmLowTemperature(TM_OneWire_t* OneWireStruct, uint8_t* ROM, int8_t temp);

/**
 * Disable alarm temperature on specific DS18B20 sensor
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 * 	- uint8_t* ROM:
 *		Pointer to first byte of ROM address
 * 
 * Returns 0 if device is not DS18B20, otherwise 1
 */
extern uint8_t TM_DS18B20_DisableAlarmTemperature(TM_OneWire_t* OneWireStruct, uint8_t* ROM);

/**
 * Search for devices with alarm flag set
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 *
 * Returns 1 if any device has flag, otherwise 0
 */
extern uint8_t TM_DS18B20_AlarmSearch(TM_OneWire_t* OneWireStruct);

/**
 * Check if all DS18B20 sensors are done with temperature conversion
 * 
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to OneWire working struct (channel)
 *
 * Return 1 if they are, otherwise 0
 */
extern uint8_t TM_DS18B20_AllDone(TM_OneWire_t* OneWireStruct);


#endif

