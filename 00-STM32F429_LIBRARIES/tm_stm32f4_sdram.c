#include "tm_stm32f4_sdram.h"

void TM_SDRAM_Init(void) {
	FMC_SDRAMInitTypeDef FMC_SDRAMInitDef;
	FMC_SDRAMTimingInitTypeDef FMC_SDRAMTimingInitDef;
	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;

	TM_SDRAM_InitPins();
	
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
	
	FMC_SDRAMTimingInitDef.FMC_LoadToActiveDelay = 2;
	FMC_SDRAMTimingInitDef.FMC_ExitSelfRefreshDelay = 7;
	FMC_SDRAMTimingInitDef.FMC_SelfRefreshTime = 4;
	FMC_SDRAMTimingInitDef.FMC_RowCycleDelay = 7;
	FMC_SDRAMTimingInitDef.FMC_WriteRecoveryTime = 2;
	FMC_SDRAMTimingInitDef.FMC_RPDelay = 2;
	FMC_SDRAMTimingInitDef.FMC_RCDDelay = 2;

	FMC_SDRAMInitDef.FMC_Bank = FMC_Bank2_SDRAM;
	FMC_SDRAMInitDef.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_8b;
	FMC_SDRAMInitDef.FMC_RowBitsNumber = FMC_RowBits_Number_12b;
	FMC_SDRAMInitDef.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_16b;
	FMC_SDRAMInitDef.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
	FMC_SDRAMInitDef.FMC_CASLatency = FMC_CAS_Latency_3;
	FMC_SDRAMInitDef.FMC_WriteProtection = FMC_Write_Protection_Disable;
	FMC_SDRAMInitDef.FMC_SDClockPeriod = FMC_SDClock_Period_2;
	FMC_SDRAMInitDef.FMC_ReadBurst = FMC_Read_Burst_Disable;
	FMC_SDRAMInitDef.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_1;
	FMC_SDRAMInitDef.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitDef;
	
	FMC_SDRAMInit(&FMC_SDRAMInitDef);
	
	
	
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_CLK_Enabled;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
	
	
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy));
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy));
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 2;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy));
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0x0231;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy));
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);


	FMC_SetRefreshCount(680);

	while(FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET);
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

void TM_SDRAM_Write8(uint32_t address, uint8_t value) {
	*(uint8_t*) (SDRAM_START_ADR + address) = value;
}

uint8_t TM_SDRAM_Read8(uint32_t address) {
	return  *(__IO uint8_t*)(SDRAM_START_ADR + address);
}

void TM_SDRAM_Write16(uint32_t address, uint16_t value) {
	*(uint16_t*) (SDRAM_START_ADR + address) = value;
}

uint16_t TM_SDRAM_Read16(uint32_t address) {
	return  *(__IO uint16_t*)(SDRAM_START_ADR + address);
}

void TM_SDRAM_Write32(uint32_t address, uint32_t value) {
	*(uint32_t*) (SDRAM_START_ADR + address) = value;
}

uint32_t TM_SDRAM_Read32(uint32_t address) {
	return  *(__IO uint32_t*)(SDRAM_START_ADR + address);
}

