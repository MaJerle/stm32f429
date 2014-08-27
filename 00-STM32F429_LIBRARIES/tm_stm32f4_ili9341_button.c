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
#include "tm_stm32f4_ili9341_button.h"

TM_ILI9341_Button_t TM_ILI9341_Buttons[TM_ILI9341_BUTTON_MAX_BUTTONS];

int8_t TM_ILI9341_Button_Add(TM_ILI9341_Button_t* button) {
	uint8_t id = 0;
	while ((TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_USED) && (id < TM_ILI9341_BUTTON_MAX_BUTTONS)) {
		id++;
	}
	
	if (id == TM_ILI9341_BUTTON_MAX_BUTTONS) {
		//Max button reached
		return -1;
	}
	TM_ILI9341_Buttons[id].x = button->x;
	TM_ILI9341_Buttons[id].y = button->y;
	TM_ILI9341_Buttons[id].width = button->width;
	TM_ILI9341_Buttons[id].height = button->height;
	TM_ILI9341_Buttons[id].background = button->background;
	TM_ILI9341_Buttons[id].borderColor = button->borderColor;
	TM_ILI9341_Buttons[id].flags = button->flags | TM_BUTTON_FLAG_USED | TM_BUTTON_FLAG_ENABLED;
	TM_ILI9341_Buttons[id].label = button->label;
	TM_ILI9341_Buttons[id].color = button->color;
	TM_ILI9341_Buttons[id].font = button->font;
	TM_ILI9341_Buttons[id].image = button->image;
	
	return id;
}

void TM_ILI9341_Button_DrawAll(void) {
	uint8_t id = 0;
	for (id = 0; id < TM_ILI9341_BUTTON_MAX_BUTTONS; id++) {
		if ((TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_USED)) {
			//Button enabled, draw it to screen
			TM_ILI9341_Button_Draw(id);
		}
	}
}

ErrorStatus TM_ILI9341_Button_Draw(uint8_t id) {
	uint16_t fontWidth, fontHeight, x, y, i, j;
	if ((TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_USED) == 0) {
		//Button not enabled
		return ERROR;
	}
	//Draw label
	if (TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_IMAGE) {
		//Draw picture
		for (i = 0; i < TM_ILI9341_Buttons[id].width; i++) {
			for (j = 0; j < TM_ILI9341_Buttons[id].height; j++) {
				TM_ILI9341_DrawPixel(	TM_ILI9341_Buttons[id].x + i,
										TM_ILI9341_Buttons[id].y + j,
										*(uint16_t *)(TM_ILI9341_Buttons[id].image + j * TM_ILI9341_Buttons[id].width + i)
				);
			}
		}
	} else {
		//Background
		TM_ILI9341_DrawFilledRectangle(	TM_ILI9341_Buttons[id].x,
										TM_ILI9341_Buttons[id].y,
										TM_ILI9341_Buttons[id].x + TM_ILI9341_Buttons[id].width,
										TM_ILI9341_Buttons[id].y + TM_ILI9341_Buttons[id].height,
										TM_ILI9341_Buttons[id].background );
		
	}
	
	//Display label
	if ((TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_NOLABEL) == 0) {
		TM_ILI9341_GetStringSize(TM_ILI9341_Buttons[id].label, TM_ILI9341_Buttons[id].font, &fontWidth, &fontHeight);
		x = TM_ILI9341_Buttons[id].x + TM_ILI9341_Buttons[id].width / 2 - (fontWidth / 2);
		y = TM_ILI9341_Buttons[id].y + TM_ILI9341_Buttons[id].height / 2 - (fontHeight / 2);
		TM_ILI9341_Puts(x, y, TM_ILI9341_Buttons[id].label, TM_ILI9341_Buttons[id].font, TM_ILI9341_Buttons[id].color, TM_ILI9341_Buttons[id].background);
	}
	
	//Border
	if ((TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_NOBORDER) == 0) {
		//Border enabled
		TM_ILI9341_DrawRectangle(	TM_ILI9341_Buttons[id].x,
									TM_ILI9341_Buttons[id].y,
									TM_ILI9341_Buttons[id].x + TM_ILI9341_Buttons[id].width,
									TM_ILI9341_Buttons[id].y + TM_ILI9341_Buttons[id].height,
									TM_ILI9341_Buttons[id].borderColor );
	}
	
	return SUCCESS;
}

int8_t TM_ILI9341_Button_Touch(TM_STMPE811_TouchData* TouchData) {
	uint8_t id;
	for (id = 0; id < TM_ILI9341_BUTTON_MAX_BUTTONS; id++) {
		//If button not enabled, ignore it
		if ((TM_ILI9341_Buttons[id].flags & TM_BUTTON_FLAG_ENABLED) == 0) {
			continue;
		}
		//If touch data is inside button somewhere
		if (
			(TouchData->x > TM_ILI9341_Buttons[id].x && TouchData->x < (TM_ILI9341_Buttons[id].x + TM_ILI9341_Buttons[id].width)) &&
			(TouchData->y > TM_ILI9341_Buttons[id].y && TouchData->y < (TM_ILI9341_Buttons[id].y + TM_ILI9341_Buttons[id].height))
		) {
			//Return its id
			return id;
		}
	}
	
	//No one was pressed
	return -1;
}


void TM_ILI9341_Button_Enable(uint8_t id) {
	//Add enabled flag
	TM_ILI9341_Buttons[id].flags |= TM_BUTTON_FLAG_ENABLED;
}

void TM_ILI9341_Button_Disable(uint8_t id) {
	//Remove enabled flag
	TM_ILI9341_Buttons[id].flags &= ~TM_BUTTON_FLAG_ENABLED;
}

void TM_ILI9341_Button_DeleteAll(void) {
	uint8_t i;
	for (i = 0; i < TM_ILI9341_BUTTON_MAX_BUTTONS; i++) {
		TM_ILI9341_Button_Delete(i);
	}
}

void TM_ILI9341_Button_Delete(uint8_t id) {
	//Just remove USED flag from button
	TM_ILI9341_Buttons[id].flags &= ~TM_BUTTON_FLAG_USED;
}

