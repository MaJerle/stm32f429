/**
 ******************************************************************************
 * @file    Audio_playback_and_record/src/usb_bsp.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    28-October-2011
 * @brief   This file implements the board support package for the USB host library
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include "defines.h"

/* Default NVIC settings */
#ifndef USB_MSC_HOST_NVIC_PRIORITY
#define USB_MSC_HOST_NVIC_PRIORITY			0x01
#endif

#ifndef USB_MSC_HOST_NVIC_SUBPRIORITY
#define USB_MSC_HOST_NVIC_SUBPRIORITY		0x01
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern USB_OTG_CORE_HANDLE          USB_OTG_MSC_Core;
extern USBH_HOST                    USB_Host;

/**
 * @brief  BSP_Init
 *         board user initializations
 * @param  None
 * @retval None
 */
void BSP_Init(void) {

}

/**
 * @brief  USB_OTG_BSP_Init
 *         Initilizes BSP configurations
 * @param  None
 * @retval None
 */
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
	/* Note: On STM32F4-Discovery board only USB OTG FS core is supported. */

	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_USB_OTG_FS

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure SOF VBUS ID DM DP Pins */
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_11 | 	// Data -
									GPIO_Pin_12;	// Data +

	#ifndef USB_MSC_HOST_DISABLE_VBUS
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_9;		// VBUS
	#endif
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	#ifndef USB_MSC_HOST_DISABLE_VBUS
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_OTG1_FS);
	#endif
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11, GPIO_AF_OTG1_FS);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12, GPIO_AF_OTG1_FS);

	/* this for ID line debug */
	#ifndef USB_MSC_HOST_DISABLE_ID
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10, GPIO_AF_OTG1_FS) ;
	#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE) ;
	
#else //USE_USB_OTG_HS

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

	/* Configure SOF VBUS ID DM DP Pins */  
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_14 |	// Data -
									GPIO_Pin_15;	// Data +

	#ifndef USB_MSC_HOST_DISABLE_ID
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_12;
	#endif

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	#ifndef USB_MSC_HOST_DISABLE_ID
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12, GPIO_AF_OTG2_FS);
	#endif
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14, GPIO_AF_OTG2_FS); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15, GPIO_AF_OTG2_FS);   
	
	/* VBUS */
	#ifndef USB_MSC_HOST_DISABLE_VBUS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_OTG_HS, ENABLE);  

#endif //USB_OTG_HS
}

void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable USB Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
#ifdef USE_USB_OTG_FS
	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
#else 
	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
#endif
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USB_MSC_HOST_NVIC_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USB_MSC_HOST_NVIC_SUBPRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state) {
#ifndef USB_MSC_HOST_DISABLE_VBUS
	if (0 == state) {
		/* DISABLE is needed on output of the Power Switch */
		GPIO_SetBits(HOST_POWERSW_PORT, HOST_POWERSW_VBUS);
	} else {
		/*ENABLE the Power Switch by driving the Enable LOW */
		GPIO_ResetBits(HOST_POWERSW_PORT, HOST_POWERSW_VBUS);
	}
#endif
}

/**
 * @brief  USB_OTG_BSP_ConfigVBUS
 *         Configures the IO for the Vbus and OverCurrent
 * @param  None
 * @retval None
 */
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev) {
#ifndef USB_MSC_HOST_DISABLE_VBUS
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HOST_POWERSW_PORT_RCC, ENABLE);

	/* Configure Power Switch Vbus Pin */
	GPIO_InitStructure.GPIO_Pin = HOST_POWERSW_VBUS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(HOST_POWERSW_PORT, &GPIO_InitStructure);

	/* By Default, DISABLE is needed on output of the Power Switch */
	GPIO_SetBits(HOST_POWERSW_PORT, HOST_POWERSW_VBUS);

	USB_OTG_BSP_mDelay(200);   /* Delay is need for stabilising the Vbus Low
      in Reset Condition, when Vbus=1 and Reset-button is pressed by user */
#endif
}

void USB_OTG_BSP_uDelay (const uint32_t usec) {
	__IO uint32_t count = 0;
	const uint32_t utime = (120 * usec / 7);
	do
	{
		if (++count > utime) {
			return ;
		}
	} while (1);
}

void USB_OTG_BSP_mDelay (const uint32_t msec) {
	USB_OTG_BSP_uDelay(msec * 1000);
}

#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void) {
#else
void OTG_HS_IRQHandler(void) {	
#endif
	USBH_OTG_ISR_Handler(&USB_OTG_MSC_Core);
}

