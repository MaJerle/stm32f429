/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Webserver SSI and CGI handlers
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
//--------------------------------------------------------------
// CGI-Funktionen
// USE_CGI_GPIO = um GPIO-Pins per HTML-Seite zu schalten
// USE_CGI_ADC  = um einen ADC auzulesen
//--------------------------------------------------------------
#define  USE_CGI_GPIO
#define  USE_CGI_ADC

//--------------------------------------------------------------
// Dieses File wertet CGI-Kommandos vom Browser auf
//
// Falls "USE_CGI_GPIO" = enable
//    werden 4 Pins aus GPIO-Ausgang definiert (PD12, PD13, PD14, PD15)
//    diese Ausgänge können per CGI-Kommando gesetzt werden
//
// Falls "USE_CGI_ADC" = enable
//    wird 1 Pin aus ADC-Eingang definiert (PA3)
//    dieser Eingang wird zyklisch gewandelt
//    und kann per CGI-Kommando ausgelesen werden
//    (CooCox-Lib : ADC, wird benötigt)
//--------------------------------------------------------------


/* Includes ------------------------------------------------------------------*/
#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "tm_stm32f4_ethernet.h"

#include <string.h>
#include <stdlib.h>

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
