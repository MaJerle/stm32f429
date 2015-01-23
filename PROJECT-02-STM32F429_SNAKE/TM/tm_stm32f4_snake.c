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
#include "tm_stm32f4_snake.h"

TM_USB_HIDHOST_Keyboard_t Keyboard;
int8_t Random[2], Snake_Food[2];
int8_t Snake_Head[2], Snake_Head_Last[2], Snake_Foot[2];
volatile uint8_t Snake_FirstTime = 1;
volatile uint8_t GameOver = 0, GameOverDisplay = 0;
char Buffer[50];
volatile TM_SNAKE_t Snake, Snake1;
volatile TM_SNAKE_Settings_t Settings, Settings1;

void TM_SNAKE_DrawPixel(uint8_t x, uint8_t y, uint16_t value);
void TM_SNAKE_PrepareDisplay(void);
void TM_SNAKE_DrawArea(void);
void TM_SNAKE_SetDefaultSnake(void);
void TM_SNAKE_SetFirstOptions(void);

void TM_SNAKE_DeleteFromArray(uint16_t index, int8_t* twobytesarray);
void TM_SNAKE_AddToArray(int8_t* twobytesarray);
void TM_SNAKE_ReplaceArray(uint16_t index, int8_t* twobytesarray); 

void TM_SNAKE_SpeedUp(void);
void TM_SNAKE_SpeedDown(void);

void TM_SNAKE_Random(int8_t* twobytesarray);
void TM_SNAKE_GenerateTarget(void);

/* Check if given X and Y location matches coordinates where snake is */
uint8_t TM_SNAKE_MatchesSnakeLocations(int8_t* twobytesarray);

void TM_SNAKE_Start(void) {
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Turn off all leds */
	TM_DISCO_LedOff(LED_ALL);
	
	/* Initialize True random number generator */
	TM_RNG_Init();
	
	/* Initialize ILI9341 LCD */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize USB HID Host for keyboard */
	TM_USB_HIDHOST_Init();
	
	/* Set default options */
	TM_SNAKE_SetFirstOptions();
	
	/* Set default values */
	TM_SNAKE_SetDefaultSnake();
	
	/* Prepare display */
	TM_SNAKE_PrepareDisplay();
	
	/* Generate random target */
	TM_SNAKE_GenerateTarget();
	
	/* Set time to 0 */
	TM_DELAY_SetTime(0);
	while (1) {
		/* Process USB HID host */
		TM_USB_HIDHOST_Process();
		
		/* Check for timeout, move snake here */
		if (TM_DELAY_Time() >= Settings.Millis && !Settings.Pause && !GameOver) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			
			/* Get new direction value from temporary variable */
			Snake.Direction = Snake1.Direction;
			
			/* Get last x/y value from snake array = snake head */
			Snake_Head[0] = Snake.Snake[Snake.LastIndex][0];
			Snake_Head[1] = Snake.Snake[Snake.LastIndex][1];
			
			/* Store last value before update */
			Snake_Head_Last[0] = Snake_Head[0];
			Snake_Head_Last[1] = Snake_Head[1];
			
			if (!Snake_FirstTime) {
				/* Move snake */
				switch (Snake.Direction) {
					case SNAKE_DIRECTION_LEFT:
						Snake_Head[0] -= 1;
						break;
					case SNAKE_DIRECTION_RIGHT:
						Snake_Head[0] += 1;
						break;
					case SNAKE_DIRECTION_UP:
						Snake_Head[1] -= 1;
						break;
					case SNAKE_DIRECTION_DOWN:
						Snake_Head[1] += 1;
						break;
					default:
						break;
				}
			}
			
			/* Overflow mode is activated */
			if (Settings.Overflow) {
				/* Check X */
				if (Snake_Head[0] == -1) {
					Snake_Head[0] = SNAKE_PIXELS - 1;
				} else if (Snake_Head[0] == SNAKE_PIXELS) {
					Snake_Head[0] = 0;
				}
				/* Check Y */
				if (Snake_Head[1] == -1) {
					Snake_Head[1] = SNAKE_PIXELS - 1;
				} else if (Snake_Head[1] == SNAKE_PIXELS) {
					Snake_Head[1] = 0;
				}
			} else {
				/* Check walls */
				if (
					Snake_Head[0] == -1 ||
					Snake_Head[0] == SNAKE_PIXELS ||
					Snake_Head[1] == -1 ||
					Snake_Head[1] == SNAKE_PIXELS
				) {
					/* We hit the wall somewhere */
					GameOver = 1;
				}
			}
					
			if (!Snake_FirstTime) {
				/* Clear first value from array = snake foot */
				TM_SNAKE_DeleteFromArray(0, Snake_Foot);
				
				/* Check if snake hit itself */
				if (TM_SNAKE_MatchesSnakeLocations(Snake_Head)) {
					/* Set gameover flag */
					GameOver = 1;
				}
			}
			
			/* Check if target is reached */
			if (
				!GameOver &&
				Snake_Head[0] == Snake_Food[0] &&
				Snake_Head[1] == Snake_Food[1]
			) {
				/* Add new value to the array, increase snake */
				TM_SNAKE_AddToArray(Snake_Head);
				
				/* Increase counter for snake hits */
				Snake.Hits++;
				
				/* Generate new target */
				TM_SNAKE_GenerateTarget();
			}
			
			if (!GameOver) {
				if (!Snake_FirstTime) {
					/* Add new value to the array = new snake head */
					TM_SNAKE_AddToArray(Snake_Head);
				}
				
				/* Clear pixel on LCD for foot */
				/* First clear foot, maybe is new head on the same position */
				TM_SNAKE_DrawPixel(Snake_Foot[0], Snake_Foot[1], 0);
				
				/* Draw pixel on LCD for new head position with head color */
				TM_SNAKE_DrawPixel(Snake_Head[0], Snake_Head[1], 3);
				/* Draw new pixel for the second pixel after head with new color to delete head color */
				TM_SNAKE_DrawPixel(Snake_Head_Last[0], Snake_Head_Last[1], 1);
			}
			
			
			/* Clear flag if needed */
			if (Snake_FirstTime) {
				Snake_FirstTime = 0;
			}
		}
		
		/* Game over happen */
		if (GameOver) {
			/* Check flag */
			if (!GameOverDisplay) {
				/* Set flag */
				GameOverDisplay = 1;
				
				/* Show content to user */
				TM_ILI9341_Puts(88, 120, "Game\nOVER", &TM_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
				TM_ILI9341_Puts(28, 180, "Press 'r' to start again!!", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
			}
		} else {
			/* Clear flag */
			GameOverDisplay = 0;
		}
		
		/* Check if connected device is keyboard */
		if (TM_USB_HIDHOST_Device() == TM_USB_HIDHOST_Result_KeyboardConnected) {
			/* Green LED ON */
			TM_DISCO_LedOn(LED_GREEN);
			
			/* Read keyboard data */
			TM_USB_HIDHOST_ReadKeyboard(&Keyboard);
			
			/* If any buttons active */
			if (Keyboard.ButtonStatus == TM_USB_HIDHOST_Button_Pressed) {
				/* Check pressed button and do actions */
				switch ((uint8_t)Keyboard.Button) {
					case SNAKE_KEY_LEFT:
						/* Change direction if possible */
						if (
							Snake.Direction == SNAKE_DIRECTION_UP ||
							Snake.Direction == SNAKE_DIRECTION_DOWN ||
							Snake.Direction == SNAKE_DIRECTION_LEFT
						) {
							/* Disable pause mode */
							Settings.Pause = 0;
							/* Set temporary direction */
							Snake1.Direction = SNAKE_DIRECTION_LEFT;
						}
						break;
					case SNAKE_KEY_RIGHT:
						/* Change direction if possible */
						if (
							Snake.Direction == SNAKE_DIRECTION_UP ||
							Snake.Direction == SNAKE_DIRECTION_DOWN ||
							Snake.Direction == SNAKE_DIRECTION_RIGHT
						) {
							/* Disable pause mode */
							Settings.Pause = 0;
							/* Set temporary direction */
							Snake1.Direction = SNAKE_DIRECTION_RIGHT;
						}
						break;
					case SNAKE_KEY_UP:
						/* Change direction if possible */
						if (
							Snake.Direction == SNAKE_DIRECTION_LEFT ||
							Snake.Direction == SNAKE_DIRECTION_RIGHT ||
							Snake.Direction == SNAKE_DIRECTION_UP
						) {
							/* Disable pause mode */
							Settings.Pause = 0;
							/* Set temporary direction */
							Snake1.Direction = SNAKE_DIRECTION_UP;
						}
						break;
					case SNAKE_KEY_DOWN:
						/* Change direction if possible */
						if (
							Snake.Direction == SNAKE_DIRECTION_LEFT ||
							Snake.Direction == SNAKE_DIRECTION_RIGHT ||
							Snake.Direction == SNAKE_DIRECTION_DOWN
						) {
							/* Disable pause mode */
							Settings.Pause = 0;
							/* Set temporary direction */
							Snake1.Direction = SNAKE_DIRECTION_DOWN;
						}
						break;
					case SNAKE_KEY_SPEED_UP:
						/* Increase speed if possible */
						TM_SNAKE_SpeedUp();
						break;
					case SNAKE_KEY_SPEED_DOWN:
						/* Decrease speed if possible */
						TM_SNAKE_SpeedDown();
						break;
					case SNAKE_KEY_PAUSE:
						/* Toggle pause */
						if (Settings.Pause) {
							Settings.Pause = 0;
						} else {
							Settings.Pause = 1;
						}
						break;
					case SNAKE_KEY_RESET:
						/* Draw snake area */
						TM_SNAKE_DrawArea();
						/* Set default snake, leave as it was before */
						TM_SNAKE_SetDefaultSnake();
						/* Generate random target */
						TM_SNAKE_GenerateTarget();
						/* Disable gameover */
						GameOver = 0;
						/* Disable pause mode */
						Settings.Pause = 0;
						/* Reset first time flag */
						Snake_FirstTime = 1;
						break;
					case SNAKE_KEY_OVERFLOW:
						/* Toggle overflow mode */
						if (Settings.Overflow) {
							Settings.Overflow = 0;
						} else {
							Settings.Overflow = 1;
						}
						break;
					default:
						break;
				}
			}
		} else {
			/* Green LED OFF, keyboard not connected */
			TM_DISCO_LedOff(LED_GREEN);
		}
		
		/* Update LCD with changed settings */
		
		/* Check overflow */
		if (Settings1.Overflow != Settings.Overflow || Settings1.Speed != Settings.Speed) {
			/* Save new */
			Settings1.Overflow = Settings.Overflow;
			/* Save new */
			Settings1.Speed = Settings.Speed;
			
			/* Display game mode and speed */
			sprintf(Buffer, "Mode:%4d; Speed: %2d/%2d", Settings.Overflow, Settings.Speed, SNAKE_SPEED_MAX);
			TM_ILI9341_Puts(10, SNAKE_TEXT_LINE1, Buffer, &TM_Font_7x10, 0x0000, SNAKE_COLOR_MAIN_BCK);
		}
		
		/* Check snake hits for LCD display */
		if (Snake1.Hits != Snake.Hits) {
			/* Save new */
			Snake1.Hits = Snake.Hits;
			
			/* Display Speed */
			sprintf(Buffer, "Hits:%4d; Score: %5d", Snake.Hits, (2 - Settings.Overflow) * Snake.Hits * Settings.Speed);
			TM_ILI9341_Puts(10, SNAKE_TEXT_LINE2, Buffer, &TM_Font_7x10, 0x0000, SNAKE_COLOR_MAIN_BCK);
		}
	}
}

/* Draw snake pixel */
void TM_SNAKE_DrawPixel(uint8_t x, uint8_t y, uint16_t value) {
	uint16_t color;
	
	/* Get pixel color */
	if (value == 0) {
		color = SNAKE_COLOR_GAME_BCK;	/* Clear pixel */
	} else if (value == 1) {
		color = SNAKE_COLOR_PIXEL;		/* Set snake pixel */
	} else if (value == 2) {
		color = SNAKE_COLOR_TARGET;		/* Set target pixel */
	} else if (value == 3) {
		color = SNAKE_COLOR_HEAD;		/* Set head pixel for snake */
	} else {
		color = value;					/* Custom color */
	}
	
	/* Draw Filled rectangle */
	TM_ILI9341_DrawFilledRectangle(
		SNAKE_BACK_START_X + 2 + x * (SNAKE_PIXEL_SIZE + 1),
		SNAKE_BACK_START_Y + 2 + y * (SNAKE_PIXEL_SIZE + 1),
		SNAKE_BACK_START_X + 1 + x * (SNAKE_PIXEL_SIZE + 1) + SNAKE_PIXEL_SIZE,
		SNAKE_BACK_START_Y + 2 + y * (SNAKE_PIXEL_SIZE + 1) + SNAKE_PIXEL_SIZE,
		color
	);
}

void TM_SNAKE_PrepareDisplay(void) {
	/* Fill screen */
	TM_ILI9341_Fill(SNAKE_COLOR_MAIN_BCK);
	
	/* Draw snake area */
	TM_SNAKE_DrawArea();
	
	/* Show game title text */
	TM_ILI9341_Puts(40, 5, SNAKE_GAME_TITLE, &TM_Font_7x10, 0x0000, SNAKE_COLOR_MAIN_BCK);
}

void TM_SNAKE_SetFirstOptions(void) {
	Settings.Speed = SNAKE_SPEED_DEFAULT;
	Settings.Millis = 1000 / Settings.Speed;
	Settings.Pause = 0;
	Settings.Overflow = 1;
}

void TM_SNAKE_SetDefaultSnake(void) {
	uint16_t i;
	
	/* Set default options */
	Snake.Direction = Snake1.Direction = SNAKE_DIRECTION_RIGHT;
	Snake.LastIndex = 0;
	Snake.Length = 0;
	Snake.Hits = 0;
	
	/* Set default snake1 settings */
	Snake1.Hits = 255;
	
	/* Default X and Y values */
	/* Get random number */
	//TM_SNAKE_Random(Random);
	
	/* Start location */
	Random[0] = 3;
	Random[1] = 3;

	/* Add first to array, foot */
	TM_SNAKE_AddToArray(Random);
	
	/* Store values */
	for (i = 0; i < SNAKE_DEFAULT_LENGTH - 1; i++) {
		if (i == (SNAKE_DEFAULT_LENGTH - 2)) {
			Random[0]++;
		}
		TM_SNAKE_AddToArray(Random);
	}
}

void TM_SNAKE_DeleteFromArray(uint16_t index, int8_t* twobytesarray) {
	if (index < Snake.Length) {
		/* Store value that will be removed */
		twobytesarray[0] = Snake.Snake[index][0];
		twobytesarray[1] = Snake.Snake[index][1];
		
		/* Remove data from array at specific index */
		for (; index < Snake.LastIndex; index++) {
			Snake.Snake[index][0] = Snake.Snake[index + 1][0];
			Snake.Snake[index][1] = Snake.Snake[index + 1][1];
		}
		
		/* Last index-- */
		Snake.LastIndex--;
		Snake.Length--;
	}
}

void TM_SNAKE_AddToArray(int8_t* twobytesarray) {
	/* Add new position to the snake array */
	
	/* Increase counters */
	if (Snake.Length == 0) {
		Snake.Length++;
	} else {
		Snake.LastIndex++;
		Snake.Length++;
	}
	
	/* Add new values to array */
	Snake.Snake[Snake.LastIndex][0] = twobytesarray[0];
	Snake.Snake[Snake.LastIndex][1] = twobytesarray[1];
}

void TM_SNAKE_ReplaceArray(uint16_t index, int8_t* twobytesarray) {
	/* Replace array values */
	Snake.Snake[index][0] = twobytesarray[0];
	Snake.Snake[index][1] = twobytesarray[1];
}

void TM_SNAKE_SpeedUp(void) {
	/* Check for maximum speed */
	if (Settings.Speed < SNAKE_SPEED_MAX) {
		/* Increase speed */
		Settings.Speed++;
		/* Calculate new millis value */
		Settings.Millis = 1000 / Settings.Speed;
	}
}

void TM_SNAKE_SpeedDown(void) {
	/* Min speed is 1Hz */
	if (Settings.Speed > SNAKE_SPEED_MIN) {
		/* Decrease speed */
		Settings.Speed--;
		/* Calculate new millis value */
		Settings.Millis = 1000 / Settings.Speed;
	}
}

void TM_SNAKE_Random(int8_t* twobytesarray) {
	float temp;
	
	/* Generate first random number */
	temp = (float)TM_RNG_Get() / (float)0xFFFFFFFF * (SNAKE_PIXELS - 1);
	twobytesarray[0] = (int8_t)temp;
	
	/* Generate second random number */
	temp = (float)TM_RNG_Get() / (float)0xFFFFFFFF * (SNAKE_PIXELS - 1);
	twobytesarray[1] = (int8_t)temp;
}

uint8_t TM_SNAKE_MatchesSnakeLocations(int8_t* twobytesarray) {
	uint16_t i;
	for (i = 0; i < Snake.Length; i++) {
		if (
			Snake.Snake[i][0] == twobytesarray[0] &&
			Snake.Snake[i][1] == twobytesarray[1] 
		) {
			/* Snake hit itself */
			return 1;
		}
	}
	/* Snake did not hit itself */
	return 0;
}

void TM_SNAKE_GenerateTarget(void) {
	/* Generate 2 coordinates which are not on snake already */
	do {
		/* Generate random X and Y locations */
		TM_SNAKE_Random(Snake_Food);
		
		/* While we are on snake */
	} while (TM_SNAKE_MatchesSnakeLocations(Snake_Food));
	
	/* Display target on LCD */
	TM_SNAKE_DrawPixel(Snake_Food[0], Snake_Food[1], 2);
}

void TM_SNAKE_DrawArea(void) {
	/* Draw filled rectangle */
	TM_ILI9341_DrawFilledRectangle(
		SNAKE_BACK_START_X,
		SNAKE_BACK_START_Y,
		SNAKE_BACK_START_X + SNAKE_PIXELS * (SNAKE_PIXEL_SIZE + 1) + 2,
		SNAKE_BACK_START_Y + SNAKE_PIXELS * (SNAKE_PIXEL_SIZE + 1) + 2,
		SNAKE_COLOR_GAME_BCK);
	
	/* Draw border */
	TM_ILI9341_DrawRectangle(
		SNAKE_BACK_START_X,
		SNAKE_BACK_START_Y,
		SNAKE_BACK_START_X + SNAKE_PIXELS * (SNAKE_PIXEL_SIZE + 1) + 2,
		SNAKE_BACK_START_Y + SNAKE_PIXELS * (SNAKE_PIXEL_SIZE + 1) + 2,
		SNAKE_COLOR_BORDER
	);
}
