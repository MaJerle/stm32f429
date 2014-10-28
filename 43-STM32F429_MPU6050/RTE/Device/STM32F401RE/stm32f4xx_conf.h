/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_conf.h  
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Library configuration file.
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
#ifndef __STM32F4xx_CONF_H
#define __STM32F4xx_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment the line below to enable peripheral header file inclusion */
#include "RTE_Components.h"

#ifdef RTE_DEVICE_STDPERIPH_ADC
  #include "stm32f4xx_adc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CRC
  #include "stm32f4xx_crc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DBGMCU
  #include "stm32f4xx_dbgmcu.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DMA
  #include "stm32f4xx_dma.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_EXTI
  #include "stm32f4xx_exti.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FLASH
  #include "stm32f4xx_flash.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_GPIO
  #include "stm32f4xx_gpio.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_I2C
  #include "stm32f4xx_i2c.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_IWDG
  #include "stm32f4xx_iwdg.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_PWR
  #include "stm32f4xx_pwr.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RCC
  #include "stm32f4xx_rcc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RTC
  #include "stm32f4xx_rtc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SDIO
  #include "stm32f4xx_sdio.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SPI
  #include "stm32f4xx_spi.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SYSCFG
  #include "stm32f4xx_syscfg.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_TIM
  #include "stm32f4xx_tim.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_USART
  #include "stm32f4xx_usart.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_WWDG
  #include "stm32f4xx_wwdg.h"
#endif
#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

#if defined (STM32F429_439xx)
#ifdef RTE_DEVICE_STDPERIPH_CRYP
  #include "stm32f4xx_cryp.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_HASH
  #include "stm32f4xx_hash.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RNG
  #include "stm32f4xx_rng.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CAN
  #include "stm32f4xx_can.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DAC
  #include "stm32f4xx_dac.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DCMI
  #include "stm32f4xx_dcmi.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DMA2D
  #include "stm32f4xx_dma2d.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FMC
  #include "stm32f4xx_fmc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_LTDC
  #include "stm32f4xx_ltdc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SAI
  #include "stm32f4xx_sai.h"
#endif
#endif /* STM32F429_439xx */

#if defined (STM32F427_437xx)
#ifdef RTE_DEVICE_STDPERIPH_CRYP
  #include "stm32f4xx_cryp.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_HASH
  #include "stm32f4xx_hash.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RNG
  #include "stm32f4xx_rng.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CAN
  #include "stm32f4xx_can.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DAC
  #include "stm32f4xx_dac.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DCMI
  #include "stm32f4xx_dcmi.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DMA2D
  #include "stm32f4xx_dma2d.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FMC
  #include "stm32f4xx_fmc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SAI
  #include "stm32f4xx_sai.h"
#endif
#endif /* STM32F427_437xx */

#if defined (STM32F40_41xxx)
#ifdef RTE_DEVICE_STDPERIPH_CRYP
  #include "stm32f4xx_cryp.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_HASH
  #include "stm32f4xx_hash.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RNG
  #include "stm32f4xx_rng.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CAN
  #include "stm32f4xx_can.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DAC
  #include "stm32f4xx_dac.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DCMI
  #include "stm32f4xx_dcmi.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FSMC
  #include "stm32f4xx_fsmc.h"
#endif

#endif /* STM32F40_41xxx */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* If an external clock source is used, then the value of the following define 
   should be set to the value of the external clock source, else, if no external 
   clock is used, keep this define commented */
/*#define I2S_EXTERNAL_CLOCK_VAL   12288000 */ /* Value of the external clock in Hz */


/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F4xx_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
