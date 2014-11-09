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


extern void TM_SPI1_Init(TM_SPI_PinsPack_t pinspack);
extern void TM_SPI2_Init(TM_SPI_PinsPack_t pinspack);
extern void TM_SPI3_Init(TM_SPI_PinsPack_t pinspack);
extern void TM_SPI4_Init(TM_SPI_PinsPack_t pinspack);
extern void TM_SPI5_Init(TM_SPI_PinsPack_t pinspack);
extern void TM_SPI6_Init(TM_SPI_PinsPack_t pinspack);

void TM_SPI_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack) {
	if (SPIx == SPI1) {
		TM_SPI1_Init(pinspack);
	} else if (SPIx == SPI2) {
		TM_SPI2_Init(pinspack);
	} else if (SPIx == SPI3) {
		TM_SPI3_Init(pinspack);
	} else if (SPIx == SPI4) {
		TM_SPI4_Init(pinspack);
	} else if (SPIx == SPI5) {
		TM_SPI5_Init(pinspack);
	} else if (SPIx == SPI6) {
		TM_SPI6_Init(pinspack);
	}
}

uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) {
	/* Fill output buffer with data */
	SPIx->DR = data;
	/* Wait for transmission to complete */
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE));
	/* Wait for received data to complete */
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
	/* Wait for SPI to be ready */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY));
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
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE));
	/* Wait for received data to complete */
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
	/* Wait for SPI to be ready */
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY));
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

void TM_SPI1_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock for GPIOA
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		//Enable clock for GPIOB
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//Pinspack nr. 2        SCK          MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = TM_SPI1_PRESCALER;
	SPI_InitStruct.SPI_DataSize = TM_SPI1_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = TM_SPI1_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = TM_SPI1_MASTERSLAVE;
	if (TM_SPI1_MODE == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI1_MODE == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (TM_SPI1_MODE == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI1_MODE == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

void TM_SPI2_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		//Pinspack nr. 1        	MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_Init(GPIOC, &GPIO_InitStruct);
		//                      	SCK
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//Pinspack nr. 2        	SCK           MISO          MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	} else if (pinspack == TM_SPI_PinsPack_3) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
		//Pinspack nr. 2        	SCK         MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_SPI2);
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = TM_SPI2_PRESCALER;
	SPI_InitStruct.SPI_DataSize = TM_SPI2_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = TM_SPI2_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = TM_SPI2_MASTERSLAVE;
	if (TM_SPI2_MODE == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI2_MODE == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (TM_SPI2_MODE == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI2_MODE == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_Cmd(SPI2, ENABLE);
}

void TM_SPI3_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		//Pinspack nr. 2        SCK           MISO          MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = TM_SPI3_PRESCALER;
	SPI_InitStruct.SPI_DataSize = TM_SPI3_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = TM_SPI3_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = TM_SPI3_MASTERSLAVE;
	if (TM_SPI3_MODE == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI3_MODE == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (TM_SPI3_MODE == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI3_MODE == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI3, &SPI_InitStruct);
	SPI_Cmd(SPI3, ENABLE);
}

void TM_SPI4_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		//Pinspack nr. 2        SCK           MISO          MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
		GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_SPI4);
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = TM_SPI4_PRESCALER;
	SPI_InitStruct.SPI_DataSize = TM_SPI4_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = TM_SPI4_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = TM_SPI4_MASTERSLAVE;
	if (TM_SPI4_MODE == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI4_MODE == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (TM_SPI4_MODE == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI4_MODE == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI4, &SPI_InitStruct);
	SPI_Cmd(SPI4, ENABLE);
}

void TM_SPI5_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);
	} else if (pinspack == TM_SPI_PinsPack_2) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
		//Pinspack nr. 1        SCK          MISO
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOF, &GPIO_InitStruct);
		//                      MOSI
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOF, GPIO_PinSource11, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_SPI5);
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = TM_SPI5_PRESCALER;
	SPI_InitStruct.SPI_DataSize = TM_SPI5_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = TM_SPI5_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = TM_SPI5_MASTERSLAVE;
	if (TM_SPI5_MODE == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI5_MODE == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (TM_SPI5_MODE == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI5_MODE == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI5, &SPI_InitStruct);
	SPI_Cmd(SPI5, ENABLE);
}

void TM_SPI6_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	//Enable clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	//Pinspack nr. 1        SCK           MISO          MOSI
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_SPI6);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_SPI6);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_SPI6);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_BaudRatePrescaler = TM_SPI6_PRESCALER;
	SPI_InitStruct.SPI_DataSize = TM_SPI6_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = TM_SPI6_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = TM_SPI6_MASTERSLAVE;
	if (TM_SPI6_MODE == TM_SPI_Mode_0) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI6_MODE == TM_SPI_Mode_1) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	} else if (TM_SPI6_MODE == TM_SPI_Mode_2) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	} else if (TM_SPI6_MODE == TM_SPI_Mode_3) {
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI6, &SPI_InitStruct);
	SPI_Cmd(SPI6, ENABLE);
}
