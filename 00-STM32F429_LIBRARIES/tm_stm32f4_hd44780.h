/**
 *	HD44780 LCD driver library for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 */
#ifndef TM_HD44780_H
#define TM_HD44780_H 100

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"

//4 bit mode
//Control pins
//RS - Register select pin
#ifndef TM_HD44780_RS_PIN
#define TM_HD44780_RS_RCC		RCC_AHB1Periph_GPIOB
#define TM_HD44780_RS_PORT		GPIOB
#define TM_HD44780_RS_PIN		GPIO_Pin_2
#endif
//E - Enable pin
#ifndef TM_HD44780_E_PIN
#define TM_HD44780_E_RCC		RCC_AHB1Periph_GPIOB
#define TM_HD44780_E_PORT		GPIOB
#define TM_HD44780_E_PIN		GPIO_Pin_7
#endif
//Data pins
//D4 - Data 4 pin
#ifndef TM_HD44780_D4_PIN
#define TM_HD44780_D4_RCC		RCC_AHB1Periph_GPIOC
#define TM_HD44780_D4_PORT		GPIOC
#define TM_HD44780_D4_PIN		GPIO_Pin_12
#endif
//D5 - Data 5 pin
#ifndef TM_HD44780_D5_PIN
#define TM_HD44780_D5_RCC		RCC_AHB1Periph_GPIOC
#define TM_HD44780_D5_PORT		GPIOC
#define TM_HD44780_D5_PIN		GPIO_Pin_13
#endif
//D6 - Data 6 pin
#ifndef TM_HD44780_D6_PIN
#define TM_HD44780_D6_RCC		RCC_AHB1Periph_GPIOC
#define TM_HD44780_D6_PORT		GPIOC
#define TM_HD44780_D6_PIN		GPIO_Pin_14
#endif
//D7 - Data 7 pin
#ifndef TM_HD44780_D7_PIN
#define TM_HD44780_D7_RCC		RCC_AHB1Periph_GPIOC
#define TM_HD44780_D7_PORT		GPIOC
#define TM_HD44780_D7_PIN		GPIO_Pin_15
#endif

#define TM_HD44780_RS_LOW		GPIO_ResetBits(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN)
#define TM_HD44780_RS_HIGH		GPIO_SetBits(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN)
#define TM_HD44780_E_LOW		GPIO_ResetBits(TM_HD44780_E_PORT, TM_HD44780_E_PIN)
#define TM_HD44780_E_HIGH		GPIO_SetBits(TM_HD44780_E_PORT, TM_HD44780_E_PIN)


//Commands
#define TM_HD44780_CLEARDISPLAY			0x01
#define TM_HD44780_RETURNHOME			0x02
#define TM_HD44780_ENTRYMODESET			0x04
#define TM_HD44780_DISPLAYCONTROL		0x08
#define TM_HD44780_CURSORSHIFT			0x10
#define TM_HD44780_FUNCTIONSET			0x20
#define TM_HD44780_SETCGRAMADDR			0x40
#define TM_HD44780_SETDDRAMADDR			0x80

//Flags for display entry mode
#define TM_HD44780_ENTRYRIGHT			0x00
#define TM_HD44780_ENTRYLEFT			0x02
#define TM_HD44780_ENTRYSHIFTINCREMENT 	0x01
#define TM_HD44780_ENTRYSHIFTDECREMENT 	0x00

//Flags for display on/off control
#define TM_HD44780_DISPLAYON			0x04
#define TM_HD44780_DISPLAYOFF			0x00
#define TM_HD44780_CURSORON				0x02
#define TM_HD44780_CURSOROFF			0x00
#define TM_HD44780_BLINKON				0x01
#define TM_HD44780_BLINKOFF				0x00

//Flags for display/cursor shift
#define TM_HD44780_DISPLAYMOVE			0x08
#define TM_HD44780_CURSORMOVE			0x00
#define TM_HD44780_MOVERIGHT			0x04
#define TM_HD44780_MOVELEFT				0x00

//Flags for function set
#define TM_HD44780_8BITMODE				0x10
#define TM_HD44780_4BITMODE				0x00
#define TM_HD44780_2LINE				0x08
#define TM_HD44780_1LINE				0x00
#define TM_HD44780_5x10DOTS				0x04
#define TM_HD44780_5x8DOTS				0x00

typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
} TM_HD44780_Options_t;

typedef enum {
	TM_HD44780_WriteType_Data,
	TM_HD44780_WriteType_Command,
	TM_HD44780_WriteType_Command_4bit
} TM_HD44780_WriteType_t;


//Public
extern void TM_HD44780_Init(uint8_t cols, uint8_t rows);

extern void TM_HD44780_DisplayOn(void);

extern void TM_HD44780_DisplayOff(void);

extern void TM_HD44780_Clear(void);

extern void TM_HD44780_CursorSet(uint8_t col, uint8_t row);

extern void TM_HD44780_Cmd(uint8_t cmd);

extern void TM_HD44780_Cmd4bit(uint8_t cmd);

extern void TM_HD44780_Data(uint8_t data);

//Private
extern void TM_HD44780_Delay(uint32_t micros);

extern void TM_HD44780_InitPins(void);

extern void TM_HD44780_Write(TM_HD44780_WriteType_t dc, uint8_t val);

#endif



