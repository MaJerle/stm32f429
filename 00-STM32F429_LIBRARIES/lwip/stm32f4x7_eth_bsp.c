/**
  ******************************************************************************
  * @file    stm32f4x7_eth_bsp.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   STM32F4x7 Ethernet hardware configuration.
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

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netif.h"
#include "netconf.h"
#include "lwip/dhcp.h"

#include "tm_stm32f4_ethernet.h"
#include "defines.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ETH_InitTypeDef ETH_InitStructure;
__IO uint32_t  EthStatus = 0;
__IO uint8_t EthLinkStatus = 0;
extern struct netif gnetif;
#ifdef ETHERNET_USE_DHCP
extern __IO uint8_t DHCP_state;
#endif /* LWIP_DHCP */

/* Private function prototypes -----------------------------------------------*/
static void ETH_GPIO_Config(void);
static void ETH_MACDMA_Config(void);

/* Private defines */
/* Bit 2 from Basic Status Register in PHY */
#define GET_PHY_LINK_STATUS()		(ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, PHY_BSR) & 0x00000004)

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
uint8_t ETH_BSP_Config(void) {	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Configure the GPIO ports for ethernet pins */
	ETH_GPIO_Config();

	/* Configure the Ethernet MAC/DMA */
	ETH_MACDMA_Config();

	/* Get Ethernet link status*/
	if (GET_PHY_LINK_STATUS()) {
		/* Check if LINK is up */
		EthStatus |= ETH_LINK_FLAG;
	}
	
	/* Call user function for specific PHY settings */
	/* User can do stuff in this function */
	/* For example, on DP83848 you can change led behaviour */
	TM_ETHERNET_INT_CustomOptions(ETHERNET_PHY_ADDRESS);
	
	/* Return Eternet status */
	return EthStatus;
}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(void) {
	/* Enable ETHERNET clock  */
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACRXEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACTXEN;

	/* Reset ETHERNET on AHB Bus */
	ETH_DeInit();

	/* Software reset */
	ETH_SoftwareReset();

	/* Wait for software reset */
	while (ETH_GetSoftwareResetStatus() == SET);

	/* ETHERNET Configuration --------------------------------------------------*/
	/* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	ETH_StructInit(&ETH_InitStructure);

	/* Fill ETH_InitStructure parametrs */
	/*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

	/*------------------------   DMA   -----------------------------------*/  
	/* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
	the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
	if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

	/* Configure Ethernet */
	EthStatus = ETH_Init(&ETH_InitStructure, ETHERNET_PHY_ADDRESS);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void ETH_GPIO_Config(void) {
	/* Enable SYSCFG clock */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/* Configure MCO1 (PA8) */
	/* This pin must be initialized as MCO, but not needed to be used */
	/* It looks like a bug in STM32F4 */
	/* Init alternate function for PA8 = MCO */
	TM_GPIO_Init(GPIOA, GPIO_PIN_8, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
	
#ifdef ETHERNET_MCO_CLOCK
	/* Set PA8 output HSE value */
	RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
#endif

#ifdef ETHERNET_MII_MODE
	/* MII Media interface selection */
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
#else
	/* RMII Media interface selection */
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
#endif
	/* Check if user has defined it's own pins */
	if (!TM_ETHERNET_InitPinsCallback()) {
#ifdef ETHERNET_MII_MODE
		/* MII */
		/* GPIOA                     REF_CLK      MDIO         RX_DV */
		TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
		
		/* GPIOB                     PPS_PUT      TDX3 */
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_5 | GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
		
		/* GPIOC                     MDC          TDX2         TX_CLK       RXD0         RXD1 */
		TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
		
		/* GPIOG                     TX_EN         TXD0          TXD1 */
		TM_GPIO_InitAlternate(GPIOG, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
		
		/* GPIOH                     CRS          COL          RDX2         RXD3 */
		TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
		
		/* GPIOI                     RX_ER */
		TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
#else
		/* RMII */
		/* Init default pins */		
		/* Ethernet pins configuration */
		TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);

		/* RX pins and MDC */
		TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);

		/* Check TX pins */
#ifdef ETHERNET_RMII_PINSPACK_2
		/* Pinspack 2, TXD0, TXD1 and TX_EN pins are connected to GPIOG pins */
		TM_GPIO_InitAlternate(GPIOG, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
#else
		/* Pinspack 1, TXD0, TXD1 and TX_EN pins are connected to GPIOB pins */
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_ETH);
#endif /* RMII pinspack 2 */
#endif /* ETHERNET_USE_MII */
	}
}

/* This function is called periodically each second */
/* It checks link status for ethernet controller */
void ETH_CheckLinkStatus(uint16_t PHYAddress) {
	static uint8_t status = 0;
	uint32_t t = GET_PHY_LINK_STATUS();
	
	/* If we have link and previous check was not yet */
	if (t && !status) {
		/* Set link up */
		netif_set_link_up(&gnetif);
		
		status = 1;
	}	
	/* If we don't have link and it was on previous check */
	if (!t && status) {
		EthLinkStatus = 1;
		/* Set link down */
		netif_set_link_down(&gnetif);
			
		status = 0;
	}
}

/**
  * @brief  Link callback function, this function is called on change of link status.
  * @param  The network interface
  * @retval None
  */
void ETH_link_callback(struct netif *netif) {
	__IO uint32_t timeout = 0;
	uint32_t tmpreg;
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;

	if (netif_is_link_up(netif)) {
		/* Restart the auto-negotiation */
		if (ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable) {
			/* Reset Timeout counter */
			timeout = 0;

			/* Enable auto-negotiation */
			ETH_WritePHYRegister(ETHERNET_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

			/* Wait until the auto-negotiation will be completed */
			do {
				timeout++;
			} while (!(ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));  

			/* Reset Timeout counter */
			timeout = 0;

			/* Get speed and duplex mode from PHY */
			/* This is different for every PHY */
			ETH_EXTERN_GetSpeedAndDuplex(ETHERNET_PHY_ADDRESS, &ETH_InitStructure);

			/*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
			/* Get the ETHERNET MACCR value */  
			tmpreg = ETH->MACCR;

			/* Set the FES bit according to ETH_Speed value */ 
			/* Set the DM bit according to ETH_Mode value */ 
			tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

			/* Write to ETHERNET MACCR */
			ETH->MACCR = (uint32_t)tmpreg;

			_eth_delay_(ETH_REG_WRITE_DELAY);
			tmpreg = ETH->MACCR;
			ETH->MACCR = tmpreg;
		}

		/* Restart MAC interface */
		ETH_Start();

#ifdef ETHERNET_USE_DHCP
		ipaddr.addr = 0;
		netmask.addr = 0;
		gw.addr = 0;
		DHCP_state = DHCP_START;
#else
		IP4_ADDR(&ipaddr, TM_ETHERNET.ip_addr[0], TM_ETHERNET.ip_addr[1], TM_ETHERNET.ip_addr[2], TM_ETHERNET.ip_addr[3]);
		IP4_ADDR(&netmask, TM_ETHERNET.netmask[0], TM_ETHERNET.netmask[1], TM_ETHERNET.netmask[2], TM_ETHERNET.netmask[3]);
		IP4_ADDR(&gw, TM_ETHERNET.gateway[0], TM_ETHERNET.gateway[1], TM_ETHERNET.gateway[2], TM_ETHERNET.gateway[3]);
#endif /*ETHERNET_USE_DHCP */

		/* Set address */
		netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(&gnetif);    

		/* Call user function */
		TM_ETHERNET_INT_LinkIsUpCallback();

		EthLinkStatus = 0;
	} else {
		ETH_Stop();
#ifdef ETHERNET_USE_DHCP
		DHCP_state = DHCP_LINK_DOWN;
		dhcp_stop(netif);
#endif /* ETHERNET_USE_DHCP */

		/*  When the netif link is down this function must be called. */
		netif_set_down(&gnetif);

		/* Call user function */
		TM_ETHERNET_INT_LinkIsDownCallback();
	}
}

/* Function by TM */
void ETH_EXTERN_GetSpeedAndDuplex(uint32_t PHYAddress, ETH_InitTypeDef* ETH_InitStruct) {
	uint32_t RegValue;
	
/* DP83848 */
#if ETHERNET_PHY == 0
	/* Read the result of the auto-negotiation */
	RegValue = ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, PHY_SR);

	/* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
	if ((RegValue & PHY_DUPLEX_STATUS) != (uint16_t)RESET) {
		/* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
		ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;  
	} else {
		/* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
		ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
	}
	/* Configure the MAC with the speed fixed by the auto-negotiation process */
	if ((RegValue & PHY_SPEED_STATUS) != (uint16_t)RESET) {
		/* Set Ethernet speed to 10M following the auto-negotiation */
		ETH_InitStruct->ETH_Speed = ETH_Speed_10M; 
	} else {
		/* Set Ethernet speed to 100M following the auto-negotiation */
		ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
	}

/* DP83848 */
#elif ETHERNET_PHY == 1
/* LAN8720A */
	/* Read status register, register number 31 = 0x1F */
	RegValue = ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, 0x1F);
	/* Mask out bits which are not for speed and link indication, bits 4:2 are used */
	RegValue = (RegValue >> 2) & 0x07;

	/* Switch statement */
	switch (RegValue) {
		case 1: /* Base 10, half-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
			break;
		case 2: /* Base 100, half-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
			break;
		case 5: /* Base 10, full-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
			break;
		case 6: /* Base 100, full-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
			break;
		default:
			break;
	}
/* LAN8720A */
#elif ETHERNET_PHY == 2
/* KSZ8081RNA */
	/* Read status register, register number 0x1F */
	RegValue = ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, 0x1E);
	/* Mask out bits which are not for speed and link indication, bits 2:0 are used */
	RegValue = (RegValue) & 0x07;

	/* Switch */
	switch (RegValue) {
		case 1: /* Base 10, half-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
			break;
		case 2: /* Base 100, half-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
			break;
		case 5: /* Base 10, full-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
			ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
			break;
		case 6: /* Base 100, full-duplex */
			ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
			ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;
			break;
		default:
			break;
	}
/* KSZ8081RNA */
#else
	#error "Invalid PHY selected. Open tm_stm32f4_ethernet.h and read manual how to set your PHY from available PHYs!!"
#endif
	
	/* Set speed and duplex mode */
	TM_ETHERNET.speed_100m = 0;
	TM_ETHERNET.full_duplex = 0;
	if (ETH_InitStruct->ETH_Speed == ETH_Speed_100M) {
		TM_ETHERNET.speed_100m = 1;
	}
	if (ETH_InitStruct->ETH_Mode == ETH_Mode_FullDuplex) {
		TM_ETHERNET.full_duplex = 1;
	}
	  
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
