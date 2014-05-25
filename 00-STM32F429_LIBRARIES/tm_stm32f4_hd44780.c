#include "tm_stm32f4_hd44780.h"

TM_HD44780_Options_t TM_HD44780_Opts;

__inline void TM_HD44780_Delay(uint32_t micros) {
	Delay(micros);
}

void TM_HD44780_Init(uint8_t cols, uint8_t rows) {
	TM_HD44780_InitPins();
	
	TM_HD44780_Opts.Rows = rows;
	TM_HD44780_Opts.Cols = cols;
	
	TM_HD44780_Opts.DisplayFunction = TM_HD44780_4BITMODE | TM_HD44780_5x8DOTS | TM_HD44780_1LINE;
	if (rows > 1) {
		TM_HD44780_Opts.DisplayFunction |= TM_HD44780_2LINE;
	}
	
	//At least 40ms
	TM_HD44780_Delay(45000);
	
	TM_HD44780_E_LOW;
	TM_HD44780_RS_LOW;
	
	//Try to set 4bit mode
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);
	
	//Second try
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(4500);
	
	//Third goo!
	TM_HD44780_Cmd4bit(0x03);
	TM_HD44780_Delay(150);
	
	//Set to 4-bit mode
	TM_HD44780_Cmd4bit(0x02);
	TM_HD44780_Delay(120);
	
	//set # lines, font size, etc.
	TM_HD44780_Cmd(TM_HD44780_FUNCTIONSET | TM_HD44780_Opts.DisplayFunction);
	
	//turn the display on with no cursor or blinking default
	TM_HD44780_Opts.DisplayControl = TM_HD44780_DISPLAYON | TM_HD44780_CURSOROFF | TM_HD44780_BLINKOFF;
	TM_HD44780_DisplayOn();
	
	//Clear lcd
	TM_HD44780_Clear();
	
	//Default font directions
	TM_HD44780_Opts.DisplayMode = TM_HD44780_ENTRYLEFT | TM_HD44780_ENTRYSHIFTDECREMENT;
	TM_HD44780_Cmd(TM_HD44780_ENTRYMODESET | TM_HD44780_Opts.DisplayMode);
	
	TM_HD44780_Delay(4500);
	
	TM_HD44780_Data('A');
	TM_HD44780_Data('B');
	TM_HD44780_Data('C');
}

void TM_HD44780_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitDef;
	RCC_AHB1PeriphClockCmd(	TM_HD44780_RS_RCC |
							TM_HD44780_E_RCC |
							TM_HD44780_D4_RCC |
							TM_HD44780_D5_RCC |
							TM_HD44780_D6_RCC |
							TM_HD44780_D7_RCC, ENABLE);
	//Common settings
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	//RS
	GPIO_InitDef.GPIO_Pin = TM_HD44780_RS_PIN;
	GPIO_Init(TM_HD44780_RS_PORT, &GPIO_InitDef);
	
	//E
	GPIO_InitDef.GPIO_Pin = TM_HD44780_E_PIN;
	GPIO_Init(TM_HD44780_E_PORT, &GPIO_InitDef);
	
	//D4
	GPIO_InitDef.GPIO_Pin = TM_HD44780_D4_PIN;
	GPIO_Init(TM_HD44780_D4_PORT, &GPIO_InitDef);
	
	//D5
	GPIO_InitDef.GPIO_Pin = TM_HD44780_D5_PIN;
	GPIO_Init(TM_HD44780_D5_PORT, &GPIO_InitDef);
	
	//D6
	GPIO_InitDef.GPIO_Pin = TM_HD44780_D6_PIN;
	GPIO_Init(TM_HD44780_D6_PORT, &GPIO_InitDef);
	
	//D7
	GPIO_InitDef.GPIO_Pin = TM_HD44780_D7_PIN;
	GPIO_Init(TM_HD44780_D7_PORT, &GPIO_InitDef);
}

void TM_HD44780_Write(TM_HD44780_WriteType_t cd, uint8_t val) {
	if (cd != TM_HD44780_WriteType_Data) {
		TM_HD44780_RS_LOW;
	} else {
		TM_HD44780_RS_HIGH;
	}
	
	if (cd == TM_HD44780_WriteType_Command_4bit) {

	} else {

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

void TM_HD44780_Clear(void) {
	TM_HD44780_Cmd(TM_HD44780_CLEARDISPLAY);
	TM_HD44780_Delay(2000);
}

void TM_HD44780_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	if (row >= TM_HD44780_Opts.Rows ) {
		row = TM_HD44780_Opts.Rows;    // we count rows starting w/0
	}
	
	TM_HD44780_Write(TM_HD44780_WriteType_Command, TM_HD44780_SETDDRAMADDR | (col + row_offsets[row]));
}

void TM_HD44780_Cmd(uint8_t cmd) {
	TM_HD44780_RS_LOW;
	
	//High nibble
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (cmd & 0x80) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (cmd & 0x40) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (cmd & 0x20) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (cmd & 0x10) ? Bit_SET : Bit_RESET);
	TM_HD44780_E_HIGH;
	TM_HD44780_Delay(10);
	TM_HD44780_E_LOW;
	TM_HD44780_Delay(10);
	
	//Low nibble
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (cmd & 0x08) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (cmd & 0x04) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (cmd & 0x02) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (cmd & 0x01) ? Bit_SET : Bit_RESET);
	TM_HD44780_E_HIGH;
	TM_HD44780_Delay(10);
	TM_HD44780_E_LOW;
	TM_HD44780_Delay(10);
}

void TM_HD44780_Cmd4bit(uint8_t cmd) {
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (cmd & 0x08) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (cmd & 0x04) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (cmd & 0x02) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (cmd & 0x01) ? Bit_SET : Bit_RESET);
	TM_HD44780_E_HIGH;
	TM_HD44780_Delay(10);
	TM_HD44780_E_LOW;
	TM_HD44780_Delay(10);
}

void TM_HD44780_Data(uint8_t data) {
	TM_HD44780_RS_HIGH;
	//High nibble
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (data & 0x80) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (data & 0x40) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (data & 0x20) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (data & 0x10) ? Bit_SET : Bit_RESET);
	TM_HD44780_E_HIGH;
	TM_HD44780_Delay(10);
	TM_HD44780_E_LOW;
	TM_HD44780_Delay(10);
	
	//Low nibble
	GPIO_WriteBit(TM_HD44780_D7_PORT, TM_HD44780_D7_PIN, (data & 0x08) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D6_PORT, TM_HD44780_D6_PIN, (data & 0x04) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D5_PORT, TM_HD44780_D5_PIN, (data & 0x02) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(TM_HD44780_D4_PORT, TM_HD44780_D4_PIN, (data & 0x01) ? Bit_SET : Bit_RESET);
	TM_HD44780_E_HIGH;
	TM_HD44780_Delay(10);
	TM_HD44780_E_LOW;
	TM_HD44780_Delay(10);
}

