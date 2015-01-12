/**
  ******************************************************************************
  * @file    global_includes.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   general includes for all the project files
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_INCLUDES_H__
#define __GLOBAL_INCLUDES_H__

/* Includes ------------------------------------------------------------------*/

#include "tm_stm32f4_ili9341_ltdc.h"

/* LCD frame buffer */
#define LCD_FRAME_BUFFER	ILI9341_FRAME_BUFFER


#include "GUI.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8)) 
/* Exported types ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __GLOBAL_INCLUDES_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

