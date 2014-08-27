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
#include "tm_stm32f4_hd44780.h"

TM_HD44780_Options_t TM_HD44780_Opts;

void TM_HD44780_Init(uint8_t cols, uint8_t rows) {
	TM_DELAY_Init();
	
	TM_HD44780_InitPins();
	
	//At least 40ms
	TM_HD44780_Delay(45000);
	
	TM_HD44780_Opts.Rows = rows;
	TM_HD44780_Opts.Cols = cols;
	
	TM_HD44780_Opts.currentX = 0;
	TM_HD44780_Opts.currentY = 0;
	
	TM_HD44780_Opts.DisplayFunction = TM_HD44780_4BITMODE | TM_HD44780_5x8DOTS | TM_HD44780_1LINE;
	if (rows > 1) {
		TM_HD44780_Opts.DisplayFunction |= TM_HD44780_2LINE;
	}
	
	//Try to set 4bit mode
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);
	
	//Second try
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);
	
	//Third goo!
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);	
	
	//Set 4-bit interface
	TM_HD44780_Cmd4bit(0x02);
	TM_HD44780_Delay(100);
	
	//set # lines, font size, etc.
	TM_HD44780_Cmd(TM_HD44780_FUNCTIONSET | TM_HD44780_Opts.DisplayFunction);

	//turn the display on with no cursor or blinking default
	TM_HD44780_Opts.DisplayControl = TM_HD44780_DISPLAYON;
	TM_HD44780_DisplayOn();

	//Clear lcd
	TM_HD44780_Clear();

	//Default font directions
	TM_HD44780_Opts.DisplayMode = TM_HD44780_ENTRYLEFT | TM_HD44780_ENTRYSHIFTDECREMENT;
	TM_HD44780_Cmd(TM_HD44780_ENTRYMODESET | TM_HD44780_Opts.DisplayMode);

	TM_HD44780_Delay(4500);
}

void TM_HD44780_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(TM_HD44780_RS_RCC | TM_HD44780_E_RCC | TM_HD44780_D4_RCC | TM_HD44780_D5_RCC | TM_HD44780_D6_RCC | TM_HD44780_D7_RCC, ENABLE);
	
	//Common settings
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	//RS
	GPIO_InitStruct.GPIO_Pin = TM_HD44780_RS_PIN;
	GPIO_Init(TM_HD44780_RS_PORT, &GPIO_InitStruct);
	GPIO_WriteBit(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN, Bit_RESET);
	
	//E
	GPIO_InitStruct.GPIO_Pin = TM_HD44780_E_PIN;
	GPIO_Init(TM_HD44780_E_PORT, &GPIO_InitStruct);
	GPIO_WriteBit(TM_HD44780_E_PORT, TM_HD44780_E_PIN, Bit_RESET);
	
	//D4
	GPIO_InitStruct.GPIO_Pin = TM_HD44780_D4_PIN;
	GPIO_Init(TM_HD44780_D4_PORT, &GPIO_InitStruct);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, Bit_RESET);
	
	//D5
	GPIO_InitStruct.GPIO_Pin = TM_HD44780_D5_PIN;
	GPIO_Init(TM_HD44780_D5_PORT, &GPIO_InitStruct);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, Bit_RESET);
	
	//D6
	GPIO_InitStruct.GPIO_Pin = TM_HD44780_D6_PIN;
	GPIO_Init(TM_HD44780_D6_PORT, &GPIO_InitStruct);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, Bit_RESET);
	
	//D7
	GPIO_InitStruct.GPIO_Pin = TM_HD44780_D7_PIN;
	GPIO_Init(TM_HD44780_D7_PORT, &GPIO_InitStruct);
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, Bit_RESET);
}

void TM_HD44780_Clear(void) {
	TM_HD44780_Cmd(TM_HD44780_CLEARDISPLAY);
	TM_HD44780_Delay(3000);
}

void TM_HD44780_Cmd(uint8_t cmd) {
	TM_HD44780_RS_LOW;
	
	TM_HD44780_Cmd4bit(cmd >> 4);			//High nibble
	TM_HD44780_Cmd4bit(cmd & 0x0F);			//Low nibble
}

void TM_HD44780_Data(uint8_t data) {
	TM_HD44780_RS_HIGH;
	
	TM_HD44780_Cmd4bit(data >> 4);			//High nibble
	TM_HD44780_Cmd4bit(data & 0x0F);		//Low nibble
}

void TM_HD44780_Cmd4bit(uint8_t cmd) {
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (cmd & 0x08) != 0 ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (cmd & 0x04) != 0 ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (cmd & 0x02) != 0 ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (cmd & 0x01) != 0 ? Bit_SET : Bit_RESET);
	TM_HD44780_E_BLINK;
}

void TM_HD44780_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	//Go to the beginning
	if (row >= TM_HD44780_Opts.Rows) {
		row = 0;
	}
	
	TM_HD44780_Opts.currentX = col;
	TM_HD44780_Opts.currentY = row;
	
	TM_HD44780_Cmd(TM_HD44780_SETDDRAMADDR | (col + row_offsets[row]));
}

void TM_HD44780_Puts(uint8_t x, uint8_t y, char* str) {
	TM_HD44780_CursorSet(x, y);
	while (*str) {
		if (TM_HD44780_Opts.currentX >= TM_HD44780_Opts.Cols) {
			TM_HD44780_Opts.currentX = 0;
			TM_HD44780_Opts.currentY++;
			TM_HD44780_CursorSet(TM_HD44780_Opts.currentX, TM_HD44780_Opts.currentY);
		}
		if (*str == '\n') {
			TM_HD44780_Opts.currentY++;
			TM_HD44780_CursorSet(TM_HD44780_Opts.currentX, TM_HD44780_Opts.currentY);
		} else if (*str == '\r') {
			TM_HD44780_CursorSet(0, TM_HD44780_Opts.currentY);
		} else {
			TM_HD44780_Data(*str);
			TM_HD44780_Opts.currentX++;
		}
		str++;
	}
}

void TM_HD44780_DisplayOn(void) {
	TM_HD44780_Opts.DisplayControl |= TM_HD44780_DISPLAYON;
	TM_HD44780_Cmd(TM_HD44780_DISPLAYCONTROL | TM_HD44780_Opts.DisplayControl);
}

void TM_HD44780_DisplayOff(void) {
	TM_HD44780_Opts.DisplayControl &= ~TM_HD44780_DISPLAYON;
	TM_HD44780_Cmd(TM_HD44780_DISPLAYCONTROL | TM_HD44780_Opts.DisplayControl);
}

void TM_HD44780_BlinkOn(void) {
	TM_HD44780_Opts.DisplayControl |= TM_HD44780_BLINKON;
	TM_HD44780_Cmd(TM_HD44780_DISPLAYCONTROL | TM_HD44780_Opts.DisplayControl);
}

void TM_HD44780_BlinkOff(void) {
	TM_HD44780_Opts.DisplayControl &= ~TM_HD44780_BLINKON;
	TM_HD44780_Cmd(TM_HD44780_DISPLAYCONTROL | TM_HD44780_Opts.DisplayControl);
}

void TM_HD44780_CursorOn(void) {
	TM_HD44780_Opts.DisplayControl |= TM_HD44780_CURSORON;
	TM_HD44780_Cmd(TM_HD44780_DISPLAYCONTROL | TM_HD44780_Opts.DisplayControl);
}

void TM_HD44780_CursorOff(void) {
	TM_HD44780_Opts.DisplayControl &= ~TM_HD44780_CURSORON;
	TM_HD44780_Cmd(TM_HD44780_DISPLAYCONTROL | TM_HD44780_Opts.DisplayControl);
}

void TM_HD44780_ScrollLeft(void) {
	TM_HD44780_Cmd(TM_HD44780_CURSORSHIFT | TM_HD44780_DISPLAYMOVE | TM_HD44780_MOVELEFT);
}

void TM_HD44780_ScrollRight(void) {
	TM_HD44780_Cmd(TM_HD44780_CURSORSHIFT | TM_HD44780_DISPLAYMOVE | TM_HD44780_MOVERIGHT);
}

void TM_HD44780_CreateChar(uint8_t location, uint8_t *data) {
	uint8_t i;
	location &= 0x07;	//8 Free locations for custom chars
	TM_HD44780_Cmd(TM_HD44780_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++) {
		TM_HD44780_Data(data[i]);
	}
}

void TM_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location) {
	TM_HD44780_CursorSet(x, y);
	TM_HD44780_Data(location);
}

