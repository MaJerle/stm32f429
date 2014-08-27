/**	
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
 */
#include "tm_stm32f4_sdram.h"

uint8_t TM_SDRAM_Init(void) {
	FMC_SDRAMInitTypeDef FMC_SDRAMInitDef;
	FMC_SDRAMTimingInitTypeDef FMC_SDRAMTimingInitDef;
	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
	uint32_t timeout = SDRAM_TIMEOUT;
	
	/* Initialize FMC pins */
	TM_SDRAM_InitPins();
	
	/* Enable FMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
	
	/* FMC SDRAM device initialization sequence --------------------------------*/ 
	/* Step 1 ----------------------------------------------------*/ 
	/* Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2) */
	/* TMRD: 2 Clock cycles */
	/* 1 clock cycle = 1 / 90MHz = 11.1ns */
	FMC_SDRAMTimingInitDef.FMC_LoadToActiveDelay    = 2;      
	/* TXSR: min=70ns (7x11.10ns) */
	FMC_SDRAMTimingInitDef.FMC_ExitSelfRefreshDelay = 7;
	/* TRAS: min=42ns (4x11.10ns) max=120k (ns) */
	FMC_SDRAMTimingInitDef.FMC_SelfRefreshTime      = 4;
	/* TRC:  min=70 (7x11.10ns) */        
	FMC_SDRAMTimingInitDef.FMC_RowCycleDelay        = 7;         
	/* TWR:  min=1+ 7ns (1+1x11.10ns) */
	FMC_SDRAMTimingInitDef.FMC_WriteRecoveryTime    = 2;      
	/* TRP:  20ns => 2x11.10ns */
	FMC_SDRAMTimingInitDef.FMC_RPDelay              = 2;                
	/* TRCD: 20ns => 2x11.10ns */
	FMC_SDRAMTimingInitDef.FMC_RCDDelay             = 2;
	
	
	/* FMC SDRAM control configuration */
	FMC_SDRAMInitDef.FMC_Bank 						= FMC_Bank2_SDRAM;
	/* Row addressing: [7:0] */
	FMC_SDRAMInitDef.FMC_ColumnBitsNumber 			= FMC_ColumnBits_Number_8b;
	/* Column addressing: [11:0] */
	FMC_SDRAMInitDef.FMC_RowBitsNumber 				= FMC_RowBits_Number_12b;
	FMC_SDRAMInitDef.FMC_SDMemoryDataWidth 			= FMC_SDMemory_Width_16b;
	FMC_SDRAMInitDef.FMC_InternalBankNumber 		= FMC_InternalBank_Number_4;
	/* CL: Cas Latency = 3 clock cycles */
	FMC_SDRAMInitDef.FMC_CASLatency 				= FMC_CAS_Latency_3;
	FMC_SDRAMInitDef.FMC_WriteProtection 			= FMC_Write_Protection_Disable;
	FMC_SDRAMInitDef.FMC_SDClockPeriod 				= FMC_SDClock_Period_2;
	FMC_SDRAMInitDef.FMC_ReadBurst 					= FMC_Read_Burst_Disable;
	FMC_SDRAMInitDef.FMC_ReadPipeDelay 				= FMC_ReadPipe_Delay_1;
	FMC_SDRAMInitDef.FMC_SDRAMTimingStruct 			= &FMC_SDRAMTimingInitDef;
	/* FMC SDRAM bank initialization */
	FMC_SDRAMInit(&FMC_SDRAMInitDef);
	
	/* Configure a clock configuration enable command */
	FMC_SDRAMCommandStructure.FMC_CommandMode				= FMC_Command_Mode_CLK_Enabled;
	FMC_SDRAMCommandStructure.FMC_CommandTarget 			= FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber 		= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */ 
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout--);
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Configure a PALL (precharge all) command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
	
	/* Wait until the SDRAM controller is ready */  
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) && timeout--);
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Configure a Auto-Refresh command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 8;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) && timeout--);
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Configure a load Mode register command */
	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0x0231;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) && timeout--);
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Set the refresh rate counter */
	/* (7.81 us x Freq) - 20 = (7.81 * 90MHz) - 20 = 683 */
	/* Set the device refresh counter */
	FMC_SetRefreshCount(680);
	
	/* Wait until the SDRAM controller is ready */ 
	timeout = SDRAM_TIMEOUT;
	while(FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout--);
	
	/* Check if everything goes right */
	/* Write 0x10 at location 0x50 and check if result is the same on read operation */
	TM_SDRAM_Write8(0x50, 0x10);
	if (TM_SDRAM_Read8(0x50) == 0x10) {
		//Initialized OK
		return 1;
	}
	//Not OK
	return 0;
}

void TM_SDRAM_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitDef;
	
	//Common settings
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	
	
	//GPIOB pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_FMC);
	//                      SDCKE1       SDNE1
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitDef);
	
	
	//GPIOC pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_FMC);
	//                      SDNWE
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitDef);
	
	
	//GPIOD pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);	
	//                      D2           D3           D13          D14          D15           D0            D13  
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitDef);
	
	
	//GPIOD pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);					
	//                      NBL0         NBL1         D4           D5           D6           D7            D8            D9            D10           D11           D12            
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitDef);	
	
	
	//GPIOF pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource11, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FMC);					
	//                      A0           A1           A2           A3           A4           A5           SDNRAS        A6            A7            A8            A9
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitDef);
	
	
	//GPIOG pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_FMC);					
	//                      A10          A11          BA0          BA1          SDCLK        SDNCAS
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitDef);
}

uint8_t TM_SDRAM_Write8(uint32_t address, uint8_t value) {
	if (address > SDRAM_MEMORY_WIDTH - 1) {
		return 0;
	}
	*(uint8_t*) (SDRAM_START_ADR + address) = value;
	return 1;
}

uint8_t TM_SDRAM_Read8(uint32_t address) {
	return  *(__IO uint8_t*) (SDRAM_START_ADR + address);
}

uint8_t TM_SDRAM_Write16(uint32_t address, uint16_t value) {
	if (address > SDRAM_MEMORY_WIDTH - 2) {
		return 0;
	}
	*(uint16_t*) (SDRAM_START_ADR + address) = value;
	return 1;
}

uint16_t TM_SDRAM_Read16(uint32_t address) {
	return  *(__IO uint16_t*) (SDRAM_START_ADR + address);
}

uint8_t TM_SDRAM_Write32(uint32_t address, uint32_t value) {
	if (address > SDRAM_MEMORY_WIDTH - 4) {
		return 0;
	}
	*(uint32_t*) (SDRAM_START_ADR + address) = value;
	return 1;
}

uint32_t TM_SDRAM_Read32(uint32_t address) {
	return  *(__IO uint32_t*) (SDRAM_START_ADR + address);
}

