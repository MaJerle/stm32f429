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
#include "tm_stm32f4_i2c.h"

uint32_t TM_I2C_Timeout;

void TM_I2C_Init(I2C_TypeDef* I2Cx, TM_I2C_PinsPack_t pinspack, uint32_t clockSpeed) {
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_ClockSpeed = clockSpeed;
	
#ifdef I2C1
	if (I2Cx == I2C1) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		TM_I2C1_InitPins(pinspack);
		
		I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C1_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = TM_I2C1_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = TM_I2C1_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = TM_I2C1_ACK;
		I2C_InitStruct.I2C_DutyCycle = TM_I2C1_DUTY_CYCLE;
	}
#endif
#ifdef I2C2
	if (I2Cx == I2C2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		TM_I2C2_InitPins(pinspack);
		
		I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C2_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = TM_I2C2_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = TM_I2C2_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = TM_I2C2_ACK;
		I2C_InitStruct.I2C_DutyCycle = TM_I2C2_DUTY_CYCLE;
	}
#endif
#ifdef I2C3
	if (I2Cx == I2C3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
		TM_I2C3_InitPins(pinspack);
		
		I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C3_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = TM_I2C3_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = TM_I2C3_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = TM_I2C3_ACK;
		I2C_InitStruct.I2C_DutyCycle = TM_I2C3_DUTY_CYCLE;
	}
#endif


	I2C_Init(I2Cx, &I2C_InitStruct);

	I2C_Cmd(I2Cx, ENABLE);
}

#ifdef I2C1
void TM_I2C1_InitPins(TM_I2C_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitDef.GPIO_OType = GPIO_OType_OD;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
	
	if (pinspack == TM_I2C_PinsPack_1) {
		//                      SCL          SDA
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
	} else if (pinspack == TM_I2C_PinsPack_2) {
		//                      SCL          SDA
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	}
	GPIO_Init(GPIOB, &GPIO_InitDef);
}
#endif

#ifdef I2C2
void TM_I2C2_InitPins(TM_I2C_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;
	
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitDef.GPIO_OType = GPIO_OType_OD;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
	
	if (pinspack == TM_I2C_PinsPack_1) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
		//                      SCL           SDA
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
		
		GPIO_Init(GPIOB, &GPIO_InitDef);
	} else if (pinspack == TM_I2C_PinsPack_2) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		
		//                      SCL          SDA
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
		
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2);
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2);
		
		GPIO_Init(GPIOF, &GPIO_InitDef);
	}
}
#endif

#ifdef I2C3
void TM_I2C3_InitPins(TM_I2C_PinsPack_t pinspack) {
	GPIO_InitTypeDef GPIO_InitDef;

	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitDef.GPIO_OType = GPIO_OType_OD;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	if (pinspack == TM_I2C_PinsPack_1) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);
		
		//                      SCL
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_8;
		GPIO_Init(GPIOA, &GPIO_InitDef);
		
		//                      SDA
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOC, &GPIO_InitDef);
	} else if (pinspack == TM_I2C_PinsPack_2) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
		
		//                      SCL          SDA
		GPIO_InitDef.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
		
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_I2C3);
		GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_I2C3);
	}
	GPIO_Init(GPIOH, &GPIO_InitDef);
}
#endif

uint8_t TM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
	uint8_t received_data;
	TM_I2C_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	TM_I2C_WriteData(I2Cx, reg);
	TM_I2C_Stop(I2Cx);
	TM_I2C_Start(I2Cx, address, I2C_Direction_Receiver, 0);
	received_data = TM_I2C_ReadNack(I2Cx);
	return received_data;
}

void TM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {
	TM_I2C_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	TM_I2C_WriteData(I2Cx, reg);
	TM_I2C_WriteData(I2Cx, data);
	TM_I2C_Stop(I2Cx);
}

void TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	TM_I2C_Start(I2Cx, address, I2C_Direction_Transmitter, 1);
	TM_I2C_WriteData(I2Cx, reg);
	TM_I2C_Stop(I2Cx);
	TM_I2C_Start(I2Cx, address, I2C_Direction_Receiver, 1);
	for (i = 0; i < count; i++) {
		if (i == (count - 1)) {
			//Last byte
			data[i] = TM_I2C_ReadNack(I2Cx);
		} else {
			data[i] = TM_I2C_ReadAck(I2Cx);
		}
	}
}

void TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	TM_I2C_Start(I2Cx, address, I2C_Direction_Transmitter, 0);
	TM_I2C_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++) {
		TM_I2C_WriteData(I2Cx, data[i]);
	}
	TM_I2C_Stop(I2Cx);
}

int16_t TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) {
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) && TM_I2C_Timeout) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}

	if (ack) {
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}
	
	I2C_Send7bitAddress(I2Cx, address, direction);

	if (direction == I2C_Direction_Transmitter) {
		TM_I2C_Timeout = TM_I2C_TIMEOUT;
		while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) && TM_I2C_Timeout) {
			if (--TM_I2C_Timeout == 0x00) {
				return 1;
			}
		}
	} else if (direction == I2C_Direction_Receiver) {
		TM_I2C_Timeout = TM_I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && TM_I2C_Timeout) {
			if (--TM_I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	I2Cx->SR2;
	
	return 0;
}


void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) {
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) && TM_I2C_Timeout) {
		TM_I2C_Timeout--;
	}
	I2C_SendData(I2Cx, data);
}


uint8_t TM_I2C_ReadAck(I2C_TypeDef* I2Cx) {
	uint8_t data;
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && TM_I2C_Timeout) {
		TM_I2C_Timeout--;
	}

	data = I2C_ReceiveData(I2Cx);
	return data;
}


uint8_t TM_I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) && TM_I2C_Timeout) {
		TM_I2C_Timeout--;
	}

	data = I2C_ReceiveData(I2Cx);
	return data;
}

uint8_t TM_I2C_Stop(I2C_TypeDef* I2Cx) {	
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (((!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF))) && TM_I2C_Timeout) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	return 0;
}

uint8_t TM_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address) {
	uint8_t connected = 0;
	if (!TM_I2C_Start(I2Cx, address, I2C_Direction_Transmitter, 1)) {
		connected = 1;
	}
	TM_I2C_Stop(I2Cx);
	
	return connected;
}

