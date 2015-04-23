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

/* Only for these devices */
#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)

/* Internal functions */
static void TM_SDRAM_InitPins(void);

uint8_t TM_SDRAM_Init(void) {
	FMC_SDRAMInitTypeDef FMC_SDRAMInitStructure;
	FMC_SDRAMTimingInitTypeDef FMC_SDRAMTimingInitStructure;
	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
	volatile uint32_t timeout = SDRAM_TIMEOUT;
	static uint8_t initialized = 0;
	
	/* Already initialized */
	if (initialized) {
		return 1;
	}
	
	/* Initialize FMC pins */
	TM_SDRAM_InitPins();
	
	/* Enable FMC clock */
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	
	/* FMC SDRAM device initialization sequence --------------------------------*/ 
	/* Step 1 ----------------------------------------------------*/ 
	/* Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2) */
	/* TMRD: 2 Clock cycles */
	/* 1 clock cycle = 1 / 90MHz = 11.1ns */
	FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay		= 2;      
	/* TXSR: min=70ns (7x11.10ns) */
	FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay	= 7;
	/* TRAS: min=42ns (4x11.10ns) max=120k (ns) */
	FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime		= 4;
	/* TRC:  min=70 (7x11.10ns) */        
	FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay			= 7;         
	/* TWR:  min=1+ 7ns (1+1x11.10ns) */
	FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime		= 2;      
	/* TRP:  20ns => 2x11.10ns */
	FMC_SDRAMTimingInitStructure.FMC_RPDelay				= 2;                
	/* TRCD: 20ns => 2x11.10ns */
	FMC_SDRAMTimingInitStructure.FMC_RCDDelay				= 2;
	
	
	/* FMC SDRAM control configuration */
	FMC_SDRAMInitStructure.FMC_Bank 						= SDRAM_BANK;
	/* Row addressing: [7:0] */
	FMC_SDRAMInitStructure.FMC_ColumnBitsNumber 			= FMC_ColumnBits_Number_8b;
	/* Column addressing: [11:0] */
	FMC_SDRAMInitStructure.FMC_RowBitsNumber      			= SDRAM_ROWBITS_NUMBER;
	FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth  			= SDRAM_MEMORY_WIDTH;
	FMC_SDRAMInitStructure.FMC_InternalBankNumber 			= FMC_InternalBank_Number_4;
	/* CL: Cas Latency = 3 clock cycles */
	FMC_SDRAMInitStructure.FMC_CASLatency              	    = FMC_CAS_Latency_3;
	FMC_SDRAMInitStructure.FMC_WriteProtection 				= FMC_Write_Protection_Disable;
	FMC_SDRAMInitStructure.FMC_SDClockPeriod 				= FMC_SDClock_Period_2;
	FMC_SDRAMInitStructure.FMC_ReadBurst 					= SDRAM_READ_BURST_STATE;
	FMC_SDRAMInitStructure.FMC_ReadPipeDelay 				= FMC_ReadPipe_Delay_1;
	FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct 			= &FMC_SDRAMTimingInitStructure;
	
	/* FMC SDRAM bank initialization */
	FMC_SDRAMInit(&FMC_SDRAMInitStructure);
	
	/* SDRAM Init sequence */
	
	/* Configure a clock configuration enable command */
	FMC_SDRAMCommandStructure.FMC_CommandMode				= FMC_Command_Mode_CLK_Enabled;
	FMC_SDRAMCommandStructure.FMC_CommandTarget 			= SDRAM_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber 		= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(SDRAM_BANK, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Little delay */
	timeout = SDRAM_TIMEOUT * 0x110;
	while (timeout--);
	
	/* Configure a PALL (precharge all) command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode          		= FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          	= SDRAM_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      	= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(SDRAM_BANK, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Configure a Auto-Refresh command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode            	= FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          	= SDRAM_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      	= 8;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(SDRAM_BANK, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Configure a load Mode register command */
	FMC_SDRAMCommandStructure.FMC_CommandMode            	= FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          	= SDRAM_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      	= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= (uint32_t)SDRAM_REG_VALUE;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(SDRAM_BANK, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Set the refresh rate counter */
	/* (7.81 us x Freq) - 20 = (7.81 * 90MHz) - 20 = 683 */
	/* Set the device refresh counter */
	FMC_SetRefreshCount(SDRAM_REFRESH_COUNT);
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(SDRAM_BANK, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Check if everything goes right */
	/* Write 0x45 at location 0x50 and check if result is the same on read operation */
	TM_SDRAM_Write8(0x50, 0x45);
	
	/* Read and check */
	if (TM_SDRAM_Read8(0x50) == 0x45) {
		/* Initialized OK */
		initialized = 1;
		/* Initialized OK */
		return 1;
	}
	
	/* Not initialized OK */
	initialized = 0;
	
	/* Not ok */
	return 0;
}

static void TM_SDRAM_InitPins(void) {
	/* Try to initialize from user */
	if (TM_SDRAM_InitCustomPinsCallback()) {
		/* User has initialized pins by itself */
		return;
	}
#if defined(SDRAM_USE_STM324x9_EVAL)
	/* GPIOD pins */
	TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOE pins */
	TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOF pins */
	TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOG pins */
	TM_GPIO_InitAlternate(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOH pins */
	TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOI pins */
	TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
#else
	/* GPIOB pins */
	TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_5 | GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOC pins */
	TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOD pins */
	TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOE pins */
	TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOF pins */
	TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOG pins */
	TM_GPIO_InitAlternate(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
#endif
}

__weak uint8_t TM_SDRAM_InitCustomPinsCallback(void) {
	/* If you need custom implementation, then create this function externally. */
	/* This function here should not be modified */
	
	/* Return 0, lib will use default pins */
	return 0;
}

#endif
