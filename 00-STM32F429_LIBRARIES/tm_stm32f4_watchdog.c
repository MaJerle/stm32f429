#include "tm_stm32f4_watchdog.h"

uint8_t TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_t timeout) {
	uint8_t result = 0;

	// Check if the system has resumed from IWDG reset
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
		// Reset by IWDG
		result = 1;
		
		// Clear reset flags
		RCC_ClearFlag();
	}

	// Enable write access to IWDG_PR and IWDG_RLR registers
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	// IWDG counter clock: LSI/32 = 1024Hz
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	// Set counter reload value.  
	if (timeout == TM_WATCHDOG_Timeout_5ms) {
		IWDG_SetReload(5);
	} else if (timeout == TM_WATCHDOG_Timeout_10ms) {
		IWDG_SetReload(10);
	} else if (timeout == TM_WATCHDOG_Timeout_15ms) {
		IWDG_SetReload(15);
	} else if (timeout == TM_WATCHDOG_Timeout_30ms) {
		IWDG_SetReload(31);
	} else if (timeout == TM_WATCHDOG_Timeout_60ms) {
		IWDG_SetReload(61);
	} else if (timeout == TM_WATCHDOG_Timeout_120ms) {
		IWDG_SetReload(123);
	} else if (timeout == TM_WATCHDOG_Timeout_250ms) {
		IWDG_SetReload(256);
	} else if (timeout == TM_WATCHDOG_Timeout_500ms) {
		IWDG_SetReload(512);
	} else if (timeout == TM_WATCHDOG_Timeout_1s) {
		IWDG_SetReload(1024);
	} else if (timeout == TM_WATCHDOG_Timeout_2s) {
		IWDG_SetReload(2048);
	} else if (timeout == TM_WATCHDOG_Timeout_4s) {
		IWDG_SetReload(4095);
	}

	// Reload IWDG counter
	IWDG_ReloadCounter();

	// Enable IWDG (the LSI oscillator will be enabled by hardware)
	IWDG_Enable();
	
	return result;
}

void TM_WATCHDOG_Reset(void) {
	// Reload IWDG counter
    IWDG_ReloadCounter(); 
}

