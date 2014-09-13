/**
 *	nRF24L01+ library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/06/library-17-nrf24l01-stm32f4xx/
 *	@version 	v1.0
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
 * This library allows you to work with nRF24L01+ modules.
 * 
 * You can send and receive data from nRF24L01+ modules.
 * 
 * Default pinout
 * 	
 * 	NRF24L01+	STM32F4xx	DESCRIPTION
 * 	
 * 	GND			GND			Ground
 * 	VCC			3.3V		3.3V
 * 	CE			PD8			RF activated pin
 * 	CSN			PD7			SPI3 Pinspack 2 CSN pin
 * 	SCK			PC10		SPI3 Pinspack 2 SCK pin
 * 	MOSI		PC12		SPI3 Pinspack 2 MOSI pin
 * 	MISO		PC11		SPI3 Pinspack 2 MISO pin
 * 	IRQ			Not used	Interrupt pin. Goes low when active
 * 	
 * IRQ pin is not used in this library, but it's functionality is enabled by this software.
 * You can still set any pin on F4xx to be an external interrupt and handle
 * interrupts from nRF24L01+ module.
 * 
 * Add lines below in your defines.h file if you want to change default pinout:
 * 
 * 	//Change SPI used. Refer to TM SPI library to check which pins are for SPI
 *	#define NRF24L01_SPI				SPI3
 *	#define NRF24L01_SPI_PINS			TM_SPI_PinsPack_2
 *	
 *	//Change CSN pin. This is for SPI communication
 *	#define NRF24L01_CSN_RCC			RCC_AHB1Periph_GPIOD
 *	#define NRF24L01_CSN_PORT			GPIOD
 *	#define NRF24L01_CSN_PIN			GPIO_Pin_7
 *	
 *	//Change CE pin. This pin is used to enable/disable transmitter/receiver functionality
 *	#define NRF24L01_CE_RCC				RCC_AHB1Periph_GPIOD
 *	#define NRF24L01_CE_PORT			GPIOD
 *	#define NRF24L01_CE_PIN				GPIO_Pin_8
 * 	
 */
#ifndef TM_NRF24L01_H
#define TM_NRF24L01_H	100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx SPI
 * - TM_SPI
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_spi.h"
#include "defines.h"

//Set default SPI used
#ifndef NRF24L01_SPI
#define NRF24L01_SPI				SPI3
#define NRF24L01_SPI_PINS			TM_SPI_PinsPack_2
#endif
//SPI chip enable pin
#ifndef NRF24L01_CSN_PIN
#define NRF24L01_CSN_RCC			RCC_AHB1Periph_GPIOD
#define NRF24L01_CSN_PORT			GPIOD
#define NRF24L01_CSN_PIN			GPIO_Pin_7
#endif
//Chip enable for transmitting
#ifndef NRF24L01_CE_PIN
#define NRF24L01_CE_RCC				RCC_AHB1Periph_GPIOD
#define NRF24L01_CE_PORT			GPIOD
#define NRF24L01_CE_PIN				GPIO_Pin_8
#endif
//Pins configuration
#define NRF24L01_CE_LOW				NRF24L01_CE_PORT->BSRRH = NRF24L01_CE_PIN
#define NRF24L01_CE_HIGH			NRF24L01_CE_PORT->BSRRL = NRF24L01_CE_PIN
#define NRF24L01_CSN_LOW			NRF24L01_CSN_PORT->BSRRH = NRF24L01_CSN_PIN
#define NRF24L01_CSN_HIGH			NRF24L01_CSN_PORT->BSRRL = NRF24L01_CSN_PIN

//NRF24L01+ registers
#define NRF24L01_REG_CONFIG			0x00	//Configuration Register
#define NRF24L01_REG_EN_AA			0x01	//Enable ‘Auto Acknowledgment’ Function
#define NRF24L01_REG_EN_RXADDR		0x02	//Enabled RX Addresses
#define NRF24L01_REG_SETUP_AW		0x03	//Setup of Address Widths (common for all data pipes)
#define NRF24L01_REG_SETUP_RETR		0x04	//Setup of Automatic Retransmission
#define NRF24L01_REG_RF_CH			0x05	//RF Channel
#define NRF24L01_REG_RF_SETUP		0x06	//RF Setup Register	
#define NRF24L01_REG_STATUS			0x07	//Status Register
#define NRF24L01_REG_OBSERVE_TX		0x08	//Transmit observe register
#define NRF24L01_REG_RPD			0x09	
#define NRF24L01_REG_RX_ADDR_P0		0x0A	//Receive address data pipe 0. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P1		0x0B	//Receive address data pipe 1. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P2		0x0C	//Receive address data pipe 2. Only LSB
#define NRF24L01_REG_RX_ADDR_P3		0x0D	//Receive address data pipe 3. Only LSB
#define NRF24L01_REG_RX_ADDR_P4		0x0E	//Receive address data pipe 4. Only LSB
#define NRF24L01_REG_RX_ADDR_P5		0x0F	//Receive address data pipe 5. Only LSB
#define NRF24L01_REG_TX_ADDR		0x10	//Transmit address. Used for a PTX device only
#define NRF24L01_REG_RX_PW_P0		0x11	
#define NRF24L01_REG_RX_PW_P1		0x12	
#define NRF24L01_REG_RX_PW_P2		0x13	
#define NRF24L01_REG_RX_PW_P3		0x14	
#define NRF24L01_REG_RX_PW_P4		0x15	
#define NRF24L01_REG_RX_PW_P5		0x16	
#define NRF24L01_REG_FIFO_STATUS	0x17	//FIFO Status Register
#define NRF24L01_REG_DYNPD			0x1C	//Enable dynamic payload length
#define NRF24L01_REG_FEATURE		0x1D

//Registers default values
#define NRF24L01_REG_DEFAULT_VAL_CONFIG			0x08	
#define NRF24L01_REG_DEFAULT_VAL_EN_AA			0x3F	
#define NRF24L01_REG_DEFAULT_VAL_EN_RXADDR		0x03	
#define NRF24L01_REG_DEFAULT_VAL_SETUP_AW		0x03	
#define NRF24L01_REG_DEFAULT_VAL_SETUP_RETR		0x03	
#define NRF24L01_REG_DEFAULT_VAL_RF_CH			0x02	
#define NRF24L01_REG_DEFAULT_VAL_RF_SETUP		0x0E	
#define NRF24L01_REG_DEFAULT_VAL_STATUS			0x0E	
#define NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX		0x00	
#define NRF24L01_REG_DEFAULT_VAL_RPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2		0xC3	
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3		0xC4	
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4		0xC5
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5		0xC6
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4		0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P0		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P1		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P2		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P3		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P4		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P5		0x00
#define NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS	0x11
#define NRF24L01_REG_DEFAULT_VAL_DYNPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_FEATURE		0x00

//Configuration register
#define NRF24L01_MASK_RX_DR		6
#define NRF24L01_MASK_TX_DS		5
#define NRF24L01_MASK_MAX_RT	4
#define NRF24L01_EN_CRC			3
#define NRF24L01_CRCO			2
#define NRF24L01_PWR_UP			1
#define NRF24L01_PRIM_RX		0

//Enable auto acknowledgment
#define NRF24L01_ENAA_P5		5
#define NRF24L01_ENAA_P4		4
#define NRF24L01_ENAA_P3		3
#define NRF24L01_ENAA_P2		2
#define NRF24L01_ENAA_P1		1
#define NRF24L01_ENAA_P0		0

//Enable rx addresses
#define NRF24L01_ERX_P5			5
#define NRF24L01_ERX_P4			4
#define NRF24L01_ERX_P3			3
#define NRF24L01_ERX_P2			2
#define NRF24L01_ERX_P1			1
#define NRF24L01_ERX_P0			0

//Setup of address width
#define NRF24L01_AW				0 //2 bits

//Setup of auto re-transmission
#define NRF24L01_ARD			4 //4 bits
#define NRF24L01_ARC			0 //4 bits

//RF setup register
#define NRF24L01_PLL_LOCK		4
#define NRF24L01_RF_DR_LOW		5
#define NRF24L01_RF_DR_HIGH		3
#define NRF24L01_RF_DR			3
#define NRF24L01_RF_PWR			1 //2 bits   

//General status register
#define NRF24L01_RX_DR			6
#define NRF24L01_TX_DS			5
#define NRF24L01_MAX_RT			4
#define NRF24L01_RX_P_NO		1 //3 bits
#define NRF24L01_TX_FULL		0

//Transmit observe register
#define NRF24L01_PLOS_CNT		4 //4 bits
#define NRF24L01_ARC_CNT		0 //4 bits

//FIFO status
#define NRF24L01_TX_REUSE		6
#define NRF24L01_FIFO_FULL		5
#define NRF24L01_TX_EMPTY		4
#define NRF24L01_RX_FULL		1
#define NRF24L01_RX_EMPTY		0

//Dynamic length
#define NRF24L01_DPL_P0			0
#define NRF24L01_DPL_P1			1
#define NRF24L01_DPL_P2			2
#define NRF24L01_DPL_P3			3
#define NRF24L01_DPL_P4			4
#define NRF24L01_DPL_P5			5

//Transmitter power
#define NRF24L01_M18DBM			0 //-18 dBm
#define NRF24L01_M12DBM			1 //-12 dBm
#define NRF24L01_M6DBM			2 //-6 dBm
#define NRF24L01_0DBM			3 //0 dBm

//Data rates
#define NRF24L01_2MBPS			0
#define NRF24L01_1MBPS			1
#define NRF24L01_250KBPS		2

#define NRF24L01_CONFIG			((1 << NRF24L01_EN_CRC) | (0 << NRF24L01_CRCO))

//Instruction Mnemonics
#define NRF24L01_REGISTER_MASK				0x1F

#define NRF24L01_READ_REGISTER_MASK(reg)	(0x00 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_WRITE_REGISTER_MASK(reg)	(0x20 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_R_RX_PAYLOAD_MASK			0x61
#define NRF24L01_W_TX_PAYLOAD_MASK			0xA0
#define NRF24L01_FLUSH_TX_MASK				0xE1
#define NRF24L01_FLUSH_RX_MASK				0xE2
#define NRF24L01_REUSE_TX_PL_MASK			0xE3
#define NRF24L01_ACTIVATE_MASK				0x50 
#define NRF24L01_R_RX_PL_WID_MASK			0x60
#define NRF24L01_NOP_MASK					0xFF

//Clear interrupt flags
#define NRF24L01_CLEAR_INTERRUPTS			TM_NRF24L01_WriteBit(7, 4, Bit_SET); TM_NRF24L01_WriteBit(7, 5, Bit_SET); TM_NRF24L01_WriteBit(7, 6, Bit_SET)		

//Flush FIFOs
#define NRF24L01_FLUSH_TX					NRF24L01_CSN_LOW; TM_SPI_Send(NRF24L01_SPI, NRF24L01_FLUSH_TX_MASK); NRF24L01_CSN_HIGH
#define NRF24L01_FLUSH_RX					NRF24L01_CSN_LOW; TM_SPI_Send(NRF24L01_SPI, NRF24L01_FLUSH_RX_MASK); NRF24L01_CSN_HIGH

#define NRF24L01_TRANSMISSON_OK 			0
#define NRF24L01_MESSAGE_LOST   			1

#define NRF24L01_CHECK_BIT(reg,bit)			(reg & (1 << bit))

typedef enum {
	TM_NRF24L01_Transmit_Status_Lost = 0,		//Message is lost, reached max retransmissions
	TM_NRF24L01_Transmit_Status_Ok = 1,			//Message sent successfully
	TM_NRF24L01_Transmit_Status_Sending = 0xFF	//Message is still sending
} TM_NRF24L01_Transmit_Status_t;

typedef enum {
	TM_NRF24L01_DataRate_2M,		// 2Mbps
	TM_NRF24L01_DataRate_1M,		// 1Mbps
	TM_NRF24L01_DataRate_250k		// 250kbps
} TM_NRF24L01_DataRate_t;

typedef enum {
	TM_NRF24L01_OutputPower_M18dBm,	// -18dBm
	TM_NRF24L01_OutputPower_M12dBm,	// -12dBm
	TM_NRF24L01_OutputPower_M6dBm,	// -6dBm
	TM_NRF24L01_OutputPower_0dBm	// 0dBm
} TM_NRF24L01_OutputPower_t;

typedef struct {
	uint8_t PayloadSize;				//Payload size
	uint8_t Channel;					//Channel selected
	TM_NRF24L01_OutputPower_t OutPwr;	//Output power
	TM_NRF24L01_DataRate_t DataRate;	//Data rate
} TM_NRF24L01_t;

/**
 * Initialize NRF24L01+
 *
 * Parameters:
 * 	- uint8_t channel: channel you will use, from 0 to 125 eg. working frequency from 2.4 to 2.525 GHz
 * 	- uint8_t payload_size: maximum data to be sent in one packet
 *
 * 	Returns 1
 */
extern uint8_t TM_NRF24L01_Init(uint8_t channel, uint8_t payload_size);

/**
 * Set MY address
 *
 * Parameters:
 * 	- uint8_t* adr: pointer to 5 bytes of address
 */
extern void TM_NRF24L01_SetMyAddress(uint8_t* adr);

extern void TM_NRF24L01_SetPipe2Address(uint8_t adr);
extern void TM_NRF24L01_SetPipe3Address(uint8_t adr);
extern void TM_NRF24L01_SetPipe4Address(uint8_t adr);
extern void TM_NRF24L01_SetPipe5Address(uint8_t adr);

/**
 * Set address you will communicate with
 *
 * Parameters:
 * 	- uint8_t* adr: pointer to 5 bytes of address
 */
extern void TM_NRF24L01_SetTxAddress(uint8_t* adr);

/**
 * Get number of transmissions needed in last sent
 *
 * Returns number from 0 to 15
 */
extern uint8_t TM_NRF24L01_GetRetransmissionsCount(void);

/**
 * Put NRF24L01 to TX mode
 *
 *
 */
extern void TM_NRF24L01_PowerUpTx(void);

/**
 * Put NRF24L01 to RX mode
 *
 *
 */
extern void TM_NRF24L01_PowerUpRx(void);

/**
 * Put NRF24L01 to power down mode
 *
 * In this mode, you will not be able to receive or transmit data
 */
extern void TM_NRF24L01_PowerDown(void);

/**
 * Get transmissions status
 *
 * Returns members of TM_NRF24L01_Transmit_Status_t struct:
 * 	- TM_NRF24L01_Transmit_Status_Lost if message is lost
	- TM_NRF24L01_Transmit_Status_Ok if message is OK sent
	- TM_NRF24L01_Transmit_Status_Sending if message is still sending
 */
extern TM_NRF24L01_Transmit_Status_t TM_NRF24L01_GetTransmissionStatus(void);

/**
 * Transmit data with NRF24L01
 *
 * Parameters:
 * 	- uint8_t *data: Pointer to bytes to be sent, max length of payload_size
 */
extern void TM_NRF24L01_Transmit(uint8_t *data);

/**
 * Get data from NRF24L01
 *
 * Parameters:
 * 	- uint8_t *data: pointer where to save data
 */
extern void TM_NRF24L01_GetData(uint8_t *data);

/**
 * Checks if data is ready to be read from NRF24L01
 *
 * Returns 1 if ready, 0 if not
 */
extern uint8_t TM_NRF24L01_DataReady(void);

/**
 * Set working channel
 *
 * Parameters:
 * 	- uint8_t channel: channel from 0 to 125
 */
extern void TM_NRF24L01_SetChannel(uint8_t channel);

/**
 * Set RF parameters for NRF24L01
 *
 * Parameters:
 * 	- TM_NRF24L01_DataRate_t DataRate: Data rate
 * 		- TM_NRF24L01_DataRate_2M			2Mbps datarate
 * 		- TM_NRF24L01_DataRate_1M			1Mbps datarate
 * 		- TM_NRF24L01_DataRate_250k			250k datarate
 * 	- TM_NRF24L01_OutputPower_t OutPwr: output power
 * 		- TM_NRF24L01_OutputPower_M18dBm	-18 dBm power
 * 		- TM_NRF24L01_OutputPower_M12dBm	-12 dBm power
 * 		- TM_NRF24L01_OutputPower_M6dBm		-6 dBm power
 * 		- TM_NRF24L01_OutputPower_0dBm		0dBm power
 */
extern void TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_t DataRate, TM_NRF24L01_OutputPower_t OutPwr);

/**
 * Private functions
 */
extern uint8_t TM_NRF24L01_GetStatus(void);
extern void TM_NRF24L01_InitPins(void);
extern void TM_NRF24L01_WriteBit(uint8_t reg, uint8_t bit, BitAction value);
extern uint8_t TM_NRF24L01_ReadBit(uint8_t reg, uint8_t bit);
extern uint8_t TM_NRF24L01_ReadRegister(uint8_t reg);
extern void TM_NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count);
extern void TM_NRF24L01_WriteRegister(uint8_t reg, uint8_t value);
extern void TM_NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count);
extern void TM_NRF24L01_SoftwareReset(void);
extern uint8_t TM_NRF24L01_RxFifoEmpty(void);

#endif

