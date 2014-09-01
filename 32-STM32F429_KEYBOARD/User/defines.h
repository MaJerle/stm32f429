#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Place your custom library settings */

/* Settings below are default in library */
/* Uncomment them only, if you want to change them */

/* Debounce counter */
/* Set how many counts have to be set before valid key press */
//#define KEYPAD_DEBOUNCE 				5000

/* Hold debounce divider */
/* With this you can set higher refresh rate for key response, if you hold button for long time */
//#define KEYPAD_HOLD_DEBOUNCE_DIVIDER	1

/* When you press button, this library works like on PC. You press button, button is activated immideatelly */
/* but if you still hold it, there is some delay between next character, just like on PC, when you first press */
/* you have to wait some time. Minimum time is the same as DEFAULT refresh rate, but if you want to increase it */
/* Then increase this value, default is 6 */
//#define KEYPAD_FIRST_HOLD_MULTIPLIER	6


/* Columns */
/* Column 1 default */
//#define KEYPAD_COLUMN_1_RCC		RCC_AHB1Periph_GPIOC
//#define KEYPAD_COLUMN_1_PORT		GPIOC
//#define KEYPAD_COLUMN_1_PIN		GPIO_Pin_1
/* Column 2 default */
//#define KEYPAD_COLUMN_2_RCC		RCC_AHB1Periph_GPIOC
//#define KEYPAD_COLUMN_2_PORT		GPIOC
//#define KEYPAD_COLUMN_2_PIN		GPIO_Pin_2
/* Column 3 default */
//#define KEYPAD_COLUMN_3_RCC		RCC_AHB1Periph_GPIOC
//#define KEYPAD_COLUMN_3_PORT		GPIOC
//#define KEYPAD_COLUMN_3_PIN		GPIO_Pin_3
/* Column 4 default */
//#define KEYPAD_COLUMN_4_RCC		RCC_AHB1Periph_GPIOC
//#define KEYPAD_COLUMN_4_PORT		GPIOC
//#define KEYPAD_COLUMN_4_PIN		GPIO_Pin_5

/* Rows */
/* Row 1 default */
//#define KEYPAD_ROW_1_RCC			RCC_AHB1Periph_GPIOD
//#define KEYPAD_ROW_1_PORT			GPIOD
//#define KEYPAD_ROW_1_PIN			GPIO_Pin_0
/* Row 2 default */
//#define KEYPAD_ROW_2_RCC			RCC_AHB1Periph_GPIOD
//#define KEYPAD_ROW_2_PORT			GPIOD
//#define KEYPAD_ROW_2_PIN			GPIO_Pin_1
/* Row 3 default */
//#define KEYPAD_ROW_3_RCC			RCC_AHB1Periph_GPIOD
//#define KEYPAD_ROW_3_PORT			GPIOD
//#define KEYPAD_ROW_3_PIN			GPIO_Pin_2
/* Row 4 default */
//#define KEYPAD_ROW_4_RCC			RCC_AHB1Periph_GPIOD
//#define KEYPAD_ROW_4_PORT			GPIOD
//#define KEYPAD_ROW_4_PIN			GPIO_Pin_3

#endif

