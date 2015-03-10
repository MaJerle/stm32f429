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

/* Internal functions */
static void TM_SDRAM_InitPins(void);

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
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout--) {
	
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Configure a PALL (precharge all) command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
	
	/* Wait until the SDRAM controller is ready */  
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) && timeout--) {
	
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Configure a Auto-Refresh command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 8;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) && timeout--) {
	
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Configure a load Mode register command */
	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0x0231;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) && timeout--) {
	
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Set the refresh rate counter */
	/* (7.81 us x Freq) - 20 = (7.81 * 90MHz) - 20 = 683 */
	/* Set the device refresh counter */
	FMC_SetRefreshCount(680);
	
	/* Wait until the SDRAM controller is ready */ 
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout--) {
	
	}
	
	/* Check if everything goes right */
	/* Write 0x10 at location 0x50 and check if result is the same on read operation */
	TM_SDRAM_Write8(0x50, 0x10);
	if (TM_SDRAM_Read8(0x50) == 0x10) {
		/* Initialized OK */
		return 1;
	}
	
	/* Not ok */
	return 0;
}

static void TM_SDRAM_InitPins(void) {	
	/* GPIOB pins */
	TM_GPIO_InitAlternate(GPIOB, GPIO_Pin_5 | GPIO_Pin_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOC pins */
	TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOD pins */
	TM_GPIO_InitAlternate(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOE pins */
	TM_GPIO_InitAlternate(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOF pins */
	TM_GPIO_InitAlternate(GPIOF, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOG pins */
	TM_GPIO_InitAlternate(GPIOG, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
}
