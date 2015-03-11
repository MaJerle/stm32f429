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
#include "tm_stm32f4_keypad.h"

uint8_t KEYPAD_INT_Buttons[4][4] = {
	{0x01, 0x02, 0x03, 0x0C},
	{0x04, 0x05, 0x06, 0x0D},
	{0x07, 0x08, 0x09, 0x0E},
	{0x0A, 0x00, 0x0B, 0x0F},
};

uint32_t KEYPAD_INT_HoldDebounce[16] = {
	KEYPAD_HOLD_DEBOUNCE_0 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_1 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_2 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_3 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_4 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_5 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_6 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_7 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_8 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_9 / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_STAR / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_HASH / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_A / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_B / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_C / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
	KEYPAD_HOLD_DEBOUNCE_D / KEYPAD_HOLD_DEBOUNCE_DIVIDER,
};

uint32_t KEYPAD_INT_FirstDebounce[16] = {
	KEYPAD_HOLD_DEBOUNCE_0 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_1 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_2 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_3 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_4 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_5 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_6 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_7 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_8 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_9 * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_STAR * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_HASH * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_A * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_B * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_C * KEYPAD_FIRST_HOLD_MULTIPLIER,
	KEYPAD_HOLD_DEBOUNCE_D * KEYPAD_FIRST_HOLD_MULTIPLIER,
};

/* Private functions */
void TM_KEYPAD_INT_SetColumn(uint8_t column);
uint8_t TM_KEYPAD_INT_CheckRow(uint8_t column);
uint8_t TM_KEYPAD_INT_Read(void);

/* Private variables */
TM_KEYPAD_Type_t TM_KEYPAD_INT_KeypadType;

extern void TM_KEYPAD_Init(TM_KEYPAD_Type_t type) {
	/* Set keyboard type */
	TM_KEYPAD_INT_KeypadType = type;
	
	/* Columns are output */
	/* Column 1 */
	TM_GPIO_Init(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	/* Column 2 */
	TM_GPIO_Init(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	/* Column 3 */
	TM_GPIO_Init(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	/* Column 3 */
	if (TM_KEYPAD_INT_KeypadType == TM_KEYPAD_Type_Large) {
		TM_GPIO_Init(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	}
	
	/* Rows are inputs */
	/* Row 1 */
	TM_GPIO_Init(KEYPAD_ROW_1_PORT, KEYPAD_ROW_1_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	/* Row 2 */
	TM_GPIO_Init(KEYPAD_ROW_2_PORT, KEYPAD_ROW_2_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	/* Row 3 */
	TM_GPIO_Init(KEYPAD_ROW_3_PORT, KEYPAD_ROW_3_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	/* Row 4 */
	TM_GPIO_Init(KEYPAD_ROW_4_PORT, KEYPAD_ROW_4_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	
	/* All columns high */
	TM_KEYPAD_INT_SetColumn(0);
}

extern TM_KEYPAD_Button_t TM_KEYPAD_Read(void) {
	static uint8_t check, lastPressed = KEYPAD_NO_PRESSED;
	static uint32_t debounce, hold_debounce;
	static uint8_t firstHold = 1;
	
	/* Read keyboard */
	check = TM_KEYPAD_INT_Read();
	
	/* Check pressed */
	if (check == KEYPAD_NO_PRESSED) {					/* If no button pressed */
		debounce = 0;									/* Reset debounce check */
		hold_debounce = 0;								/* Reset debounce on hold */
		lastPressed = KEYPAD_NO_PRESSED;				/* Last button is not pressed */
		firstHold = 1;									/* Set flag for first hold time */
		
		return TM_KEYPAD_Button_NOPRESSED;				/* Keypad is not pressed */
	} else {
		/* Check previous state */
		if (lastPressed == check) {						/* If button now is same than last */			
			/* Button hold event */
			if (firstHold == 1) {						/* First hold waiting */				
				if (hold_debounce++ >= KEYPAD_INT_FirstDebounce[check]) {
					hold_debounce = 0;					/* Reset hold debounce */
					firstHold = 0;
					
					return (TM_KEYPAD_Button_t)check;	/* Return debounce */
				}
			} else {
				if (hold_debounce++ >= KEYPAD_INT_HoldDebounce[check]) {
					hold_debounce = 0;					/* Reset hold debounce */
					
					return (TM_KEYPAD_Button_t)check;	/* Return debounce */
				}
			}
			/* Reset */
			return TM_KEYPAD_Button_NOPRESSED;
		}
		/* New state */								/* Increase debounce counter */
		if (debounce++ >= KEYPAD_DEBOUNCE) {		/* Check debounce counter */
			debounce = 0;							/* Reset debounce */
			firstHold = 1;							/* Reset first hold state */
			lastPressed = check;					/* Save last valid state */
			
			/* Return check */
			return (TM_KEYPAD_Button_t)check;
		}
	}
	
	/* No pressed */
	return TM_KEYPAD_Button_NOPRESSED;
}

/* Private */
void TM_KEYPAD_INT_SetColumn(uint8_t column) {
	/* Set rows high */
	KEYPAD_COLUMN_1_HIGH;
	KEYPAD_COLUMN_2_HIGH;
	KEYPAD_COLUMN_3_HIGH;
	if (TM_KEYPAD_INT_KeypadType == TM_KEYPAD_Type_Large) {
		KEYPAD_COLUMN_4_HIGH;
	}
	
	if (column == 0) {
		/* row = 0 means all rows high */
		return;
	}
	switch (column) {
		case 1:
			KEYPAD_COLUMN_1_LOW;
			break;
		case 2:
			KEYPAD_COLUMN_2_LOW;
			break;
		case 3:
			KEYPAD_COLUMN_3_LOW;
			break;
		case 4:
			KEYPAD_COLUMN_4_LOW;
		default:
			break;
	}
}

uint8_t TM_KEYPAD_INT_CheckRow(uint8_t column) {
	/* Read rows */
	
	/* Scan row 1 */
	if (KEYPAD_ROW_1_CHECK) {
		return KEYPAD_INT_Buttons[0][column - 1];	
	}
	/* Scan row 2 */
	if (KEYPAD_ROW_2_CHECK) {
		return KEYPAD_INT_Buttons[1][column - 1];
	}
	/* Scan row 3 */
	if (KEYPAD_ROW_3_CHECK) {
		return KEYPAD_INT_Buttons[2][column - 1];
	}
	/* Scan row 4 */
	if (TM_KEYPAD_INT_KeypadType == TM_KEYPAD_Type_Large && KEYPAD_ROW_4_CHECK) {
		return KEYPAD_INT_Buttons[3][column - 1];
	}	
	/* Not pressed */
	return KEYPAD_NO_PRESSED;
}

uint8_t TM_KEYPAD_INT_Read(void) {
	uint8_t check;
	/* Set row 1 to LOW */
	TM_KEYPAD_INT_SetColumn(1);
	/* Check rows */
	check = TM_KEYPAD_INT_CheckRow(1);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}
	
	/* Set row 2 to LOW */
	TM_KEYPAD_INT_SetColumn(2);
	/* Check columns */
	check = TM_KEYPAD_INT_CheckRow(2);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}
	
	/* Set row 3 to LOW */
	TM_KEYPAD_INT_SetColumn(3);
	/* Check columns */
	check = TM_KEYPAD_INT_CheckRow(3);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}

	if (TM_KEYPAD_INT_KeypadType == TM_KEYPAD_Type_Large) {
		/* Set column 4 to LOW */
		TM_KEYPAD_INT_SetColumn(4);
		/* Check rows */
		check = TM_KEYPAD_INT_CheckRow(4);
		if (check != KEYPAD_NO_PRESSED) {
			return check;
		}
	}
	
	/* Not pressed */
	return KEYPAD_NO_PRESSED;
}

