/**
 *	External interrupts library for STM32F4 devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-38-external-interrupts-for-stm32f4
 *	@version 	v1.2
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
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
 * 
 * Version 1.2
 *	- March 10, 2015
 *	- TM GPIO Library supported
 *
 * Version 1.1
 * 	- March 08, 2015
 *	- Added function TM_EXTI_Handler, which is called anytime EXTI interrupt occur,
 *	  no matter on which GPIO_Pin. GPIO_Pin_x is passed as argument where x is 0 to 15
 *
 * This library allows you to easy enable external interrupt on specific pin.
 * 
 * You only need to specify GPIOx, pin and interrupt trigger you want to use and you are ready to use.
 * There are predefined function names that you use to handle this interrupts.
 * 
 * There are 16 external interrupts possible.
 * From lines 0 to 15.
 * Each GPIO_Pin_x from all GPIOx is connected to one line. In other words, PC0, PA0, PB0, PJ0,...
 * are all connected to line 0, and so on. But you can use only one pin for one line at a time.
 * So only PA0 at one time, or PD0 or PC0, but one a time.
 * 
 * You can still use more lines at the same time. So let's say PA0 is line0 and PC13 is line13.
 * This 2 interrupts can be used simultaneouly without problems.
 * 
 * Next step is handling interrupts.
 * There are 16 interrupt lines, but only 7 interrupt handlers.
 * Lines0 to 4 have each own handler, then lines 5 to 9 have one common
 * and lines 10 to 15 have one common.
 * 
 * I write these functions in my library and use my own to handle specific line for you for easly.
 * Sometimes there is need for use default handler names in your own, but 2 functions with same name 
 * can not be used, so I made some settings.
 * 
 * If you open defines.h file and add any of lines below, you will disable handler line for interrupts.
 * By default, all handlers for lines are enabled. Disabled then with adding lines below in defines.h file:
 * 
 * 	//Disable EXTI0_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_0
 * 	//Disable EXTI1_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_1
 * 	//Disable EXTI2_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_2
 * 	//Disable EXTI3_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_3
 * 	//Disable EXTI4_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_4
 * 	//Disable EXTI9_5_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_9_5
 * 	//Disable EXTI15_10_IRQHandler function
 * 	#define TM_EXTI_DISABLE_DEFAULT_HANDLER_15_10
 * 
 * If you need higher priority for external interrupts in NVIC, add lines below in defines.h file and edit them
 * 	
 * 	//Set custom NVIC priority
 * 	#define TM_EXTI_PRIORITY	0x0A
 */
#ifndef TM_EXTI_H
#define TM_EXTI_H 120

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx EXTI
 * - STM32F4xx SYSCFG
 * - misc.h
 * - defines.h
 * - attributes.h
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "defines.h"
#include "attributes.h"
#include "tm_stm32f4_gpio.h"

/* Default priority for NVIC */
#ifndef TM_EXTI_PRIORITY
#define TM_EXTI_PRIORITY	0x0A
#endif

/**
 * Result enumeration
 * 
 * Parameters:
 * 	- TM_EXTI_Result_Ok:
 * 		Everything ok
 * 	- TM_EXTI_Result_Error:
 * 		An error has occured
 */
typedef enum {
	TM_EXTI_Result_Ok = 0,
	TM_EXTI_Result_Error
} TM_EXTI_Result_t;

/**
 * Interrupt trigger enumeration
 * 
 * Parameters:
 * 	- TM_EXTI_Trigger_Rising:
 * 		Trigger interrupt on rising edge on line
 * 	- TM_EXTI_Trigger_Falling:
 * 		Trigger interrupt on falling edge on line
 * 	- TM_EXTI_Trigger_Rising_Falling:
 * 		Trigger interrupt on any edge on line
 */
typedef enum {
	TM_EXTI_Trigger_Rising = 0x08,
	TM_EXTI_Trigger_Falling = 0x0C,
	TM_EXTI_Trigger_Rising_Falling = 0x10
} TM_EXTI_Trigger_t;

/**
 * Attach interrupt to specific pin.
 * This function automatically enables the clock for GPIO peripheral and also sets pull resistors
 * depending on trigger you use.
 * 	- Falling edge: pull up is enabled
 * 	- Rising edge: pull down is enabled
 * 	- Any edge: no pull activated
 * 
 * Also, you can attach only one GPIOx to specific GPIO_Pin.
 * In other words, GPIO_Pin_5 can not be attached to GPIOA and GPIOB at the same time.
 * 
 * Parameters:
 * 	- GPIO_TypeDef* GPIOx:
 * 		GPIOx to be used. Valid x is between A and K
 * 	- uint16_t GPIO_Line:
 * 		GPIO line in use. Valid GPIO is GPIO_PIN_0 to GPIO_PIN_15
 * 	- TM_EXTI_Trigger_t trigger:
 * 		Trigger for interrupt
 * 		TM_EXTI_Trigger_t member
 * 
 * Returns OK if attached OK or error if previous gio with same number was attached
 */
extern TM_EXTI_Result_t TM_EXTI_Attach(GPIO_TypeDef* GPIOx, uint16_t GPIO_Line, TM_EXTI_Trigger_t trigger);

/**
 * Detach gpio pin from interrupt lines
 * 
 * Parameters:
 * 	- uint16_t GPIO_Line:
 * 		GPIO line in use. Valid GPIO is GPIO_PIN_0 to GPIO_PIN_15
 * 
 * Returns OK if detached or error if already detached
 */
extern TM_EXTI_Result_t TM_EXTI_Detach(uint16_t GPIO_Line);

/**
 * Create software interrupt on specific GPIO_Line
 *
 * Parameters:
 * 	- uint16_t GPIO_Line:
 * 		GPIO line in use. Valid GPIO is GPIO_PIN_0 to GPIO_PIN_15
 *
 * Defined as macro for faster execution
 */
#define TM_EXTI_SoftwareInterrupt(GPIO_Line)	(EXTI->SWIER |= (GPIO_Line))

/* Handler functions */
/* __weak parameter is added to prevent link errors if function is not implemented */
__weak void TM_EXTI_Handler_0(void);	/* Handle lines 0 */
__weak void TM_EXTI_Handler_1(void);	/* Handle lines 1 */
__weak void TM_EXTI_Handler_2(void);	/* Handle lines 2 */
__weak void TM_EXTI_Handler_3(void);	/* Handle lines 3 */
__weak void TM_EXTI_Handler_4(void);	/* Handle lines 4 */
__weak void TM_EXTI_Handler_5(void);	/* Handle lines 5 */
__weak void TM_EXTI_Handler_6(void);	/* Handle lines 6 */
__weak void TM_EXTI_Handler_7(void);	/* Handle lines 7 */
__weak void TM_EXTI_Handler_8(void);	/* Handle lines 8 */
__weak void TM_EXTI_Handler_9(void);	/* Handle lines 9 */
__weak void TM_EXTI_Handler_10(void);	/* Handle lines 10 */
__weak void TM_EXTI_Handler_11(void);	/* Handle lines 11 */
__weak void TM_EXTI_Handler_12(void);	/* Handle lines 12 */
__weak void TM_EXTI_Handler_13(void);	/* Handle lines 13 */
__weak void TM_EXTI_Handler_14(void);	/* Handle lines 14 */
__weak void TM_EXTI_Handler_15(void);	/* Handle lines 15 */
/* Handler for all lines. This function is called anytime interrupt occured on any pin */
__weak void TM_EXTI_Handler(uint16_t GPIO_Pin);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
