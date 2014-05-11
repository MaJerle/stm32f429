#include "tm_stm32f4_hd44780.h"

TM_LCD_Options_t TM_LCD_Opts;

__inline void TM_LCD_Delay(uint32_t micros) {
	Delay(micros);
}

void TM_LCD_Init(uint8_t cols, uint8_t rows) {
	TM_LCD_InitPins();
	//At least 40ms
	TM_LCD_Delay(45000);
	
	TM_LCD_Opts.Rows = rows;
	TM_LCD_Opts.Cols = cols;
	
	TM_LCD_Opts.DisplayFunction = TM_LCD_4BITMODE | TM_LCD_5x8DOTS | TM_LCD_1LINE;
	if (rows > 1) {
		TM_LCD_Opts.DisplayFunction |= TM_LCD_2LINE;
	}
	TM_LCD_E_LOW;
	
	TM_LCD_Write(TM_LCD_WriteType_Command_4bit, 0x03);
	TM_LCD_Delay(4500);
	
	TM_LCD_Write(TM_LCD_WriteType_Command_4bit, 0x03);
	TM_LCD_Delay(120);
	
	TM_LCD_Write(TM_LCD_WriteType_Command_4bit, 0x03);
	TM_LCD_Delay(40);
	
	TM_LCD_Write(TM_LCD_WriteType_Command_4bit, 0x02);
	TM_LCD_Delay(120);
	
	TM_LCD_Write(TM_LCD_WriteType_Command, TM_LCD_FUNCTIONSET | TM_LCD_Opts.DisplayFunction);
	
	TM_LCD_Opts.DisplayControl = TM_LCD_DISPLAYON | TM_LCD_CURSOROFF | TM_LCD_BLINKOFF;
	TM_LCD_Write(TM_LCD_WriteType_Command, TM_LCD_DISPLAYCONTROL | TM_LCD_Opts.DisplayControl);
	
	
	TM_LCD_Write(TM_LCD_WriteType_Data, 'a');
}

void TM_LCD_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitDef;
	RCC_AHB1PeriphClockCmd(	TM_LCD_RS_RCC |
							TM_LCD_RW_RCC |
							TM_LCD_E_RCC |
							TM_LCD_D4_RCC |
							TM_LCD_D5_RCC |
							TM_LCD_D6_RCC |
							TM_LCD_D7_RCC , ENABLE);
	//Common settings
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	//RS
	GPIO_InitDef.GPIO_Pin = TM_LCD_RS_PIN;
	GPIO_Init(TM_LCD_RS_PORT, &GPIO_InitDef);
	
	//RW
	GPIO_InitDef.GPIO_Pin = TM_LCD_RW_PIN;
	GPIO_Init(TM_LCD_RW_PORT, &GPIO_InitDef);
	
	//E
	GPIO_InitDef.GPIO_Pin = TM_LCD_E_PIN;
	GPIO_Init(TM_LCD_E_PORT, &GPIO_InitDef);
	
	//D4
	GPIO_InitDef.GPIO_Pin = TM_LCD_D4_PIN;
	GPIO_Init(TM_LCD_D4_PORT, &GPIO_InitDef);
	
	//D5
	GPIO_InitDef.GPIO_Pin = TM_LCD_D5_PIN;
	GPIO_Init(TM_LCD_D5_PORT, &GPIO_InitDef);
	
	//D6
	GPIO_InitDef.GPIO_Pin = TM_LCD_D6_PIN;
	GPIO_Init(TM_LCD_D6_PORT, &GPIO_InitDef);
	
	//D7
	GPIO_InitDef.GPIO_Pin = TM_LCD_D7_PIN;
	GPIO_Init(TM_LCD_D7_PORT, &GPIO_InitDef);
}

void TM_LCD_Write(TM_LCD_WriteType_t cd, uint8_t val) {
	if (cd != TM_LCD_WriteType_Data) {
		TM_LCD_RS_LOW;
	} else {
		TM_LCD_RS_HIGH;
	}
	TM_LCD_RW_LOW;
	
	if (cd == TM_LCD_WriteType_Command_4bit) {
		TM_LCD_E_HIGH;
		GPIO_WriteBit(TM_LCD_D7_PORT, TM_LCD_D7_PIN, (val & 0x08) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D6_PORT, TM_LCD_D7_PIN, (val & 0x04) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D5_PORT, TM_LCD_D7_PIN, (val & 0x02) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D4_PORT, TM_LCD_D7_PIN, (val & 0x01) ? Bit_SET : Bit_RESET);
		TM_LCD_Delay(1);
		TM_LCD_E_LOW;
	} else {
		//High nibble
		TM_LCD_E_HIGH;
		GPIO_WriteBit(TM_LCD_D7_PORT, TM_LCD_D7_PIN, (val & 0x80) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D6_PORT, TM_LCD_D7_PIN, (val & 0x40) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D5_PORT, TM_LCD_D7_PIN, (val & 0x20) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D4_PORT, TM_LCD_D7_PIN, (val & 0x10) ? Bit_SET : Bit_RESET);
		TM_LCD_Delay(1);
		TM_LCD_E_LOW;
		
		//Low nibble
		TM_LCD_E_HIGH;
		GPIO_WriteBit(TM_LCD_D7_PORT, TM_LCD_D7_PIN, (val & 0x08) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D6_PORT, TM_LCD_D7_PIN, (val & 0x04) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D5_PORT, TM_LCD_D7_PIN, (val & 0x02) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(TM_LCD_D4_PORT, TM_LCD_D7_PIN, (val & 0x01) ? Bit_SET : Bit_RESET);
		TM_LCD_Delay(1);
		TM_LCD_E_LOW;
	}
}


