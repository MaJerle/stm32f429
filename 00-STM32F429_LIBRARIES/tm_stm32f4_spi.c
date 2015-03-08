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
#include "tm_stm32f4_spi.h"

/* Private functions */
static void TM_SPI1_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);
static void TM_SPI2_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);
static void TM_SPI3_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);
static void TM_SPI4_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);
static void TM_SPI5_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);
static void TM_SPI6_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);

void TM_SPI_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack) {
	/* Init with default settings */
	if (SPIx == SPI1) {
		TM_SPI1_Init(pinspack, TM_SPI1_MODE, TM_SPI1_PRESCALER, TM_SPI1_MASTERSLAVE, TM_SPI1_FIRSTBIT);
	} else if (SPIx == SPI2) {
		TM_SPI2_Init(pinspack, TM_SPI2_MODE, TM_SPI2_PRESCALER, TM_SPI2_MASTERSLAVE, TM_SPI2_FIRSTBIT);
	} else if (SPIx == SPI3) {
		TM_SPI3_Init(pinspack, TM_SPI3_MODE, TM_SPI3_PRESCALER, TM_SPI3_MASTERSLAVE, TM_SPI3_FIRSTBIT);
	} else if (SPIx == SPI4) {
		TM_SPI4_Init(pinspack, TM_SPI4_MODE, TM_SPI4_PRESCALER, TM_SPI4_MASTERSLAVE, TM_SPI4_FIRSTBIT);
	} else if (SPIx == SPI5) {
		TM_SPI5_Init(pinspack, TM_SPI5_MODE, TM_SPI5_PRESCALER, TM_SPI5_MASTERSLAVE, TM_SPI5_FIRSTBIT);
	} else if (SPIx == SPI6) {
		TM_SPI6_Init(pinspack, TM_SPI6_MODE, TM_SPI6_PRESCALER, TM_SPI6_MASTERSLAVE, TM_SPI6_FIRSTBIT);
	}
}

void TM_SPI_InitWithMode(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode) {
	/* Init with custom mode, 0, 1, 2, 3 */
	if (SPIx == SPI1) {
		TM_SPI1_Init(pinspack, SPI_Mode, TM_SPI1_PRESCALER, TM_SPI1_MASTERSLAVE, TM_SPI1_FIRSTBIT);
	} else if (SPIx == SPI2) {
		TM_SPI2_Init(pinspack, SPI_Mode, TM_SPI2_PRESCALER, TM_SPI2_MASTERSLAVE, TM_SPI2_FIRSTBIT);
	} else if (SPIx == SPI3) {
		TM_SPI3_Init(pinspack, SPI_Mode, TM_SPI3_PRESCALER, TM_SPI3_MASTERSLAVE, TM_SPI3_FIRSTBIT);
	} else if (SPIx == SPI4) {
		TM_SPI4_Init(pinspack, SPI_Mode, TM_SPI4_PRESCALER, TM_SPI4_MASTERSLAVE, TM_SPI4_FIRSTBIT);
	} else if (SPIx == SPI5) {
		TM_SPI5_Init(pinspack, SPI_Mode, TM_SPI5_PRESCALER, TM_SPI5_MASTERSLAVE, TM_SPI5_FIRSTBIT);
	} else if (SPIx == SPI6) {
		TM_SPI6_Init(pinspack, SPI_Mode, TM_SPI6_PRESCALER, TM_SPI6_MASTERSLAVE, TM_SPI6_FIRSTBIT);
	}
}

void TM_SPI_InitFull(
	SPI_TypeDef* SPIx, 
	TM_SPI_PinsPack_t pinspack,
	uint16_t SPI_BaudRatePrescaler,
	TM_SPI_Mode_t SPI_Mode_t,
	uint16_t SPI_Mode,
	uint16_t SPI_FirstBit\
) {
	/* Init FULL SPI settings */
	if (SPIx == SPI1) {
		TM_SPI1_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
	} else if (SPIx == SPI2) {
		TM_SPI2_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
	} else if (SPIx == SPI3) {
		TM_SPI3_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
	} else if (SPIx == SPI4) {
		TM_SPI4_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
	} else if (SPIx == SPI5) {
		TM_SPI5_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
	} else if (SPIx == SPI6) {
		TM_SPI6_Init(pinspack, SPI_Mode_t, SPI_BaudRatePrescaler, SPI_Mode, SPI_FirstBit);
	}
}

uint16_t TM_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency) {
	RCC_ClocksTypeDef RCC_Clocks;
	uint32_t APB_Frequency;
	uint8_t i;
	
	/* Prevent false input */
	if (MAX_SPI_Frequency == 0) {
		return SPI_BaudRatePrescaler_256;
	}
	
	/* Get clock values from RCC */
	RCC_GetClocksFreq(&RCC_Clocks);
	
	/* Calculate max SPI clock */
	if (SPIx == SPI1 || SPIx == SPI4 || SPIx == SPI5 || SPIx == SPI6) {
		APB_Frequency = RCC_Clocks.PCLK2_Frequency;
	} else {
		APB_Frequency = RCC_Clocks.PCLK1_Frequency;
	}
	
	/* Calculate prescaler value */
	/* Bits 5:3 in CR1 SPI registers are prescalers */
	/* 000 = 2, 001 = 4, 002 = 8, ..., 111 = 256 */
	for (i = 0; i < 8; i++) {
		if (APB_Frequency / (1 << (i + 1)) <= MAX_SPI_Frequency) {
			/* Bits for BP are 5:3 in CR1 register */
			return (i << 3);
		}
	}
	
	/* Use max prescaler possible */
	return SPI_BaudRatePrescaler_256;
}

uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) {
	/* Fill output buffer with data */
	SPIx->DR = data;
	/* Wait for transmission to complete */
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0);
	/* Wait for received data to complete */
	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0);
	/* Wait for SPI to be ready */
	while (SPIx->SR & SPI_I2S_FLAG_BSY);
	/* Return data from buffer */
	return SPIx->DR;
}

void TM_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint16_t count) {
	uint16_t i;
	for (i = 0; i < count; i++) {
		dataIn[i] = TM_SPI_Send(SPIx, dataOut[i]);
	}
}

void TM_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint16_t count) {
	uint16_t i;
	for (i = 0; i < count; i++) {
		TM_SPI_Send(SPIx, dataOut[i]);
	}
}

void TM_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint16_t count) {
	uint16_t i;
	for (i = 0; i < count; i++) {
		dataIn[i] = TM_SPI_Send(SPIx, dummy);
	}
}

uint16_t TM_SPI_Send16(SPI_TypeDef* SPIx, uint16_t data) {
	/* Fill output buffer with data */
	SPIx->DR = data;
	/* Wait for transmission to complete */
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0);
	/* Wait for received data to complete */
	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0);
	/* Wait for SPI to be ready */
	while (SPIx->SR & SPI_I2S_FLAG_BSY);
	/* Return data from buffer */
	return SPIx->DR;
}

void TM_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint16_t count) {
	uint16_t i;
	for (i = 0; i < count; i++) {
		dataIn[i] = TM_SPI_Send16(SPIx, dataOut[i]);
	}
}

void TM_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t count) {
	uint16_t i;
	for (i = 0; i < count; i++) {
		TM_SPI_Send16(SPIx, dataOut[i]);
	}
}

void TM_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint16_t count) {
	uint16_t i;
	for (i = 0; i < count; i++) {
		dataIn[i] = TM_SPI_Send16(SPIx, dummy);
	}
}


/* Private functions */
static void TM_SPI1_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {
	SPI_InitTypeDef SPI_InitStruct;

	/* Init SPI pins */
	if (pinspack == TM_SPI_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI1);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI1);
	}

	/* Enable SPI clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* Fill SPI settings */
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = TM_SPI1_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	if (SPI_Mode == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (SPI_Mode == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	/* Deinit first */
	SPI_Cmd(SPI1, DISABLE);
	SPI_DeInit(SPI1);
	
	/* Init SPI */
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

static void TM_SPI2_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {
	SPI_InitTypeDef SPI_InitStruct;

	/* Init SPI pins */
	if (pinspack == TM_SPI_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_Pin_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI2);
		TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_2 | GPIO_Pin_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI2);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI2);
	} else if (pinspack == TM_SPI_PinsPack_3) {
		TM_GPIO_InitAlternate(GPIOI, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI2);
	}

	/* Enable SPI clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	/* Fill SPI settings */
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = TM_SPI2_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	if (SPI_Mode == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (SPI_Mode == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	/* Deinit first */
	SPI_Cmd(SPI2, DISABLE);
	SPI_DeInit(SPI2);
	
	/* Init SPI */
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_Cmd(SPI2, ENABLE);
}

static void TM_SPI3_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {
	SPI_InitTypeDef SPI_InitStruct;

	/* Enable SPI pins */
	if (pinspack == TM_SPI_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOI, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI3);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI3);
	}

	/* Enable SPI clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	/* Fill SPI settings */
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = TM_SPI3_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	if (SPI_Mode == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (SPI_Mode == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	/* Deinit first */
	SPI_Cmd(SPI3, DISABLE);
	SPI_DeInit(SPI3);
	
	/* Init SPI */
	SPI_Init(SPI3, &SPI_InitStruct);
	SPI_Cmd(SPI3, ENABLE);
}

static void TM_SPI4_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {
	SPI_InitTypeDef SPI_InitStruct;

	/* Init SPI pins */
	if (pinspack == TM_SPI_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOE, GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI4);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOE, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI4);
	}

	/* Enable SPI clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	/* Fill SPI settings */
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = TM_SPI4_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	if (SPI_Mode == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (SPI_Mode == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	/* Deinit first */
	SPI_Cmd(SPI4, DISABLE);
	SPI_DeInit(SPI4);
	
	/* Init SPI */
	SPI_Init(SPI4, &SPI_InitStruct);
	SPI_Cmd(SPI4, ENABLE);
}

static void TM_SPI5_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {
	SPI_InitTypeDef SPI_InitStruct;

	/* Init SPI pins */
	if (pinspack == TM_SPI_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOF, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI5);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOF, GPIO_Pin_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI5);
		TM_GPIO_InitAlternate(GPIOH, GPIO_Pin_6 | GPIO_Pin_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI5);
	}

	/* Enable SPI clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

	/* Fill SPI settings */
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = TM_SPI5_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	if (SPI_Mode == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (SPI_Mode == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	/* Deinit first */
	SPI_Cmd(SPI5, DISABLE);
	SPI_DeInit(SPI5);
	
	/* Init SPI */
	SPI_Init(SPI5, &SPI_InitStruct);
	SPI_Cmd(SPI5, ENABLE);
}

static void TM_SPI6_Init(TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {
	SPI_InitTypeDef SPI_InitStruct;

	/* Init SPI pins */
	TM_GPIO_InitAlternate(GPIOG, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_SPI6);
	
	/* Enable SPI clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);

	/* Fill SPI settings */
	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_DataSize = TM_SPI6_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit;
	SPI_InitStruct.SPI_Mode = SPI_MasterSlave;
	if (SPI_Mode == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (SPI_Mode == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (SPI_Mode == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	/* Deinit first */
	SPI_Cmd(SPI6, DISABLE);
	SPI_DeInit(SPI6);
	
	/* Init SPI */
	SPI_Init(SPI6, &SPI_InitStruct);
	SPI_Cmd(SPI6, ENABLE);
}
