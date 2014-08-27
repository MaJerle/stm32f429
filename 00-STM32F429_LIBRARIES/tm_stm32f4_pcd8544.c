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
#include "tm_stm32f4_pcd8544.h"

unsigned char PCD8544_Buffer[PCD8544_BUFFER_SIZE];
unsigned char PCD8544_UpdateXmin = 0, PCD8544_UpdateXmax = 0, PCD8544_UpdateYmin = 0, PCD8544_UpdateYmax = 0;
unsigned char PCD8544_x;
unsigned char PCD8544_y;

//Fonts 5x7
const uint8_t PCD8544_Font5x7 [97][PCD8544_CHAR5x7_WIDTH] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x32, 0x34, 0x08, 0x16, 0x26 },   // %
	{ 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
	{ 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
	{ 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
	{ 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
	{ 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
	{ 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
	{ 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
	{ 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
	{ 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x00, 0x7F, 0x3E, 0x1C, 0x08 },   // > Filled		123
	{ 0x08, 0x1C, 0x3E, 0x7F, 0x00 }, 	 // < Filled	124
	{ 0x08, 0x7C, 0x7E, 0x7C, 0x08 },   // Arrow up		125
	{ 0x10, 0x3E, 0x7E, 0x3E, 0x10 },   // Arrow down	126
	{ 0x3E, 0x3E, 0x3E, 0x3E, 0x3E },   // Stop			127
	{ 0x00, 0x7F, 0x3E, 0x1C, 0x08 }    // Play			128
};

const uint8_t PCD8544_Font3x5[106][3] = {
	{ 0x00, 0x00, 0x00 },   // sp - 32
	{ 0x00, 0x17, 0x00 },   // ! - 33
	{ 0x03, 0x00, 0x03 },   // " - 34
	{ 0x1F, 0x0A, 0x1F },   // # - 35
	{ 0x0A, 0x1F, 0x05 },   // $
	{ 0x09, 0x04, 0x12 },   // %
	{ 0x0F, 0x17, 0x1C },   // &
	{ 0x00, 0x03, 0x00 },   // '
	{ 0x00, 0x0E, 0x11 },   // ( - 40
	{ 0x11, 0x0E, 0x00 },   // )
	{ 0x05, 0x02, 0x05 },   // *
	{ 0x04, 0x0E, 0x04 },   // +
	{ 0x10, 0x08, 0x00 },   // ,
	{ 0x04, 0x04, 0x04 },   // - - 45
	{ 0x00, 0x10, 0x00 },   // .
	{ 0x08, 0x04, 0x02 },   // /
	{ 0x1F, 0x11, 0x1F },   // 0
	{ 0x12, 0x1F, 0x10 },   // 1
	{ 0x1D, 0x15, 0x17 },   // 2 - 50
	{ 0x11, 0x15, 0x1F },   // 3
	{ 0x07, 0x04, 0x1F },   // 4
	{ 0x17, 0x15, 0x1D },   // 5
	{ 0x1F, 0x15, 0x1D },   // 6
	{ 0x01, 0x01, 0x1F },   // 7 - 55
	{ 0x1F, 0x15, 0x1F },   // 8
	{ 0x17, 0x15, 0x1F },   // 9 - 57
	{ 0x00, 0x0A, 0x00 },   // :
	{ 0x10, 0x0A, 0x00 },   // ;
	{ 0x04, 0x0A, 0x11 },   // < - 60
	{ 0x0A, 0x0A, 0x0A },   // =
	{ 0x11, 0x0A, 0x04 },   // >
	{ 0x01, 0x15, 0x03 },   // ?
	{ 0x0E, 0x15, 0x16 },   // @
	{ 0x1E, 0x05, 0x1E },   // A - 65
	{ 0x1F, 0x15, 0x0A },   // B
	{ 0x0E, 0x11, 0x11 },   // C
	{ 0x1F, 0x11, 0x0E },   // D
	{ 0x1F, 0x15, 0x15 },   // E
	{ 0x1F, 0x05, 0x05 },   // F - 70
	{ 0x0E, 0x15, 0x1D },   // G
	{ 0x1F, 0x04, 0x1F },   // H
	{ 0x11, 0x1F, 0x11 },   // I
	{ 0x08, 0x10, 0x0F },   // J
	{ 0x1F, 0x04, 0x1B },   // K - 75
	{ 0x1F, 0x10, 0x10 },   // L
	{ 0x1F, 0x06, 0x1F },   // M
	{ 0x1F, 0x0E, 0x1F },   // N
	{ 0x0E, 0x11, 0x0E },   // O
	{ 0x1F, 0x05, 0x02 },   // P - 80
	{ 0x0E, 0x11, 0x1E },   // Q
	{ 0x1F, 0x0D, 0x16 },   // R
	{ 0x12, 0x15, 0x09 },   // S
	{ 0x01, 0x1F, 0x01 },   // T
	{ 0x0F, 0x10, 0x0F },   // U - 85
	{ 0x07, 0x18, 0x07 },   // V
	{ 0x1F, 0x0C, 0x1F },   // W
	{ 0x1B, 0x04, 0x1B },   // X
	{ 0x03, 0x1C, 0x03 },   // Y
	{ 0x19, 0x15, 0x13 },   // Z - 90
	{ 0x1F, 0x11, 0x00 },   // [
	{ 0x02, 0x04, 0x08 },   // 55 - backspace - 92
	{ 0x00, 0x11, 0x1F },   // ]
	{ 0x02, 0x01, 0x02 },   // ^
	{ 0x10, 0x10, 0x10 },   // _ - 95
	{ 0x01, 0x02, 0x00 },   // `
	{ 0x1A, 0x16, 0x1C },   // a
	{ 0x1F, 0x12, 0x0C },   // b
	{ 0x0C, 0x12, 0x12 },   // c
	{ 0x0C, 0x12, 0x1F },   // d - 100
	{ 0x0C, 0x1A, 0x16 },   // e
	{ 0x04, 0x1E, 0x05 },   // f
	{ 0x06, 0x15, 0x0F },   // g
	{ 0x1F, 0x02, 0x1C },   // h
	{ 0x00, 0x1D, 0x00 },   // i - 105
	{ 0x10, 0x10, 0x0D },   // j
	{ 0x1F, 0x0C, 0x12 },   // k
	{ 0x11, 0x1F, 0x10 },   // l
	{ 0x1E, 0x0E, 0x1E },   // m
	{ 0x1E, 0x02, 0x1C },   // n - 110
	{ 0x0C, 0x12, 0x0C },   // o
	{ 0x1E, 0x0A, 0x04 },   // p
	{ 0x04, 0x0A, 0x1E },   // q
	{ 0x1C, 0x02, 0x02 },   // r
	{ 0x14, 0x1E, 0x0A },   // s - 115
	{ 0x02, 0x1F, 0x12 },   // t
	{ 0x0E, 0x10, 0x1E },   // u
	{ 0x0E, 0x10, 0x0E },   // v
	{ 0x1E, 0x1C, 0x1E },   // w
	{ 0x12, 0x0C, 0x12 },   // x - 120
	{ 0x02, 0x14, 0x1E },   // y
	{ 0x1A, 0x1E, 0x16 },   // z
	{ 0x04, 0x1B, 0x11 },   // {
	{ 0x00, 0x1F, 0x00 },   // |
	{ 0x11, 0x1B, 0x04 },   // }
	{ 0x04, 0x06, 0x02 },   // ~
	{ 0x1F, 0x1F, 0x1F },   // delete
};


void PCD8544_InitIO(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for all pins
	RCC_AHB1PeriphClockCmd(PCD8544_RST_RCC | PCD8544_CE_RCC | PCD8544_DC_RCC, ENABLE);

	//Common settings for all pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	
	//RST pin
	GPIO_InitStruct.GPIO_Pin = PCD8544_RST_PIN;
	GPIO_Init(PCD8544_RST_PORT, &GPIO_InitStruct);
	
	//DC pin
	GPIO_InitStruct.GPIO_Pin = PCD8544_DC_PIN;
	GPIO_Init(PCD8544_DC_PORT, &GPIO_InitStruct);
	
	//CE pin
	GPIO_InitStruct.GPIO_Pin = PCD8544_CE_PIN;
	GPIO_Init(PCD8544_CE_PORT, &GPIO_InitStruct);

	//Reset pin HIGH
	GPIO_SetBits(PCD8544_RST_PORT, PCD8544_RST_PIN);
	
	//CE HIGH
	PCD8544_CE_HIGH;

	//Initialize SPI
	TM_SPI_Init(PCD8544_SPI, PCD8544_SPI_PINSPACK);
}

void PCD8544_send(unsigned char data) {
	PCD8544_CE_LOW;
	TM_SPI_Send(PCD8544_SPI, data);
	PCD8544_CE_HIGH;
}

void PCD8544_Pin(PCD8544_Pin_t pin, PCD8544_State_t state) {
	switch (pin) {
		case PCD8544_Pin_DC:
			if (state != PCD8544_State_Low) {
				GPIO_SetBits(PCD8544_DC_PORT, PCD8544_DC_PIN);
			} else {
				GPIO_ResetBits(PCD8544_DC_PORT, PCD8544_DC_PIN);
			}
			break;
		case PCD8544_Pin_RST:
			if (state != PCD8544_State_Low) {
				GPIO_SetBits(PCD8544_RST_PORT, PCD8544_RST_PIN);
			} else {
				GPIO_ResetBits(PCD8544_RST_PORT, PCD8544_RST_PIN);
			}
			break;
		default: break;
	}
}


void PCD8544_Delay(unsigned long micros) {
	volatile unsigned long i;
	for (i = 0; i < micros; i++) {

	}
}

void PCD8544_Init(unsigned char contrast) {
	//Initialize IO's
	PCD8544_InitIO();
	//Reset
	PCD8544_Pin(PCD8544_Pin_RST, PCD8544_State_Low);
	PCD8544_Delay(10000);
	PCD8544_Pin(PCD8544_Pin_RST, PCD8544_State_High);

	// Go in extended mode
	PCD8544_Write(PCD8544_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);

	// LCD bias select
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETBIAS | 0x4);

	// set VOP
	if (contrast > 0x7F) {
		contrast = 0x7F;
	}
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETVOP | contrast);

	// normal mode
	PCD8544_Write(PCD8544_COMMAND, PCD8544_FUNCTIONSET);

	// Set display to Normal
	PCD8544_Write(PCD8544_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

	//Set cursor to home position
	PCD8544_Home();

	//Normal display
	PCD8544_Write(PCD8544_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

	//Clear display
	PCD8544_Clear();
}

void PCD8544_Write(PCD8544_WriteType_t cd, unsigned char data) {
	switch (cd) {
		//Send data to lcd's ram
		case PCD8544_DATA:
			//Set DC pin HIGH
			PCD8544_Pin(PCD8544_Pin_DC, PCD8544_State_High);
			break;
		//Send command to lcd
		case PCD8544_COMMAND:
			//Set DC pin LOW
			PCD8544_Pin(PCD8544_Pin_DC, PCD8544_State_Low);
			break;
		default: break;
	}
	//Send data
	PCD8544_send(data);
}

void PCD8544_SetContrast(unsigned char contrast) {
	// Go in extended mode
	PCD8544_Write(PCD8544_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);

	// set VOP
	if (contrast > 0x7F) {
		contrast = 0x7F;
	}
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETVOP | contrast);

	// normal mode
	PCD8544_Write(PCD8544_COMMAND, PCD8544_FUNCTIONSET);
}

void PCD8544_DrawPixel(unsigned char x, unsigned char y, PCD8544_Pixel_t pixel) {
	if (x >= PCD8544_WIDTH) {
		return;
	}
	if (y >= PCD8544_HEIGHT) {
		return;
	}

	if (pixel != PCD8544_Pixel_Clear) {
		PCD8544_Buffer[x + (y / 8) * PCD8544_WIDTH] |= 1 << (y % 8);
	} else {
		PCD8544_Buffer[x + (y / 8) * PCD8544_WIDTH] &= ~(1 << (y % 8));
	}
	PCD8544_UpdateArea(x, y, x, y);
}

void PCD8544_Invert(PCD8544_Invert_t invert) {
	if (invert != PCD8544_Invert_No) {
		PCD8544_Write(PCD8544_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED);
	} else {
		PCD8544_Write(PCD8544_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
	}
}

void PCD8544_Clear(void) {
	unsigned int i;
	PCD8544_Home();
	for (i = 0; i < PCD8544_BUFFER_SIZE; i++) {
		PCD8544_Buffer[i] = 0x00;
		//PCD8544_Write(PCD8544_DATA, 0x00);
	}
	PCD8544_GotoXY(0, 0);
	PCD8544_UpdateArea(0, 0, PCD8544_WIDTH - 1, PCD8544_HEIGHT - 1);
	PCD8544_Refresh();
}

void PCD8544_Home(void) {
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETXADDR | 0);
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETYADDR | 0);
}

void PCD8544_Refresh(void) {
	unsigned char i, j;
	for (i = 0; i < 6; i++) {
		//Not in range yet
		if (PCD8544_UpdateYmin > ((i + 1) * 8)) {
			continue;
		}
		//Over range, stop
		if ((i * 8) > PCD8544_UpdateYmax) {
			break;
		}

		PCD8544_Write(PCD8544_COMMAND, PCD8544_SETYADDR | i);
		PCD8544_Write(PCD8544_COMMAND, PCD8544_SETXADDR | PCD8544_UpdateXmin);

		for (j = PCD8544_UpdateXmin; j <= PCD8544_UpdateXmax; j++) {
			PCD8544_Write(PCD8544_DATA, PCD8544_Buffer[(i * PCD8544_WIDTH) + j]);
		}
	}

	PCD8544_UpdateXmin = PCD8544_WIDTH - 1;
	PCD8544_UpdateXmax = 0;
	PCD8544_UpdateYmin = PCD8544_HEIGHT - 1;
	PCD8544_UpdateYmax = 0;
}

void PCD8544_UpdateArea(unsigned char xMin, unsigned char yMin, unsigned char xMax, unsigned char yMax) {
	if (xMin < PCD8544_UpdateXmin) {
		PCD8544_UpdateXmin = xMin;
	}
	if (xMax > PCD8544_UpdateXmax) {
		PCD8544_UpdateXmax = xMax;
	}
	if (yMin < PCD8544_UpdateYmin) {
		PCD8544_UpdateYmin = yMin;
	}
	if (yMax > PCD8544_UpdateYmax) {
		PCD8544_UpdateYmax = yMax;
	}
}

void PCD8544_GotoXY(unsigned char x, unsigned char y) {
	PCD8544_x = x;
	PCD8544_y = y;
}

void PCD8544_Putc(char c, PCD8544_Pixel_t color, PCD8544_FontSize_t size) {
	unsigned char c_height, c_width, i, b, j;
	if (size == PCD8544_FontSize_3x5) {
		c_width = PCD8544_CHAR3x5_WIDTH;
		c_height = PCD8544_CHAR3x5_HEIGHT;
	} else {
		c_width = PCD8544_CHAR5x7_WIDTH;
		c_height = PCD8544_CHAR5x7_HEIGHT;
	}
	if ((PCD8544_x + c_width) > PCD8544_WIDTH) {
		//If at the end of a line of display, go to new line and set x to 0 position
		PCD8544_y += c_height;
		PCD8544_x = 0;
	}
	for (i = 0; i < c_width - 1; i++) {
		if (c < 32) {
			//b = _custom_chars[_font_size][(uint8_t)chr][i];
		} else if (size == PCD8544_FontSize_3x5) {
			b = PCD8544_Font3x5[c - 32][i];
		} else {
			b = PCD8544_Font5x7[c - 32][i];
		}
		if (b == 0x00 && (c != 0 && c != 32)) {
			continue;
		}
		for (j = 0; j < c_height; j++) {
			if (color == PCD8544_Pixel_Set) {
				PCD8544_DrawPixel(PCD8544_x, (PCD8544_y + j), ((b >> j) & 1) ? PCD8544_Pixel_Set : PCD8544_Pixel_Clear);
			} else {
				PCD8544_DrawPixel(PCD8544_x, (PCD8544_y + j), ((b >> j) & 1) ? PCD8544_Pixel_Clear : PCD8544_Pixel_Set);
			}
		}
		PCD8544_x++;
	}
	PCD8544_x++;
}

void PCD8544_Puts(char *c, PCD8544_Pixel_t color, PCD8544_FontSize_t size) {
	while (*c) {
		PCD8544_Putc(*c++, color, size);
	}
}

void PCD8544_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color) {
	short dx, dy;
	short temp;

	if (x0 > x1) {
		temp = x1;
		x1 = x0;
		x0 = temp;
	}
	if (y0 > y1) {
		temp = y1;
		y1 = y0;
		y0 = temp;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx == 0) {
		do {
			PCD8544_DrawPixel(x0, y0, color);
			y0++;
		} while (y1 >= y0);
		return;
	}
	if (dy == 0) {
		do {
			PCD8544_DrawPixel(x0, y0, color);
			x0++;
		} while (x1 >= x0);
		return;
	}

	/* Based on Bresenham's line algorithm  */
	if (dx > dy) {
		temp = 2 * dy - dx;
		while (x0 != x1) {
			PCD8544_DrawPixel(x0, y0, color);
			x0++;
			if (temp > 0) {
				y0++;
				temp += 2 * dy - 2 * dx;
			} else {
				temp += 2 * dy;
			}
		}
		PCD8544_DrawPixel(x0, y0, color);
	} else {
		temp = 2 * dx - dy;
		while (y0 != y1) {
			PCD8544_DrawPixel(x0, y0, color);
			y0++;
			if (temp > 0) {
				x0++;
				temp += 2 * dy - 2 * dx;
			} else {
				temp += 2 * dy;
			}
		}
		PCD8544_DrawPixel(x0, y0, color);
	}
}

void PCD8544_DrawRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color) {
	PCD8544_DrawLine(x0, y0, x1, y0, color); 	//Top
	PCD8544_DrawLine(x0, y0, x0, y1, color);	//Left
	PCD8544_DrawLine(x1, y0, x1, y1, color);	//Right
	PCD8544_DrawLine(x0, y1, x1, y1, color);	//Bottom
}

void PCD8544_DrawFilledRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color) {
	for (; y0 < y1; y0++) {
		PCD8544_DrawLine(x0, y0, x1, y0, color);
	}
}

void PCD8544_DrawCircle(char x0, char y0, char r, PCD8544_Pixel_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    PCD8544_DrawPixel(x0, y0 + r, color);
    PCD8544_DrawPixel(x0, y0 - r, color);
    PCD8544_DrawPixel(x0 + r, y0, color);
    PCD8544_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        PCD8544_DrawPixel(x0 + x, y0 + y, color);
        PCD8544_DrawPixel(x0 - x, y0 + y, color);
        PCD8544_DrawPixel(x0 + x, y0 - y, color);
        PCD8544_DrawPixel(x0 - x, y0 - y, color);

        PCD8544_DrawPixel(x0 + y, y0 + x, color);
        PCD8544_DrawPixel(x0 - y, y0 + x, color);
        PCD8544_DrawPixel(x0 + y, y0 - x, color);
        PCD8544_DrawPixel(x0 - y, y0 - x, color);
    }
}

void PCD8544_DrawFilledCircle(char x0, char y0, char r, PCD8544_Pixel_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    PCD8544_DrawPixel(x0, y0 + r, color);
    PCD8544_DrawPixel(x0, y0 - r, color);
    PCD8544_DrawPixel(x0 + r, y0, color);
    PCD8544_DrawPixel(x0 - r, y0, color);
    PCD8544_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        PCD8544_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        PCD8544_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        PCD8544_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        PCD8544_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}
