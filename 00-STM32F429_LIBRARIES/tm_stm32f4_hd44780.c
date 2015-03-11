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

/* Private */
void TM_HD44780_InitPins(void);
void TM_HD44780_Cmd(uint8_t cmd);
void TM_HD44780_Cmd4bit(uint8_t cmd);
void TM_HD44780_Data(uint8_t data);
void TM_HD44780_CursorSet(uint8_t col, uint8_t row);

typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} TM_HD44780_Options_t;

TM_HD44780_Options_t TM_HD44780_Opts;

void TM_HD44780_Init(uint8_t cols, uint8_t rows) {
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Init pinout */
	TM_HD44780_InitPins();
	
	/* At least 40ms */
	TM_HD44780_Delay(45000);
	
	TM_HD44780_Opts.Rows = rows;
	TM_HD44780_Opts.Cols = cols;
	
	TM_HD44780_Opts.currentX = 0;
	TM_HD44780_Opts.currentY = 0;
	
	TM_HD44780_Opts.DisplayFunction = TM_HD44780_4BITMODE | TM_HD44780_5x8DOTS | TM_HD44780_1LINE;
	if (rows > 1) {
		TM_HD44780_Opts.DisplayFunction |= TM_HD44780_2LINE;
	}
	
	/* Try to set 4bit mode */
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);
	
	/* Second try */
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);
	
	/* Third goo! */
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);	
	
	/* Set 4-bit interface */
	TM_HD44780_Cmd4bit(0x02);
	TM_HD44780_Delay(100);
	
	/* Set # lines, font size, etc. */
	TM_HD44780_Cmd(TM_HD44780_FUNCTIONSET | TM_HD44780_Opts.DisplayFunction);

	/* Turn the display on with no cursor or blinking default */
	TM_HD44780_Opts.DisplayControl = TM_HD44780_DISPLAYON;
	TM_HD44780_DisplayOn();

	/* Clear lcd */
	TM_HD44780_Clear();

	/* Default font directions */
	TM_HD44780_Opts.DisplayMode = TM_HD44780_ENTRYLEFT | TM_HD44780_ENTRYSHIFTDECREMENT;
	TM_HD44780_Cmd(TM_HD44780_ENTRYMODESET | TM_HD44780_Opts.DisplayMode);

	/* Delay */
	TM_HD44780_Delay(4500);
}

void TM_HD44780_InitPins(void) {
	/* Init all pins */
	TM_GPIO_Init(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	TM_GPIO_Init(TM_HD44780_E_PORT, TM_HD44780_E_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	TM_GPIO_Init(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	TM_GPIO_Init(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	TM_GPIO_Init(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	TM_GPIO_Init(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	
	/* Set pins low */
	TM_GPIO_SetPinLow(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN);
	TM_GPIO_SetPinLow(TM_HD44780_E_PORT, TM_HD44780_E_PIN);
	TM_GPIO_SetPinLow(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN);
	TM_GPIO_SetPinLow(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN);
	TM_GPIO_SetPinLow(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN);
	TM_GPIO_SetPinLow(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN);
}

void TM_HD44780_Clear(void) {
	TM_HD44780_Cmd(TM_HD44780_CLEARDISPLAY);
	TM_HD44780_Delay(3000);
}

void TM_HD44780_Cmd(uint8_t cmd) {
	/* Command mode */
	TM_HD44780_RS_LOW;
	
	/* High nibble */
	TM_HD44780_Cmd4bit(cmd >> 4);
	/* Low nibble */
	TM_HD44780_Cmd4bit(cmd & 0x0F);
}

void TM_HD44780_Data(uint8_t data) {
	/* Data mode */
	TM_HD44780_RS_HIGH;
	
	/* High nibble */
	TM_HD44780_Cmd4bit(data >> 4);
	/* Low nibble */
	TM_HD44780_Cmd4bit(data & 0x0F);
}

void TM_HD44780_Cmd4bit(uint8_t cmd) {
	/* Set output port */
	TM_GPIO_SetPinValue(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (cmd & 0x08));
	TM_GPIO_SetPinValue(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (cmd & 0x04));
	TM_GPIO_SetPinValue(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (cmd & 0x02));
	TM_GPIO_SetPinValue(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (cmd & 0x01));
	TM_HD44780_E_BLINK;
}

void TM_HD44780_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	/* Go to the beginning */
	if (row >= TM_HD44780_Opts.Rows) {
		row = 0;
	}
	
	/* Save colums and row */
	TM_HD44780_Opts.currentX = col;
	TM_HD44780_Opts.currentY = row;
	
	/* Set location address */
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
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	TM_HD44780_Cmd(TM_HD44780_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++) {
		TM_HD44780_Data(data[i]);
	}
}

void TM_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location) {
	TM_HD44780_CursorSet(x, y);
	TM_HD44780_Data(location);
}

