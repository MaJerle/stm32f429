/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
 */
#include "tm_stm32f4_flashloader.h"

/* All FLASH flags */
#define FLAG_ALL (FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |  \
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR)

/* Private typedef */
typedef struct {
	uint32_t ApplicationAddress;     /*!< Application start address */
	uint16_t ApplicationStartSector; /*!< Application start sector */
	uint32_t ApplicationProgramSize; /*!< Application program size */
	uint16_t ApplicationEndSector;   /*!< Application end sector */
	uint32_t FlashEndAddress;        /*!< Flash end address in device */
	uint16_t FlashEndSector;         /*!< Flash end sector in device */
	uint32_t WritePointer;           /*!< Current write pointer for next data packet save */
	uint8_t SectorsErased;           /*!< Set to 1 when sectors for application are already erased */
	uint8_t State;                   /*!< Current bootloader programming state */
	uint8_t AffectedSectorsCount;    /*!<Number of affected sectors used for program/settings */
} FLASHLOADER_t;

/* Private variable */
FLASHLOADER_t FLASHLOADER;

uint8_t TM_FLASHLOADER_Begin(uint32_t ApplicationAddress, uint32_t ApplicationSize) {
	uint16_t sector;
	
	/* Save application address location */
	FLASHLOADER.ApplicationAddress = FLASHLOADER_ADDRESS(ApplicationAddress);
	
	/* Get flash last location in device */
	FLASHLOADER.FlashEndAddress = 0x08000000 + (uint32_t) (*(__IO uint16_t *) (0x1FFF7A22)) * 1024 - 1;
	
	/* Store application program size */
	if (ApplicationSize > 0) {
		FLASHLOADER.ApplicationProgramSize = ApplicationSize;
	} else {
		/* Set application program size */
		FLASHLOADER.ApplicationProgramSize = FLASHLOADER.FlashEndAddress - FLASHLOADER.ApplicationAddress;
	}
	
	/* Check application address */
	if (FLASHLOADER.FlashEndAddress <= FLASHLOADER.ApplicationAddress) {
		/* No go, not in side flash area */
		return 1;
	}
	
	/* Check if available memory in flash */
	if ((FLASHLOADER.FlashEndAddress - FLASHLOADER.ApplicationAddress) < FLASHLOADER.ApplicationProgramSize) {
		/* No go, no memory available for whole data */
		return 1;
	}
	
	/* Get application sectors */
	FLASHLOADER.ApplicationStartSector = TM_FLASHLOADER_GetSector(FLASHLOADER.ApplicationAddress);
	FLASHLOADER.FlashEndSector = TM_FLASHLOADER_GetSector(FLASHLOADER.FlashEndAddress - 1);
	
	/* Calculate application end sector */
	FLASHLOADER.ApplicationEndSector = TM_FLASHLOADER_GetSector(FLASHLOADER.ApplicationAddress + FLASHLOADER.ApplicationProgramSize - 1);
	
	/* Ready */
	FLASHLOADER.State = 1;
	
	/* Reset everything */
	FLASHLOADER.WritePointer = FLASHLOADER.ApplicationAddress;
	FLASHLOADER.SectorsErased = 0;
	FLASHLOADER.AffectedSectorsCount = 0;
	
	/* Get all sectors */
	for (sector = FLASHLOADER.ApplicationStartSector; sector < FLASHLOADER.ApplicationEndSector + 1; sector += 8) {
		/* Increase number of sectors */
		FLASHLOADER.AffectedSectorsCount++;
	}
	
	/* Clear flash flags */
	FLASH_ClearFlag(FLAG_ALL);
	
	/* Unlock flash */
	FLASH_Unlock();
	
	/* Enable access to read/write flash */
	FLASH_OB_Unlock();	
	
	/* Clear flash flags */
	FLASH_ClearFlag(FLAG_ALL);
	
	/* Disable write protection for sectors where application program will be stored */
	TM_FLASHLOADER_DisableWP();
	
	/* Everything OK */
	return 0;
}

uint8_t TM_FLASHLOADER_End(void) {
	/* Write protect sectors */
	//TM_FLASHLOADER_EnableWP();
	
	/* Wait flash for last operation */
	FLASH_WaitForLastOperation();
	
	/* Lock flash */
	FLASH_Lock();
	
	/* Everything OK */
	return 0;
}

uint32_t TM_FLASHLOADER_ProcessPacket(uint8_t* data, uint32_t count) {
	uint32_t status;
	uint32_t blocks;
	
	/* Check if sectors erased */
	if (TM_FLASHLOADER_EraseSectors()) {
		/* Return error */
		return 1;
	}
	
	/* Clear all flash flags */
	FLASH_ClearFlag(FLAG_ALL);
	
	/* Status flag */
	status = 0;
	
	/* Get number of blocks in buffer */
	blocks = count >> 2;
	
	/* Write data to flash memory */
	while (blocks--) {
		/* Check if application end address has reached */
		if (FLASHLOADER.WritePointer >= FLASHLOADER.ApplicationAddress + FLASHLOADER.ApplicationProgramSize) {
			/* No memory available for program anymore */
			return 2;
		}
		
		/* Program flash as words */
		status += FLASH_ProgramWord(FLASHLOADER.WritePointer, *(uint32_t *)data) != FLASH_COMPLETE;
		
		/* Increase pointer */
		FLASHLOADER.WritePointer += 4;
		data += 4;
	}
	
	/* Write remaining data */
	count = count % 4;
	
	/* Write to flash */
	while (count--) {
		/* Check if application end address has reached */
		if (FLASHLOADER.WritePointer >= FLASHLOADER.ApplicationAddress + FLASHLOADER.ApplicationProgramSize) {
			/* No memory available for program anymore */
			return 2;
		}
		
		/* Program flash as bytes */
		status += FLASH_ProgramByte(FLASHLOADER.WritePointer++, *(__IO uint8_t *)data++) != FLASH_COMPLETE;
	}
	
	/* Everything OK = status = 0 */
	return status;
}

uint8_t TM_FLASHLOADER_EraseSectors(void) {
	uint16_t sector;
	
	/* Erase sectors if the are not already */
	if (FLASHLOADER.SectorsErased) {
		/* Erase all affected sectors */
		return 0;
	}
	
	/* Disable all interrupts */
	__disable_irq();
	
	/* Do a sectors reset */
	for (sector = FLASHLOADER.ApplicationStartSector; sector < FLASHLOADER.ApplicationEndSector + 1; sector += 8) {
		/* Clear flash flags */
		FLASH_ClearFlag(FLAG_ALL);
		
		/* Do a sector erase */
		if (FLASH_EraseSector((uint32_t)sector, FLASHLOADER_VOLTAGE_RANGE) != FLASH_COMPLETE) {
			/* Return error */
			return 1;
		}
	}
	
	/* Set flag, sectors erased */
	FLASHLOADER.SectorsErased = 1;
	
	/* Enable all interrupts */
	__enable_irq();
	
	/* Return OK */
	return 0;
}

uint8_t TM_FLASHLOADER_GetAffectedSectorsCount(void) {
	/* Return affected sectors count */
	return FLASHLOADER.AffectedSectorsCount;
}

uint8_t TM_FLASHLOADER_JumpToProgram(uint32_t FlashAddress) {
	/* New type for function call */
	typedef void (*Bootloader_Function)(void);
	Bootloader_Function Jump_To_Application;
	
	/* Check flash address */
	if (FlashAddress == 0) {
		FlashAddress = FLASHLOADER.ApplicationAddress;
	}
	
	/* Format address */
	FlashAddress = FLASHLOADER_ADDRESS(FlashAddress);
	
	/* Test if user code is programmed starting from address "program_address" */
    if (
		((*(__IO uint32_t *) FlashAddress) & 0x2FFE0000) == 0x20000000 && /*!< Program is loaded */
		TM_FLASHLOADER_BeforeJump(FlashAddress)                           /*<! Jump is allowed to location */
	) { 
		/* Disable interrupts */
		__disable_irq();
		
		/* Set FLASH vector table */
		SCB->VTOR = 0x08000000 | (FlashAddress & (uint32_t)0x1FFFFF80);
		
		/* Jump to user application, set jump address */
		Jump_To_Application = (Bootloader_Function) *(__IO uint32_t *) (FlashAddress + 4);
		
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t *) FlashAddress);
		
		/* Jump to application */
		Jump_To_Application();
		
		/* Return 1 = This should never happen */
		return 1;
    }
	
	/* Return error in case we didn't jump */
	return 0;
}

TM_FLASHLOADER_Sector_t TM_FLASHLOADER_GetSector(uint32_t Address) {
	TM_FLASHLOADER_Sector_t sector = TM_FLASHLOADER_Sector_0;
	
	/* Format address */
	Address = FLASHLOADER_ADDRESS(Address);

	/* Get proper sector */
	if ((Address < ADDR_FLASH_SECTOR_1)) {
		sector = TM_FLASHLOADER_Sector_0;
	} else if ((Address < ADDR_FLASH_SECTOR_2)) {
		sector = TM_FLASHLOADER_Sector_1;  
	} else if ((Address < ADDR_FLASH_SECTOR_3)) {
		sector = TM_FLASHLOADER_Sector_2;  
	} else if ((Address < ADDR_FLASH_SECTOR_4)) {
		sector = TM_FLASHLOADER_Sector_3;  
	} else if ((Address < ADDR_FLASH_SECTOR_5)) {
		sector = TM_FLASHLOADER_Sector_4;  
	} else if ((Address < ADDR_FLASH_SECTOR_6)) {
		sector = TM_FLASHLOADER_Sector_5;  
	} else if ((Address < ADDR_FLASH_SECTOR_7)) {
		sector = TM_FLASHLOADER_Sector_6;  
	} else if ((Address < ADDR_FLASH_SECTOR_8)) {
		sector = TM_FLASHLOADER_Sector_7;  
	} else if ((Address < ADDR_FLASH_SECTOR_9)) {
		sector = TM_FLASHLOADER_Sector_8;  
	} else if ((Address < ADDR_FLASH_SECTOR_10)) {
		sector = TM_FLASHLOADER_Sector_9;  
	} else if ((Address < ADDR_FLASH_SECTOR_11)) {
		sector = TM_FLASHLOADER_Sector_10;  
	} else if ((Address < ADDR_FLASH_SECTOR_12)) {
		sector = TM_FLASHLOADER_Sector_11;
	} else if ((Address < ADDR_FLASH_SECTOR_13)) {
		sector = TM_FLASHLOADER_Sector_12;
	} else if ((Address < ADDR_FLASH_SECTOR_14)) {
		sector = TM_FLASHLOADER_Sector_13;
	} else if ((Address < ADDR_FLASH_SECTOR_15)) {
		sector = TM_FLASHLOADER_Sector_14;
	} else if ((Address < ADDR_FLASH_SECTOR_16)) {
		sector = TM_FLASHLOADER_Sector_15;
	} else if ((Address < ADDR_FLASH_SECTOR_17)) {
		sector = TM_FLASHLOADER_Sector_16;
	} else if ((Address < ADDR_FLASH_SECTOR_18)) {
		sector = TM_FLASHLOADER_Sector_17;
	} else if ((Address < ADDR_FLASH_SECTOR_19)) {
		sector = TM_FLASHLOADER_Sector_18;
	} else if ((Address < ADDR_FLASH_SECTOR_20)) {
		sector = TM_FLASHLOADER_Sector_19;
	} else if ((Address < ADDR_FLASH_SECTOR_21)) {
		sector = TM_FLASHLOADER_Sector_20;
	} else if ((Address < ADDR_FLASH_SECTOR_22)) {
		sector = TM_FLASHLOADER_Sector_21;
	} else if ((Address < ADDR_FLASH_SECTOR_23)) {
		sector = TM_FLASHLOADER_Sector_22;
	} else if ((Address < USER_FLASH_END_ADDRESS)) {
		sector = TM_FLASHLOADER_Sector_23;
	}
	
	return sector;
}

uint32_t TM_FLASHLOADER_GetSectorSize(TM_FLASHLOADER_Sector_t sector) {
	uint32_t size;
	
	/* Check sectors */
	switch (sector) {
		case TM_FLASHLOADER_Sector_0:
		case TM_FLASHLOADER_Sector_1:
		case TM_FLASHLOADER_Sector_2:
		case TM_FLASHLOADER_Sector_3:
		case TM_FLASHLOADER_Sector_12:
		case TM_FLASHLOADER_Sector_13:
		case TM_FLASHLOADER_Sector_14:
		case TM_FLASHLOADER_Sector_15:
			/* 16-kB sector size */
			size = 0x4000;
			break;
		case TM_FLASHLOADER_Sector_4:
		case TM_FLASHLOADER_Sector_16:
			/* 64-kB sector size */
			size = 0x10000;
			break;
		case TM_FLASHLOADER_Sector_5:
		case TM_FLASHLOADER_Sector_6:
		case TM_FLASHLOADER_Sector_7:
		case TM_FLASHLOADER_Sector_8:
		case TM_FLASHLOADER_Sector_9:
		case TM_FLASHLOADER_Sector_10:
		case TM_FLASHLOADER_Sector_11:
		case TM_FLASHLOADER_Sector_17:
		case TM_FLASHLOADER_Sector_18:
		case TM_FLASHLOADER_Sector_19:
		case TM_FLASHLOADER_Sector_20:
		case TM_FLASHLOADER_Sector_21:
		case TM_FLASHLOADER_Sector_22:
		case TM_FLASHLOADER_Sector_23:
			/* 128-kB sector size */
			size = 0x20000;
			break;
		default:
			size = 0;
	}
	
	/* Return size value */
	return size;
}

uint8_t TM_FLASHLOADER_DisableWP(void) {
	uint8_t UserWrpSectors;

	/* Mark all sectors inside the user flash area as non protected */  
	UserWrpSectors = 0xFFF - ((1 << (FLASHLOADER.ApplicationStartSector / 8)) - 1);

	/* Unlock the Option Bytes */
	FLASH_OB_Unlock();

	/* Disable the write protection for all sectors inside the user flash area */
	FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);

	/* Returns status, 0 = OK, 1 = Error */  
	if (FLASH_OB_Launch() != FLASH_COMPLETE) {
		/* return error */
		return 1;
	}
	
	/* Lock option bytes */
	FLASH_OB_Lock();
	
	/* Return OK */
	return 0;
}

uint8_t TM_FLASHLOADER_EnableWP(void) {
	uint8_t UserWrpSectors;

	/* Mark all sectors inside the user flash area as non protected */  
	UserWrpSectors = 0xFFF - ((1 << (FLASHLOADER.ApplicationStartSector / 8)) - 1);

	/* Unlock the Option Bytes */
	FLASH_OB_Unlock();

	/* Disable the write protection for all sectors inside the user flash area */
	FLASH_OB_WRPConfig(UserWrpSectors, ENABLE);

	/* Returns status, 0 = OK, 1 = Error */  
	if (FLASH_OB_Launch() != FLASH_COMPLETE) {
		/* Return error */
		return 1;
	}
	
	/* Lock option bytes */
	FLASH_OB_Lock();
	
	/* Return OK */
	return 0;
}

uint8_t TM_FLASHLOADER_Read(uint32_t FlashAddress, uint8_t* buff, uint32_t count) {
	/* Format address */
	FlashAddress = FLASHLOADER_ADDRESS(FlashAddress);
	
	/* Do a memory copy */
	memcpy(buff, (uint8_t *)FlashAddress, count);
	
	/* Everything OK */
	return 0;
}

__weak uint8_t TM_FLASHLOADER_BeforeJump(uint32_t FlashAddress) {
	/* Allow jump, return 1 */
	return 1;
}
