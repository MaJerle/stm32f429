/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library template 
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_FLASHLOADER_H
#define TM_FLASHLOADER_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_FLASHLOADER
 * @brief    Flash loader (program loader, settings loader) for STM32F4xx devices
 * @{
 *
 * @par Features
 *
 * Flash loader library allows you variuos things:
 *
@verbatim
- Program STM32F4xx device while running
  - You need bootloader program for that operation which will read program data from external source and save it to flash memory
- Write configuration data to one (or more) of your sectors in flash memory
- Allows you to protect your flash sectors for write operation from external debugger (MAY LOCK STM FOREVER!)
- Allows you to protect your flash sectors for read operation from external debugger
@endverbatim
 * 
 * Library includes functions to erase sectors, write to sectors, lock flash for read/write operations
 * jump between main application and user defined flashloader application and more.
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx FLASH
 - defines.h
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"
#include "defines.h"
#include "attributes.h"
#include "string.h"

/**
 * @defgroup TM_FLASHLOADER_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Voltage range value
 */
#ifndef FLASHLOADER_VOLTAGE_RANGE
#define FLASHLOADER_VOLTAGE_RANGE   VoltageRange_3
#endif

/**
 * @brief  Locations for each sector in flash memory 
 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /*!< Base @ of Sector 00, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /*!< Base @ of Sector 01, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /*!< Base @ of Sector 02, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /*!< Base @ of Sector 03, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /*!< Base @ of Sector 04, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /*!< Base @ of Sector 05, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /*!< Base @ of Sector 06, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /*!< Base @ of Sector 07, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /*!< Base @ of Sector 08, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /*!< Base @ of Sector 09, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /*!< Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /*!< Base @ of Sector 11, 128 Kbytes */
#define ADDR_FLASH_SECTOR_12    ((uint32_t)0x08100000) /*!< Base @ of Sector 12, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13    ((uint32_t)0x08104000) /*!< Base @ of Sector 13, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14    ((uint32_t)0x08108000) /*!< Base @ of Sector 14, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15    ((uint32_t)0x0810C000) /*!< Base @ of Sector 15, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16    ((uint32_t)0x08110000) /*!< Base @ of Sector 16, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17    ((uint32_t)0x08120000) /*!< Base @ of Sector 17, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18    ((uint32_t)0x08140000) /*!< Base @ of Sector 18, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19    ((uint32_t)0x08160000) /*!< Base @ of Sector 19, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20    ((uint32_t)0x08180000) /*!< Base @ of Sector 20, 128 Kbytes */
#define ADDR_FLASH_SECTOR_21    ((uint32_t)0x081A0000) /*!< Base @ of Sector 21, 128 Kbytes */
#define ADDR_FLASH_SECTOR_22    ((uint32_t)0x081C0000) /*!< Base @ of Sector 22, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23    ((uint32_t)0x081E0000) /*!< Base @ of Sector 23, 128 Kbytes */
#define USER_FLASH_END_ADDRESS  ((uint32_t)0x081FFFFF) /*!< End of FLASH memory */	

/**
 * @brief  Flash address filter 
 */
#define FLASHLOADER_ADDRESS(x)  (ADDR_FLASH_SECTOR_0 | ((x) & USER_FLASH_END_ADDRESS)) 

/**
 * @}
 */

/**
 * @defgroup TM_FLASHLOADER_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Flash sector definitions
 */
typedef enum {
	TM_FLASHLOADER_Sector_0 = FLASH_Sector_0,
	TM_FLASHLOADER_Sector_1 = FLASH_Sector_1,
	TM_FLASHLOADER_Sector_2 = FLASH_Sector_2,
	TM_FLASHLOADER_Sector_3 = FLASH_Sector_3,
	TM_FLASHLOADER_Sector_4 = FLASH_Sector_4,
	TM_FLASHLOADER_Sector_5 = FLASH_Sector_5,
	TM_FLASHLOADER_Sector_6 = FLASH_Sector_6,
	TM_FLASHLOADER_Sector_7 = FLASH_Sector_7,
	TM_FLASHLOADER_Sector_8 = FLASH_Sector_8,
	TM_FLASHLOADER_Sector_9 = FLASH_Sector_9,
	TM_FLASHLOADER_Sector_10 = FLASH_Sector_10,
	TM_FLASHLOADER_Sector_11 = FLASH_Sector_11,
	TM_FLASHLOADER_Sector_12 = FLASH_Sector_12,
	TM_FLASHLOADER_Sector_13 = FLASH_Sector_13,
	TM_FLASHLOADER_Sector_14 = FLASH_Sector_14,
	TM_FLASHLOADER_Sector_15 = FLASH_Sector_15,
	TM_FLASHLOADER_Sector_16 = FLASH_Sector_16,
	TM_FLASHLOADER_Sector_17 = FLASH_Sector_17,
	TM_FLASHLOADER_Sector_18 = FLASH_Sector_18,
	TM_FLASHLOADER_Sector_19 = FLASH_Sector_19,
	TM_FLASHLOADER_Sector_20 = FLASH_Sector_20,
	TM_FLASHLOADER_Sector_21 = FLASH_Sector_21,
	TM_FLASHLOADER_Sector_22 = FLASH_Sector_22,
	TM_FLASHLOADER_Sector_23 = FLASH_Sector_23,
} TM_FLASHLOADER_Sector_t;

/**
 * @}
 */

/**
 * @defgroup TM_FLASHLOADER_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Begins and prepares flash memory for future programming
 * @note   This function has to be called first before you can send data to program flash memory
 * @param  ApplicationAddress: Address in flash where application will start
 * @param  ApplicationSize: Application size in bytes. If you don't know your size, set this parameter to zero (0)
 * @retval Begin status:
 *            - 0: Begin was successful, @ref TM_FLASHLOADER_ProcessPacket is ready to accept data packet to save to flash
 *            - > 0: Begin was not successful
 */
uint8_t TM_FLASHLOADER_Begin(uint32_t ApplicationAddress, uint32_t ApplicationSize);

/**
 * @brief  Ends programming, locks flash register access for security purpose
 * @param  None
 * @retval End status:
 *            - 0: End was successful
 *            - > 0: End was not successful
 */
uint8_t TM_FLASHLOADER_End(void);

/**
 * @brief  Jumps to application program after successfully programmed flash memory
 * @note   This function can be used after you begin flashloader with @ref TM_FLASHLOADER_Begin() function
 * @param  FlashAddress: Flash address where program will jump. Set parameter to NULL or zero (0) if you want to jump
 *            to location set with @ref TM_FLASHLOADER_Begin function
 * @retval Jump status:
 *            - 0: Jump was not successful because program is not properly loaded on application address 
 *            - > 0: Jump was successful. This should never happen because program will go to application address
 */
uint8_t TM_FLASHLOADER_JumpToProgram(uint32_t FlashAddress);

/**
 * @brief  Process input packet with data to be stored into flash memory
 * @note   Function will automatically erase all sectors which are needed for program to load
 * @note   Function will also disable all sectors from write protection if needed
 *
 * @note   This function can be used after you begin flashloader with @ref TM_FLASHLOADER_Begin() function
 * @param  *data: Pointer to data array to be appended in flash
 * @param  count: Number of data bytes in array. This value has to be multiple of four (4)
 *           as data are stored in flash as words (4 bytes) at a time
 * @retval Process status:
 *            - 0: Data stored on flash OK
 *            - > 0: Data not stored properly
 */
uint32_t TM_FLASHLOADER_ProcessPacket(uint8_t* data, uint32_t count);

/**
 * @brief  Erases all affected sectors for program memory or any other flash operation
 * @note   This function can be used after you begin flashloader with @ref TM_FLASHLOADER_Begin() function
 * @note   This function will be called from @ref TM_FLASHLOADER_ProcessPacket() if you don't use it, to erase sectors
 * @note   Sectors that will be erased depends on input parameters when using @ref TM_FLASHLOADER_Begin() function
 * @note   Erase operation will be done only first time, when this function is called after @ref TM_FLASHLOADER_Begin() function call.
 *         If you will try to call this more times, it will have no effect
 * @param  None
 * @retval Erasing status:
 *            - 0: Erased OK
 *            - > 0: Erase process was not successful
 */
uint8_t TM_FLASHLOADER_EraseSectors(void);

/** 
 * @brief  Returns number of affected sectors for erasing and for available data memory
 * @note   This function can be used after you begin flashloader with @ref TM_FLASHLOADER_Begin() function
 * @param  None
 * @retval Number of affected FLASH sectors 
 */
uint8_t TM_FLASHLOADER_GetAffectedSectorsCount(void);

/**
 * @brief  Disables write protection for ALL sectors in flash!
 * @note   This function can be used after you begin flashloader with @ref TM_FLASHLOADER_Begin() function
 * @param  None
 * @retval Disabled write protection status:
 *            - 0: Write protection disabled
 *            - > 0: Write protection was not disabled
 */ 
uint8_t TM_FLASHLOADER_DisableWP(void);

/**
 * @brief  Enables write protection for sectors in flash
 * @note   This function can be used after you begin flashloader with @ref TM_FLASHLOADER_Begin() function
 * @note   Be careful when using this function. It may block your flash forever and disables you future programming!
 * @param  None
 * @retval Disabled write protection status:
 *            - 0: Write protection enabled
 *            - > 0: Write protection was not enabled
 */
uint8_t TM_FLASHLOADER_EnableWP(void);

/**
 * @brief  Returns sector number value for specific address in flash memory
 * @param  Address: Address in flash memory to get sector number back
 * @retval Sector number
 * @note   Sector number is not in format:
 *            0x00 = Sector 0,
 *            0x01 = Sector 1,
 *            0x02 = ...
 *         But in format:
 *            0x00 = Sector 0
 *            0x08 = Sector 1
 *            0x10 = Sector 2
 *            0x18 = ...
 */
TM_FLASHLOADER_Sector_t TM_FLASHLOADER_GetSector(uint32_t Address);

/**
 * @brief  Gets sector size in bytes
 * @param  Sector: Sector number to be checked. This parameter can be a value of @ref TM_FLASHLOADER_Sector_t enumeration
 * @retval Sector size in units of bytes
 */
uint32_t TM_FLASHLOADER_GetSectorSize(TM_FLASHLOADER_Sector_t Sector);

/**
 * @brief  Reads data from flash memory
 * @note   For this operation, you don't need to call @ref TM_FLASHLOADER_Begin() function before
 * @param  FlashAddress: Starting address in flash where read operation will start
 * @param  *buff: Pointer to buffer where data will be stored from flash
 * @param  count: Number of bytes to read, starting from @arg FlashAddress location
 * @retval Read status:
 *            - 0: Read was successful
 *            - > 0: Read was not successful
 */
uint8_t TM_FLASHLOADER_Read(uint32_t FlashAddress, uint8_t* buff, uint32_t count);

/**
 * @brief  Before jump callback
 * @note   This function is called from @ref TM_FLASHLOADER_JumpToProgram function.
 * @note   It must return > 0 to allow jump to address, or 0 to deny
 * @param  uint32_t FlashAddress
 * @retval Status for allow/deny jump to location
 *            - 0: Jump is denied by user
 *            - > 0: Jump is allowed by user
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint8_t TM_FLASHLOADER_BeforeJump(uint32_t FlashAddress);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
