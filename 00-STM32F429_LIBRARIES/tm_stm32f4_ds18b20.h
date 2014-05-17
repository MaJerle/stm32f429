/**
 * Library for interfacing DS18B20 temperature sensor from Dallas semiconductors.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_DS18B20_H
#define TM_DS18B20_H 110
/**
 * Library dependencies
 * - tm_stm32f4_onewire.h for OneWire
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "tm_stm32f4_onewire.h"
#include "defines.h"

#define TM_DS18B20_FAMILY_CODE				0x28

#define TM_DS18B20_CMD_CONVERTTEMP			0x44 	//Convert temperature
#define TM_DS18B20_DECIMAL_STEPS_12BIT		0.0625
#define TM_DS18B20_DECIMAL_STEPS_11BIT		0.125
#define TM_DS18B20_DECIMAL_STEPS_10BIT		0.25
#define TM_DS18B20_DECIMAL_STEPS_9BIT		0.5

#define TM_DS18B20_RESOLUTION_R1			6
#define TM_DS18B20_RESOLUTION_R0			5

typedef enum {
	TM_DS18B20_Resolution_9bit,
	TM_DS18B20_Resolution_10bit,
	TM_DS18B20_Resolution_11bit,
	TM_DS18B20_Resolution_12bit
} TM_DS18B20_Resolution_t;

extern uint8_t TM_DS18B20_Start(uint8_t *ROM);
extern uint8_t TM_DS18B20_Read(uint8_t *ROM, float *destination);
extern uint8_t TM_DS18B20_GetResolution(uint8_t *ROM);
extern uint8_t TM_DS18B20_SetResolution(uint8_t *ROM, TM_DS18B20_Resolution_t resolution);

#endif

