/**
 * Library for interfacing DS18B20 temperature sensor from Dallas semiconductors.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/13-reading-temperature-with-dallas-ds18b20-on-stm32f429-discovery-board/
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	
 * With this you can read temperature, set and get temperature resolution from 9 to 12 bits
 * and check if device is DS18B20
 * 
 * Pin for STM32F4xx is the same as set in TM ONEWIRE library.
 */
#ifndef TM_DS18B20_H
#define TM_DS18B20_H 110
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

//Every onewire chip has different ROM code, but all the same chips has same family code,
//in case of DS18B20 this is 0x28 and this is first byte of ROM address
#define TM_DS18B20_FAMILY_CODE				0x28
#define TM_DS18B20_CMD_ALARMSEARCH			0xEC

//DS18B20 read temperature command
#define TM_DS18B20_CMD_CONVERTTEMP			0x44 	//Convert temperature
#define TM_DS18B20_DECIMAL_STEPS_12BIT		0.0625
#define TM_DS18B20_DECIMAL_STEPS_11BIT		0.125
#define TM_DS18B20_DECIMAL_STEPS_10BIT		0.25
#define TM_DS18B20_DECIMAL_STEPS_9BIT		0.5

//Bits locations for resolution
#define TM_DS18B20_RESOLUTION_R1			6
#define TM_DS18B20_RESOLUTION_R0			5

/**
 * Resolutions available
 */
typedef enum {
	TM_DS18B20_Resolution_9bits = 9,
	TM_DS18B20_Resolution_10bits = 10,
	TM_DS18B20_Resolution_11bits = 11,
	TM_DS18B20_Resolution_12bits = 12
} TM_DS18B20_Resolution_t;

/**
 * Start temperature conversion 
 * 
 * Parameters: 
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 	
 * Returns 1 if device is DS18B20 or 0 if not
 */
extern uint8_t TM_DS18B20_Start(uint8_t *ROM);

/**
 * Start temperature conversion for all DS18B20 devices
 * 
 * 
 */
extern void TM_DS18B20_StartAll(void);

/**
 * Read temperature from DS18B20
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 	- float *destination: pointer to float variable to store temperature
 * 	
 * Returns 1 if temperature is read or 0 if device is not DS18B20 or conversion is not done yet
 */
extern uint8_t TM_DS18B20_Read(uint8_t *ROM, float *destination);

/**
 * Get resolution for temperature conversion
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 
 * Returns 0 if device is not DS18B20 or bits number (9 - 12)
 */
extern uint8_t TM_DS18B20_GetResolution(uint8_t *ROM);

/**
 * Set resolution for specific DS18B20 device
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 	- TM_DS18B20_Resolution_t resolution: set resolution for DS18B20 device
 * 		- TM_DS18B20_Resolution_9bit: 9bits resolution
 * 		- TM_DS18B20_Resolution_10bit: 10bits resolution
 * 		- TM_DS18B20_Resolution_11bit: 11bits resolution
 * 		- TM_DS18B20_Resolution_12bit: 12bits resolution
 * 		
 * Returns 0 if device is not DS18B20 or 1 if resolution is set
 */
extern uint8_t TM_DS18B20_SetResolution(uint8_t *ROM, TM_DS18B20_Resolution_t resolution);

/**
 * Check if device DS18B20
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 	
 * Returns 1 if device is DS18B20, otherwise 0
 */
extern uint8_t TM_DS18B20_Is(uint8_t *ROM);

/**
 * Set high alarm temperature to specific DS18B20 sensor
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 	- int8_t temp: temperature between -55 and 125°C
 * 
 * Returns 0 if device is not DS18B20, otherwise 1
 */
extern uint8_t TM_DS18B20_SetAlarmHighTemperature(uint8_t *ROM, int8_t temp);

/**
 * Set low alarm temperature to specific DS18B20 sensor
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 	- int8_t temp: temperature between -55 and 125°C
 * 
 * Returns 0 if device is not DS18B20, otherwise 1
 */
extern uint8_t TM_DS18B20_SetAlarmLowTemperature(uint8_t *ROM, int8_t temp);

/**
 * Disable alarm temperature
 * 
 * Parameters:
 * 	- uint8_t *ROM: pointer to first byte of ROM address
 * 
 * Returns 0 if device is not DS18B20, otherwise 1
 */
extern uint8_t TM_DS18B20_DisableAlarmTemperature(uint8_t *ROM);

/**
 * Search for devices with alarm flag set
 * 
 * Returns 1 if any device has flag, otherwise 0
 */
extern uint8_t TM_DS18B20_AlarmSearch(void);

/**
 * Check if all DS18B20 sensors are done with temperature conversion
 * 
 * Return 1 if they are, otherwise 0
 */
extern uint8_t TM_DS18B20_AllDone(void);


#endif

