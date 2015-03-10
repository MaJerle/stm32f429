/**
 *	Onewire library for STM32F4 devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/library-12-onewire-library-for-stm43f4xx/
 *	@version 	v2.1
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
 * Version 2.1
 *	- March 10, 2015
 *	- Added support for my GPIO library 
 *
 * Version 2.0
 * 	- January 04, 2015
 *	- New OneWire system
 *	- With support for multiple OneWire ports to separate group of devices
 *
 * Version 1.1
 *	- December 06, 2014
 *	- Added 8-bit CRC calculation for 1-Wire devices, algorythm from Dallas
 *
 * As of version 2.0 you can now use more than just one one-wire "port" on STM32F4.
 * This allows you to group devices to separate ports.
 * Because if you have a loot devices on one port, if one device fail, everything is failed.
 * You can prevent this by use more than just one port.
 */
#ifndef TM_ONEWIRE_H
#define TM_ONEWIRE_H 210
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM DELAY
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/* OneWire delay */
#define ONEWIRE_DELAY(x)				Delay(x)

/* Pin settings */
#define ONEWIRE_LOW(structure)			TM_GPIO_SetPinLow((structure)->GPIOx, (structure)->GPIO_Pin)
#define ONEWIRE_HIGH(structure)			TM_GPIO_SetPinHigh((structure)->GPIOx, (structure)->GPIO_Pin)
#define ONEWIRE_INPUT(structure)		TM_GPIO_SetPinAsInput(structure->GPIOx, (structure)->GPIO_Pin)
#define ONEWIRE_OUTPUT(structure)		TM_GPIO_SetPinAsOutput(structure->GPIOx, (structure)->GPIO_Pin)

/* OneWire commands */
#define ONEWIRE_CMD_RSCRATCHPAD			0xBE
#define ONEWIRE_CMD_WSCRATCHPAD			0x4E
#define ONEWIRE_CMD_CPYSCRATCHPAD		0x48
#define ONEWIRE_CMD_RECEEPROM			0xB8
#define ONEWIRE_CMD_RPWRSUPPLY			0xB4
#define ONEWIRE_CMD_SEARCHROM			0xF0
#define ONEWIRE_CMD_READROM				0x33
#define ONEWIRE_CMD_MATCHROM			0x55
#define ONEWIRE_CMD_SKIPROM				0xCC

/**
 * OneWire working struct
 *
 * Parameters:
 *	- GPIO_TypeDef* GPIOx:
 *		GPIOx port to be used for I/O functions
 *	- uint16_t GPIO_Pin:
 *		GPIO Pin to be used for I/O functions
 *	- uint8_t LastDiscrepancy:
 *		Search private
 *	- uint8_t LastFamilyDiscrepancy:
 *		Search private
 *	- uint8_t LastDeviceFlag:
 *		Search private
 *	- uint8_t ROM_NO[8]:
 *		8-bytes address of last search device
 */
typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint8_t LastDiscrepancy;
	uint8_t LastFamilyDiscrepancy;
	uint8_t LastDeviceFlag;
	uint8_t ROM_NO[8];
} TM_OneWire_t;

/**
 * Initialize OneWire bus
 *
 * Parameters:
 * 	- TM_OneWire_t* OneWireStruct:
 *		Pointer to empty working onewire struct
 * 	- GPIO_TypeDef* GPIOx:
 *		Pointer to GPIO used for onewire
 * 	- uint16_t GPIO_Pin:
 *		GPIO Pin on specific GPIOx to be used for onewire
 * 
 * No return
 */
extern void TM_OneWire_Init(TM_OneWire_t* OneWireStruct, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * Reset OneWire bus
 * 
 * Sends reset command for OneWire
 *
 * Parameters: 
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * No return
 */
extern uint8_t TM_OneWire_Reset(TM_OneWire_t* OneWireStruct);

/**
 * Read byte from one wire bus
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * Byte is returned
 */
extern uint8_t TM_OneWire_ReadByte(TM_OneWire_t* OneWireStruct);

/**
 * Write byte to bus
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 * 	- uint8_t byte:
 * 		Byte to be sent
 *
 * No return
 */
extern void TM_OneWire_WriteByte(TM_OneWire_t* OneWireStruct, uint8_t byte);

/**
 * Write single bit to onewire bus
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 * 	- uint8_t bit:
 * 		bit to be sent, 1 or 0
 *
 * No return
 */
extern void TM_OneWire_WriteBit(TM_OneWire_t* OneWireStruct, uint8_t bit);

/**
 * Read single bit from one wire bus
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * Returns 0 if bit is low, or 1 if bit is high
 */
extern uint8_t TM_OneWire_ReadBit(TM_OneWire_t* OneWireStruct);

/**
 * Search OneWire devices.
 * Not meant for public user use
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * Returns 1 if new device is recognized or 0 if not
 */
extern uint8_t TM_OneWire_Search(TM_OneWire_t* OneWireStruct, uint8_t command);

/**
 * Reset search states
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * No return
 */
extern void TM_OneWire_ResetSearch(TM_OneWire_t* OneWireStruct);

/**
 * Start search, reset states
 *
 * Parameters: 
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * Returns 1 if any device on bus
 */
extern uint8_t TM_OneWire_First(TM_OneWire_t* OneWireStruct);

/**
 * Read next device
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 *
 * Returns 1 if more devices are on bus, otherwise 0
 */
extern uint8_t TM_OneWire_Next(TM_OneWire_t* OneWireStruct);

/**
 * Get rom from device from search
 *
 * Parameters
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 * 	- uint8_t index:
 * 		because each device has 8bytes long rom, you have to call this 8 times, to get rom bytes from 0 to 7
 * 
 * Returns ROM byte for index (0 to 7) at current found device
 */
extern uint8_t TM_OneWire_GetROM(TM_OneWire_t* OneWireStruct, uint8_t index);

/**
 * Get all 8 bytes ROM value from device from search
 *
 * Parameters
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 * 	- uint8_t* firstIndex:
 * 		Pointer to first location for first byte, other bytes are automatically incremented
 *
 * No return
 */
extern void TM_OneWire_GetFullROM(TM_OneWire_t* OneWireStruct, uint8_t *firstIndex);

/**
 * Select specific slave on bus
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 * 	- uint8_t* addr:
 * 		Pointer to 8 bytes ROM address
 *
 * No return
 */
extern void TM_OneWire_Select(TM_OneWire_t* OneWireStruct, uint8_t* addr);

/**
 * Select specific slave on bus with pointer address
 *
 * Parameters:
 *	- TM_OneWire_t* OneWireStruct:
 *		Pointer to TM_OneWire_t working struct
 * 	- uint8_t* ROM:
 * 		pointer to first byte of ROM address
 *
 * No return
 */
extern void TM_OneWire_SelectWithPointer(TM_OneWire_t* OneWireStruct, uint8_t* ROM);

/**
 * Calculate 8-bit CRC for 1-wire devices
 *
 * Parameters:
 * 	- uint8_t* addr:
 * 		Pointer to data to calculate CRC
 *	- uint8_t len:
 *		Number of bytes to check
 *
 * Calculated CRC is returned
 */
extern uint8_t TM_OneWire_CRC8(uint8_t* addr, uint8_t len);

#endif

