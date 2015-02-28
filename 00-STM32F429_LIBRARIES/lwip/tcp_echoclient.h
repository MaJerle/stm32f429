/**
  ******************************************************************************
  * @file    tcp_echoclient.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   Header file for tcp_echoclient.c
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
#ifndef __TCP_ECHOCLIENT_H__
#define __TCP_ECHOCLIENT_H__

#include "stm32f4xx.h"
#include "err.h"
#include "tm_stm32f4_ethernet.h"


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
err_t tcp_echoclient_connect(char* conn_name, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t port, void* user_parameters);

void tcp_echoclient_link_is_up_callback(void);
void tcp_echoclient_link_is_down_callback(void);

#endif /* __TCP_ECHOCLIENT_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
