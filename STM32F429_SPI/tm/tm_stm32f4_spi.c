#include "tm_stm32f4_spi.h"

uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) {
	//Fill output buffer with data
	SPIx->DR = data;
	//Wait for transmission to complete
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE));
	//Wait for received data to complete
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE));
	//Wait for SPI to be ready
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY));
	//Return data from buffer
	return SPIx->DR;
}

void TM_SPI1_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	//Common settings for all pins
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock for GPIOA
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	} else {
		//Enable clock for GPIOB
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//Pinspack nr. 2        SCK          MISO         MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_StructInit(&SPI_InitDef);
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitDef);
	SPI_Cmd(SPI1, ENABLE);
}


void TM_SPI2_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//Common settings for all pins
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		//Pinspack nr. 1        MISO         MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_Init(GPIOC, &GPIO_InitDef);
		//                      SCK
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
	} else {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//Pinspack nr. 2        SCK           MISO          MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_StructInit(&SPI_InitDef);
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_InitDef);
	SPI_Cmd(SPI2, ENABLE);
}


void TM_SPI3_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	//Common settings for all pins
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	} else {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		//Pinspack nr. 2        SCK           MISO          MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_Init(GPIOC, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	SPI_StructInit(&SPI_InitDef);
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI3, &SPI_InitDef);
	SPI_Cmd(SPI3, ENABLE);
}

void TM_SPI4_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	//Common settings for all pins
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_Init(GPIOE, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	} else {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		//Pinspack nr. 2        SCK           MISO          MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
		GPIO_Init(GPIOE, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_SPI4);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_SPI4);
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	SPI_StructInit(&SPI_InitDef);
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI4, &SPI_InitDef);
	SPI_Cmd(SPI4, ENABLE);
}

void TM_SPI5_Init(TM_SPI_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	//Common settings for all pins
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;

	if (pinspack == TM_SPI_PinsPack_1) {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		//Pinspack nr. 1        SCK          MISO         MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_Init(GPIOF, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);
	} else {
		//Enable clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
		//Pinspack nr. 1        SCK          MISO
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOF, &GPIO_InitDef);
		//                      MOSI
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOF, &GPIO_InitDef);

		GPIO_PinAFConfig(GPIOF, GPIO_PinSource11, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_SPI5);
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_SPI5);
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

	SPI_StructInit(&SPI_InitDef);
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI5, &SPI_InitDef);
	SPI_Cmd(SPI5, ENABLE);
}

void TM_SPI6_Init(void) {
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;

	//Enable clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	//Common settings for all pins
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	//Pinspack nr. 1        SCK           MISO          MOSI
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitDef);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_SPI6);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_SPI6);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_SPI6);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);

	SPI_StructInit(&SPI_InitDef);
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI6, &SPI_InitDef);
	SPI_Cmd(SPI6, ENABLE);
}
