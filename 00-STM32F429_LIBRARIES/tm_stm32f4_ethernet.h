/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/02/library-52-ethernet-peripheral-on-stm32f4xx/
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   ETHERNET library for STM32F4xx devices with built-in Ethernet controller
 *          and external PHY with RMII connnection (tested with DP83848)
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_ETHERNET_H
#define TM_ETHERNET_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_ETHERNET
 * @brief    Ethernet library for STM32F4xx devices with built in ETH MAC - http://stm32f4-discovery.com/2015/02/library-52-ethernet-peripheral-on-stm32f4xx/
 * @{
 *
 * \par Pinout
 *
 * Library supports 2 modes of connecting and use:
 *  - MII: Media Independent Interface
 *  - RMII: Reduced Media Independent Interface
 *
 * \par MII Connection
 *
 * MII connection uses a lot of pins, but has slower (25MHz) reference clock.
 *
 * It uses 4 data bytes and with 25MHz clock, you get 100Mbit connection.
 *
 * To enable MII connection mode, open defines.h file and add define like below:
 *
@verbatim
//Enable MII connection mode
#define ETHERNET_MII_MODE
@endverbatim
 *
 *
@verbatim
ETH PHY     |STM32F4xx with ethernet

MDIO        |PA2
MDC         |PC1
REF_CLK     |PA1
CRS         |PH2
COL         |PH3
PPS_OUT     |PB5
RXD0        |PC4
RXD1        |PC5
RXD2        |PH6
RXD3        |PH7
RX_ERR      |PI10
RX_DV       |PA7
RX_CLK      |PC3
TX_EN       |PG11
TXD0        |PG13
TXD1        |PG14
TXD2        |PC2
TXD3        |PB8
@endverbatim
 *
 * \par RMII Connection
 *
 * RMII connection uses a small number of pins, but has greater (50MHz) reference clock.
 *
 * It uses 2 data bytes and with 50MHz clock, you get 100Mbit connection.
 *
 * RMII mode is supported using 2 pinspacks. By default, PP1 (PinsPack 1) is used. If you want to use PP2, then you need define in defines.h file:
 *
@verbatim
//Enable PinsPack 2 for RMII ethernet:
#define ETHERNET_RMII_PINSPACK_2
@endverbatim
 *
@verbatim
ETH PHY     |STM32F4xx with ethernet
            |PP1    |PP2
		
MDIO        |PA2    |PA2
MDC         |PC1    |PC1
REF_CLK     |PA1    |PA1
CRS         |PA7    |PA7
RXD0        |PC4    |PC4
RXD1        |PC5    |PC5
TX_EN       |PB11   |PG11
TXD0        |PB12   |PG13
TXD1        |PB13   |PG14
@endverbatim
 *
 * Also, PA8 pins HAVE TO be initialized as MCO alternate function, or communication will not work. Who knows why.
 *
 * PA8 pin is also MCO output, which has ability to output different clocks.
 *
 * I've added feature to output HSE crystal on this MCO pin. It can be used for 25MHz output if you have HSE 25MHz crystal and MII connection.
 *
 * Then you don't need separate crystal for PHY device.
 *
 * To enable MCO output HSE crystal value, open defines.h file and add define:
 *
@verbatim
//Enable MCO output with HSE clock
#define ETHERNET_MCO_CLOCK
@endverbatim
 *
 * \par DHCP
 *
 * Library supports DHCP to get IP from router.
 * This feature is disabled by default
 * To enable it, you need to add define in defines.h file:
 *
@verbatim
//Enable DHCP 
#define ETHERNET_USE_DHCP
@endverbatim
 *
 *\par Supported PHY devices
 *
 * This library supports more than just DP83848 PHY. Currently, supported list is below.
 * The number which is near text means value you have to set @ref ETHERNET_PHY define in case you want to select custom PHY.
 *
 *	- 0: DP83848
 *	- 1: LAN8720A
 *	- 2: KSZ8081RNA
 *
 * If you want (for example) to use KSZ8081RNA PHY, you need to do this (defines.h):
 *
@verbatim
//Use KSZ8081RNA PHY
#define ETHERNET_PHY   2
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 18, 2015
  - Added support for MII connection
  - Tested on STM324x9-Eval board

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx GPIO
 - STM32F4xx ETH
 - defines.h
 - attributes.h
 - LwIP stack
 - TM DELAY
 - TM GPIO
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "attributes.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/* Ethernet based includes */
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "tcp_echoclient.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"

/* C based includes */
#include "string.h"
#include "stdio.h"

/**
 * @defgroup TM_ETHERNET_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Ethernet PHY chip you will use for low level communication
 * @note   Default is DP83848 PHY
 */
#ifndef ETHERNET_PHY
#define ETHERNET_PHY						0
#endif

/**
 * @brief  Number of milliseconds when to check for link status from PHY
 */
#ifndef LINK_TIMER_INTERVAL
#define LINK_TIMER_INTERVAL					1000
#endif

/**
 * @brief  Default server PORT used for server 
 */
#ifndef ETHERNET_SERVER_PORT
#define ETHERNET_SERVER_PORT				80
#endif

/* Include server based options */
#include "httpd.h"
#include "fs.h"

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
/* In case you want to use custom MAC, use parameter in init function */
/**
 * @brief  Default MAC address for your device
 * @note   This MAC is used in case you set mac_addr param in @ref TM_ETHERNET_Init() function to "NULL"
 */
#ifndef MAC_ADDR0
#define MAC_ADDR0							0x06
#define MAC_ADDR1							0x05
#define MAC_ADDR2							0x04
#define MAC_ADDR3							0x03
#define MAC_ADDR4							0x02
#define MAC_ADDR5							0x01
#endif

/* Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
/* Used in case DHCP is not used or response failed */
/* In case you want to use custom IP, use parameter in init function */
/**
 * @brief  Default IP address
 * @note   Used in case DHCP is not used or response failed 
 * @note   In case you want to use custom IP, use parameter in @ref TM_ETHERNET_Init() function
 */
#ifndef IP_ADDR0
#define IP_ADDR0							192
#define IP_ADDR1							168
#define IP_ADDR2							0
#define IP_ADDR3							120
#endif

/**
 * @brief  Netmask address
 * @note   In case you want to use custom netmask, use parameter in @ref TM_ETHERNET_Init() function
 */
#ifndef NETMASK_ADDR0
#define NETMASK_ADDR0						255
#define NETMASK_ADDR1						255
#define NETMASK_ADDR2						255
#define NETMASK_ADDR3						0
#endif

/**
 * @brief  Gateway address
 * @note   In case you want to use custom gateway, use parameter in @ref TM_ETHERNET_Init() function
 */
#ifndef GW_ADDR0
#define GW_ADDR0							192
#define GW_ADDR1							168
#define GW_ADDR2							0
#define GW_ADDR3							1
#endif

/**
 * @brief  Default device name to be shown to your router when requesting IP address with DHCP 
 */
#ifndef ETHERNET_HOSTNAME_DHCP
#define ETHERNET_HOSTNAME_DHCP				"TM-ETH-Device"
#endif

/* RMII mode by default */
#if !defined(ETHERNET_MII_MODE) && !defined(ETHERNET_RMII_MODE)
	#define ETHERNET_RMII_MODE
#endif

/* Buffer size for ethernet headers when requesting as client */
/* Used for headers after you are connected to server, HTTP headers */
/* Ex: GET / HTTP/1.1\r\n...etc */
/**
 * @brief  Buffer size for ethernet headers when requesting as client
 * @note   Used for headers after you are connected to server, HTTP headers
 *         Ex: GET / HTTP/1.1\r\n...etc
 */
#ifndef ETHERNET_MAX_HEADER_SIZE
#define ETHERNET_MAX_HEADER_SIZE			1024
#endif  

/**
 * @brief  Max connection string name when connection as client
 * @note   This is also max domain name when using DNS request
 */
#ifndef ETHERNET_MAX_CONNECTION_NAME
#define ETHERNET_MAX_CONNECTION_NAME		100
#endif

/** 
 * @brief  Maximal number of opened files at a time as server
 */
#ifndef ETHERNET_MAX_OPEN_FILES
#define ETHERNET_MAX_OPEN_FILES     		10
#endif

/**
 * @brief  Maximal length for SSI tag used in HTML
 */
#ifndef ETHERNET_SSI_MAX_TAG_LEN
#define ETHERNET_SSI_MAX_TAG_LEN			32
#endif

/**
 * @brief  Maximal buffer length for SSI tag insert text
 * @note   Content of buffer will be replaced with SSI tag
 */
#ifndef ETHERNET_SSI_MAX_TAG_INSERT_LEN
#define ETHERNET_SSI_MAX_TAG_INSERT_LEN		512
#endif

 /**
 * @}
 */

/**
 * @defgroup TM_ETHERNET_Typedefs
 * @brief    Library Typedefs
 * @{
 */
 
/**
 * @defgroup TM_ETHERNET_PRIVATE_Typedefs
 * @brief    Library private Typedefs
 * @{
 */

/*
 * @brief  Internal structure which should not be used by user
 * @note   For private use
 */
typedef struct {
	uint8_t ip_addr[4];
	uint8_t mac_addr[6];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t staticip;
	uint8_t ip_is_set;
	uint8_t link_is_up;
	uint32_t ClientSuccessfullConnections, ClientConnections;
	uint64_t Client_TX_Bytes, Client_RX_Bytes;
	uint32_t Server_Connections;
	uint64_t Server_TX_Bytes, Server_RX_Bytes;
	uint8_t server_active;
	uint16_t server_port;
	uint8_t speed_100m;
	uint8_t full_duplex;
	uint8_t timeout_detected;
} TM_ETHERNET_t;
extern TM_ETHERNET_t TM_ETHERNET;

/**
 * @brief  Client protocol states
 * @note   For private use
 */
typedef enum  {
	CLIENT_NOT_CONNECTED = 0,
	CLIENT_CONNECTED,
	CLIENT_RECEIVED,
	CLIENT_CLOSING,
} client_states;

 /**
 * @}
 */

/**
 * @brief  Client structure, passed as first parameter in all client based callback functions
 */
typedef struct {
	char name[ETHERNET_MAX_CONNECTION_NAME]; /*!< Connection name, we choose it when we call @ref TM_ETHERNETCLIENT_Connect function */
	uint8_t active;                          /*!< Connection is active */
	uint8_t ip_addr[4];                      /*!< IP address for our external connection */
	uint16_t port;                           /*!< Port for our external connection */
	uint8_t headers_done;                    /*!< This is user defined option.
                                                  When you connect to some website and receive data back, you will also get HTTP headers and your data.
                                                  When receive data handler will be first called, this parameter will be set to 0.
                                                  If you detect when headers are done ("\r\n\r\n" string) then you can set this parameter to 1,
                                                  and it will stay 1 until connection is closed. This can be used, if you don't want to print headers to user. */
	uint8_t* active_connections_count;       /*!< Pointer to number of active connections this time. */
	void* user_parameters;                   /*!< Pointer to user parameters for connection which are passed on "Connect" function
                                                  This can be used to pass special data for your connection, which you can then easily be used in headers callback
                                                  to format proper request string. */
	/* Private use */
	struct tcp_pcb* pcb;                     /*!< Pointer to currently active PCB */
	client_states state;                     /*!< client status */
	struct pbuf *p_tx;                       /*!< Pointer to current active buffer */
} TM_TCPCLIENT_t;

/**
 * @brief  POST request structure
 * @note   Used in POST request callbacks
 * @note   POST is not fully tested and it is not recommended to use.
 */
typedef struct {
	void* connection;          /*!< Pointer to connection, it is type of struct http_state * */
	const char* uri;           /*!< URL which is used in POST request. URL user use with POST request */
	const char* http_request;  /*!< Pointer to HTTP request buffer. Here are stored entire HTTP request headers */
	uint16_t http_request_len; /*!< Length of HTTP request buffer */
	int content_len;           /*!< Length of content (data) in bytes */
	char* response_uri;        /*!< Pointer to address (file address, "/index.html" for example) which will be used to show url to user. This parameter must be filled by user */
	uint16_t response_uri_len; /*!< Length of buffer for response uri */
} TM_ETHERNETPOST_t;

/**
 * @brief  Result enumeration used in ethernet library.
 */
typedef enum {
	TM_ETHERNET_Result_Ok = 0,               /*!< Everything is OK */
	TM_ETHERNET_Result_Error,                /*!< An error occured */
	TM_ETHERNET_Result_IPIsNotSetYet,        /*!< We don't have set IP */
	TM_ETHERNET_Result_LinkIsDown,           /*!< Link is down */
	TM_ETHERNET_Result_NeedHardReset,        /*!< We need hardware reset */
	TM_ETHERNET_Result_LibraryNotInitialized /*!< Library is not initialized */
} TM_ETHERNET_Result_t;

/**
 * @brief  User friendly typedef for SSI tags
 */
typedef const char * TM_ETHERNET_SSI_t;

/**
 * @brief  Structure for CGI tags 
 * @note   
 * @param  *pcCGIName: Link, when handler function will be called, "/leds.cgi" for example
 * @param  pfnCGIHandler: Function handler, which will be called when user access to some link
 * @note   Handler function looks like this:
 *            const char * HandlerFunctionName(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
 * @note   Function must return "URL" which will be displayed to user, for example "/index.html" to show first site
 */
typedef tCGI TM_ETHERNET_CGI_t;

/**
 * @}
 */

/**
 * @defgroup TM_ETHERNET_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @defgroup TM_ETHERNET_GLOBAL_Functions
 * @brief    Library global Functions
 * @{
 */
 
/**
 * @brief  Initializes ethernet device and prepare device to work.
 * @note   This function prepares LwIP stack to work and DP83848 PHY,
 *         but does not enable server functionality. You have separate
 *         function which enables server functionality in case you need it.
 * @param  *mac_addr: Pointer to 6 bytes long array, if you want to select custom
 *		      MAC address, instead of one in in defines.
 *		      Useful if you have more than one device in one network
 *		      and you want to separate them by custom MAC (with their unique ID number)
 *		      Set to NULL for default value
 * @param  *ip_addr:
 *		      Pointer to 4 bytes long array, if you want to select custom
 *		      IP address in case STATIC is used. This will also be used,
 *		      if DHCP can't get IP address
 *		      Set to NULL for default value
 * @param  *gateway:
 *		      Pointer to 4 bytes long array, if you want to select custom	gateway.
 *		      Set to NULL for default value
 * @param  *netmask:
 *		      Pointer to 4 bytes long array, if you want to select custom netmask
 *		      Set to NULL for default value
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNET_Init(uint8_t* mac_addr, uint8_t* ip_addr, uint8_t* gateway, uint8_t* netmask);

/**
 * @brief  Updates ethernet LwIP stack
 * @note   This function should be called periodically, as fast as possible
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNET_Update(void);

/**
 * @brief  This function should be called in specific time period for LwIP stack using interrupts
 * @param  millis: Number of milliseconds, that will be added to LwIP stack.
 *             This value should be the same as period of your interrupt (Systick) timer.
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNET_TimeUpdate(uint16_t millis);

/**
 * @brief  Tests if ethernet library is prepared to work as client or server
 * @param  None
 * @retval Ethernet status
 *            - TM_ETHERNET_Result_Ok: In case link is up and ip is set (static or DHCP)
 *            - TM_ETHERNET_Result_Error: On other cases
 */
TM_ETHERNET_Result_t TM_ETHERNET_TestReady(void);

/**
 * @brief  Checks if device has static IP
 * @param  None
 * @retval IP status
 *            - 0: IP assigned via DHCP
 *            - > 0: IP is static
 */
#define TM_ETHERNET_IsIPStatic()            (TM_ETHERNET.staticip)

/**
 * @brief  Checks if device has 100Mbit network connection
 * @param  None
 * @retval 100Mbit connection status
 *            - 0: Connection is 10Mbit
 *            - > 0: Connection is 100Mbit
 */
#define TM_ETHERNET_Is100M()                (TM_ETHERNET.speed_100m)

/**
 * @brief  Checks if device is in full duplex mode
 * @param  None
 * @retval Full duplex mode status
 *            - 0: Device is in full duplex mode
 *            - > 0: Device is in half dupley mode
 */
#define TM_ETHERNET_IsFullDuplex()          (TM_ETHERNET.full_duplex)

/**
 * @brief  Get local IP address
 * @param  x: IP section, 0 to 3 are allowed. 0 is MSB and 3 is LSB
 * @retval IP address is returned at x location of address
 */
#define TM_ETHERNET_GetLocalIP(x)			(((x) >= 0 && (x) < 4) ? TM_ETHERNET.ip_addr[(x)] : 0)

/**
 * @brief  Get MAC address
 * @param  x: MAC section, 0 to 5 are allowed. 0 is MSB and 5 is LSB
 * @retval MAC address is returned at x location of address
 */
#define TM_ETHERNET_GetMACAddr(x)			(((x) >= 0 && (x) < 6) ? TM_ETHERNET.mac_addr[(x)] : 0)

/**
 * @brief  Get gateway address
 * @param  x: IP section, 0 to 3 are allowed. 0 is MSB and 3 is LSB
 * @retval Gateway address is returned at x location of address
 */
#define TM_ETHERNET_GetGateway(x)			(((x) >= 0 && (x) < 4) ? TM_ETHERNET.gateway[(x)] : 0)

/**
 * @brief  Get netmask address
 * @param  x: Netmask section, 0 to 3 are allowed. 0 is MSB and 3 is LSB
 * @retval Netmask address is returned at x location of address
 */
#define TM_ETHERNET_GetNetmask(x)			(((x) >= 0 && (x) < 4) ? TM_ETHERNET.netmask[(x)] : 0)

/**
 * @brief  Sets flag when timeout was detected by your software
 * @note   When ethernet is not inside your network (not come to your router), you can call this function,
 *            and it will disable any future ethernet client requests to other servers.
 * @note   After you set this flag you will have to make software reset to get flag back
 * @param  None
 * @retval None
 */
#define TM_ETHERNET_SetTimeoutDetected()    (TM_ETHERNET.timeout_detected = 1)

/**
 * @}
 */
 
 /**
 * @defgroup TM_ETHERNET_SERVER_Functions
 * @brief    Library server based Functions
 * @{
 */

/**
 * @brief  Enables and prepares LwIP stack for server functionality.
 * @note   Before you can use this function, you have to initialize LwIP stack with @ref TM_ETHERNET_Init function.
 * @param  server_port: Select your port on which you want to operate.
 *            Set this variable to 0 and last used PORT will be selected
 *            If you call this function multiple times, only first time will work this parameter
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNETSERVER_Enable(uint16_t server_port);

/**
 * @brief  Disables and stops LwIP stack from server functionality.
 * @param  None
 * @retval Member of @ref TM_ETHERNET_Result_t is returned
 */
TM_ETHERNET_Result_t TM_ETHERNETSERVER_Disable(void);

/**
 * @brief  Checks if server mode is enabled.
 * @param  None
 * @retval Server active status
 *            - 0: Server mode is not active
 *            - > 0: Server mode is active
 */
#define TM_ETHERNETSERVER_Enabled()				(TM_ETHERNET.server_active)

/**
 * @brief  Gets port number on which server is active
 * @param  None
 * @retval Port number on which server is active
 */
#define TM_ETHERNETSERVER_GetPortNumber()		(TM_ETHERNET.server_port)

/**
 * @brief  Gets number of all sent bytes from server to client
 * @param  None
 * @retval Number of sent bytes from server to client
 */
#define TM_ETHERNETSERVER_GetTXBytes()			(TM_ETHERNET.Server_TX_Bytes)

/**
 * @brief  Gets number of all received bytes from client to server
 * @param  None
 * @retval Number of received bytes from client to server
 */
#define TM_ETHERNETSERVER_GetRXBytes()			(TM_ETHERNET.Server_RX_Bytes)

/**
 * @brief  Gets number of all connections that have been made to server
 * @param  None
 * @retval Number of connections made to server
 */
#define TM_ETHERNETSERVER_GetConnectionsCount()	(TM_ETHERNET.Server_Connections)

/**
 * @brief  Set SSI tags, which will be used in your HTML files for displaying variables on website.
 * @note   For more about SSI tags, you can check chapter 4.2 on link below:
 *
 *         http://www.state-machine.com/lwip/AN_QP_and_lwIP.pdf
 *
 *         Note: SSI tags only work if your file name ends with .shtm, .shtml, or .ssi!
 *
 *         This function can be called only once, future calls will be denied!
 * @param  *tags: Pointer to array of pointers for tags that are used in your html files
 * @param  number_of_tags: Number of tags in your array
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNETSERVER_SetSSITags(TM_ETHERNET_SSI_t* tags, uint16_t number_of_tags);

/**
 * @brief  Set GCI handlers which will be used when you want to control something over ethernet configured as server, for example, turn on/off leds.
 * @note   For more about CGI tags, you can check chapter 4.3 on link below:
 *
 *         http://www.state-machine.com/lwip/AN_QP_and_lwIP.pdf
 *
 *         CGI tags only work if your file name ends with .cgi!
 *
 *         This function can be called only once, future calls will be denied!
 *
 * @param  *cgis: Pointer to TM_ETHERNET_CGI_t structure with link/function pair combination. It can be array of structures
 * @param  number_of_handlers: Number of pairs for .cgi and handler combination
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNETSERVER_SetCGIHandlers(const TM_ETHERNET_CGI_t* cgis, uint16_t number_of_handlers);

/**
 * @}
 */
 
 /**
 * @defgroup TM_ETHERNET_CLIENT_Functions
 * @brief    Library client based Functions
 * @{
 */
 
/**
 * @brief  Makes TCP CLIENT Connection to external server.
 * @note   After this call, different client callbacks will be called depending on what will happen.
 * @param  *conn_name: Connection name. This value will be then passed to all client based handlers
 * @param  ip1: IP address. ip1 is MSB, ip4 is LSB
 * @param  ip2: IP address. ip1 is MSB, ip4 is LSB
 * @param  ip3: IP address. ip1 is MSB, ip4 is LSB
 * @param  ip4: IP address. ip1 is MSB, ip4 is LSB
 * @param  port: Port to which we want connect to server
 * @param  *user_parameters: Pointer to user data to be passed to this function.
 *            This data will be used in all client based callbacks inside TM_TCPCLIENT_t pointer variable
 *            Use NULL in case you don't want to pass any data.
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNETCLIENT_Connect(char* conn_name, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t port, void* user_parameters);

/**
 * @brief  Gets total number of sent bytes when you are client
 * @param  None
 * @retval Number of transmitted bytes as a client
 */
#define TM_ETHERNETCLIENT_GetTXBytes()						(TM_ETHERNET.Client_TX_Bytes)

/**
 * @brief  Gets total number of received bytes when you are client
 * @param  None
 * @retval Number of received bytes as a client
 */
#define TM_ETHERNETCLIENT_GetRXBytes()						(TM_ETHERNET.Client_RX_Bytes)

/**
 * @brief  Gets number of all connections done as a client
 * @param  None
 * @retval Number of all connections done as a client
 */
#define TM_ETHERNETCLIENT_GetConnectionsCount()				(TM_ETHERNET.ClientConnections)

/**
 * @brief  Get number of all successfull connections done as a client
 * @param  None
 * @retval All successfull connections done as a client
 */
#define TM_ETHERNETCLIENT_GetSuccessfullConnectionsCount()	(TM_ETHERNET.ClientSuccessfullConnections)

/**
 * @}
 */
 
 /**
 * @defgroup TM_ETHERNET_DNS_Functions
 * @brief    Library DNS based Functions
 * @{
 */
 
/**
 * @param  Gets host by name via DNS server
 * @note   This function can be used to get IP address by passing it's domain name to it.
 * @note   Different DNS callback functions will be called, depending on what will happen with DNS.
 * @param  *host_name: Domain name for which you need IP address.
 * @retval Member of @ref TM_ETHERNET_Result_t
 */
TM_ETHERNET_Result_t TM_ETHERNETDNS_GetHostByName(char* host_name);

/**
 * @}
 */
 
 /**
 * @defgroup TM_ETHERNET_CALLBACK_Functions
 * @brief    Library callback Functions
 * @{
 */

/**
 * @brief  IP is set callback. This function is called when IP is assigned to device.
 * @note   It is called in case DHCP is used or static ip is used.
 * @note   It can be used to store IP address, if you use DHCP dynamic IP address.
 * @param  ip_addr1: IP address. ip_addr1 is MSB, ip_addr4 is LSB
 * @param  ip_addr2: IP address. ip_addr1 is MSB, ip_addr4 is LSB
 * @param  ip_addr3: IP address. ip_addr1 is MSB, ip_addr4 is LSB
 * @param  ip_addr4: IP address. ip_addr1 is MSB, ip_addr4 is LSB
 * @param  dhcp: This is set to 1 in case DHCP has been used for IP assign or 0 if static is used in case of user select or error
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNET_IPIsSetCallback(uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4, uint8_t dhcp);

/**
 * @brief  DHCP started callback
 * @note   This function is called when DHCP starts with IP address assigning for device.
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNET_DHCPStartCallback(void);

/**
 * @brief  Link is down callback
 * @note   This function will be called when link is down.
 * @note   This can happen, if your router is down, or you unplug cable
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNET_LinkIsDownCallback(void);

/**
 * @brief  Link is up callback
 * @note   This function will be called when link is up again.
 *         It is not called in case that link is up already on initialization at the begin
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNET_LinkIsUpCallback(void);

/**
 * @brief  System reset callback
 * @note   If client connection is opened and link become down in this time, after link is up back,
 *         then before new connection can be made, system MUST be restarted for security purposes.
 *         Before you call NVIC_SystemReset() in function to reset system, you can do important stuff here.
 *
 * @note   If you don't reset your system, then function which is used to connect as a client,
 *         will always refuse your try to connect to your specific IP.
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNET_SystemResetCallback(void);

/**
 * @brief  Create headers callback
 *
 *         This function is called when connection is prepared to send some headers data to server which is waiting for response.
 *
 *         If function is not implemented, then default headers are sent.
 *
 * \par Default headers
 *
@verbatim
GET / HTTP/1.1\r\n
Host: TM_ETHERNETClient\r\n
Connection: close\r\n
\r\n
@endverbatim
 *
 * @note   Buffer length can't be larger than @ref ETHERNET_MAX_HEADER_SIZE define.
 *
 * @param  *connection: Pointer to @ref TM_TCPCLIENT_t which contains current connection specifications.
 * @param  *buffer: Char memory buffer where to store your header data
 * @param  buffer_length: Maximal memory length for your header data
 * @retval Number of characters written in buffer.
 *         If you return 0 (zero) then connection will be closed.
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint16_t TM_ETHERNETCLIENT_CreateHeadersCallback(TM_TCPCLIENT_t* connection, char* buffer, uint16_t buffer_length);

/**
 * @brief  Receive data callback
 *
 *         This function is called when we have data available to be received.
 *
 * @note   It might happen that it will be called multiple times for one request, depends on server's response and length of data.
 * @param  *connection: Pointer to @ref TM_TCPCLIENT_t which contains current connection specifications.
 * @param  buffer: Memory buffer with data
 * @param  buffer_length: Number of characters in buffer
 * @param  Number of unread data. If this value is the same as buffer_length, then no remaining data is available in packet
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETCLIENT_ReceiveDataCallback(TM_TCPCLIENT_t* connection, uint8_t* buffer, uint16_t buffer_length, uint16_t total_length);

/**
 * @brief  Connected callback
 *
 *         This function is called when device is connected to specific server.
 * @param  *connection: Pointer to @ref TM_TCPCLIENT_t which contains current connection's specifications.
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETCLIENT_ConnectedCallback(TM_TCPCLIENT_t* connection);

/**
 * @brief  Client Connection closed callback
 *
 *         This function is called when connection is closed and you are ready to make a new one.
 *
 *         You can also detect, if connection was closed after success or error.
 * @param  *connection: Pointer to @ref TM_TCPCLIENT_t which contains current connection specifications.
 * @param  success: It is set to 1 if connection was closed after successfull transmission or 0 if because of failure
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETCLIENT_ConnectionClosedCallback(TM_TCPCLIENT_t* connection, uint8_t success);

/**
 * @brief  Client Error callback
 *
 *         This function is called when an error occured.
 *
 *         It will be called in case when IP address is not available
 * @param  *connection: Pointer to @ref TM_TCPCLIENT_t which contains current connection specifications.
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETCLIENT_ErrorCallback(TM_TCPCLIENT_t* connection);

/**
 * @brief  Connection started callback
 *
 *         This function will be called when connection to server has started
 * @param  *connection: Pointer to @ref TM_TCPCLIENT_t which contains current connection specifications.
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETCLIENT_ConnectionStartedCallback(TM_TCPCLIENT_t* connection);

/**
 * @brief  DNS Found callback
 *
 *         This function will be called when DNS server has valid IP address for your domain request.
 *
 *         It can be useful to make first connection in this function when you know your IP address where you must connect to.
 * @param  *host_name:
 * @param  ip_addr1: IP address 1st byte, MSB
 * @param  ip_addr2: IP address 2nd byte
 * @param  ip_addr3: IP address 3rd byte
 * @param  ip_addr4: IP address 4th byte, LSB
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETDNS_FoundCallback(char* host_name, uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4);

/**
 * @brief  DNS error callback
 *
 *         This function will be called when DNS error occured.
 * \par Possible errors
 *
 * 	- Requested domain does not exists,
 * 	- You have bad parameters in DNS function call
 *
 * @param  *host_name: Host name where an error occured
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETDNS_ErrorCallback(char* host_name);

/**
 * @brief  SSI callback for ethernet server
 *
 *         This function is called, when ethernet server wants to serve tag you pass in TM_ETHERNETSERVER_SetSSITags() function.
 * @param  iIndex: Tag index number. They are specified in order you pass to TM_ETHERNETSERVER_SetSSITags function
 * @param  *pcInsert: Pointer where you should set your content that will be displayed where your tag is specified in files
 * @param  iInsertLen: Max string length
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint16_t TM_ETHERNETSERVER_SSICallback(int iIndex, char *pcInsert, int iInsertLen);

/**
 * @brief  Client connected callback
 *         Called when new client is connected to server
 *
 *         This function can be used, to block IP address.
 *
 *         IP address, from where user access to your server is stored in *pcb variable:
@verbatim
remote_ip = pcb->remote_ip.addr;
@endverbatim
 *
 * @note   You can check here for your blocked IP addresses
 * @param  *pcb: Pointer to active LwIP PCB structure
 * @retval Connection status:
 *            - 0: You don't allow connection to server
 *            - 1: COnnection to server is allowed
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint8_t TM_ETHERNETSERVER_ClientConnectedCallback(struct tcp_pcb *pcb);

/**
 * @brief  Client disconnected from server
 *
 *         Called when new client is diconnected from server (Or connection closed)
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETSERVER_ClientDisconnectedCallback(void);

/**
 * @brief  Open file callback
 *
 *         Called when user based file should be open when new client is connected
 *         If you want to use server on your SDcard or USB flash or whatever,
 *         here you can open file and do the stuff.
 *
 * @note   You must set file->len parameter to your file size number!
 *
 * @note   If You don't open file (return 0) then default file in flash will be used,
 *         that will say "Hello world from default server file", stored in fsdata.c.
 *
 * @note   Only one file at a time will be opened, so if you need to mount card,
 *         you can mount it in this function also.
 * @param  *file: Pointer to @ref fs_file structure
 * @param  *name: File name that should be opened
 * @retval 0 in case you can't/don't want to open file, or 1 if file is successfully opened and prepared for reading
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
int TM_ETHERNETSERVER_OpenFileCallback(struct fs_file* file, const char *name);

/**
 * @brief  Close file callback
 *
 *         Called when reading file is finished and file can be closed.
 *
 * @note   If you need to unmount your SD card, you can do it here.
 * @param  *file: Pointer to @ref fs_file struct
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETSERVER_CloseFileCallback(struct fs_file* file);

/**
 * @brief  Read file callback
 *
 *         Called when file should be read.
 *
 * @note   This function may be called more than just one time, depends on file length.
 * @param  *file: Pointer to @ref fs_file struct
 * @param  *buffer: Pointer to buffer, where you should save your data
 * @param  count: Number of bytes that should be read. You can read less than this number. This is max number you can read this time
 * @retval Number of read bytes or -1 in case we are at the end of file.
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
int TM_ETHERNETSERVER_ReadFileCallback(struct fs_file* file, char* buffer, int count);

/**
 * @brief  Post request begin callback
 *
 *         This function is called, when POST request is trying to be started to our server.
 *
 *         With this function, you can accept/decline POST request, depending on parameters you receive
 *
 * @note   After POST request finish, you have to choose one file, which will be used to displayed to user after successfull POST end.
 *
 * @note   If you don't set it by yourself, then "/index.html" file will be used. You can do that to set your custom file:
@verbatim
strcpy(params->response_uri, "/yourfile.html");
@endverbatim
 *
 * @note   In this structure are also other variables, which one, you should look at @ref TM_ETHERNETPOST_t struct declaration.
 * @param  *params: Pointer to @ref TM_ETHERNETPOST_t structure which contains everything about POST connection.
 * @retval Accept or deny POST request
 *            - 0: Deny POST request
 *            - > 0: Allow POST request
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint8_t TM_ETHERNETSERVER_PostRequestBeginCallback(TM_ETHERNETPOST_t* params);

/**
 * @brief  Post request receive data available callback
 *
 *         This function is called, when we have "Content-Length" statement.
 *
 * @note   Function might be called multiple times, depends on how many data you try to sent over POST method.
 *
 * @note   Data are stored inside p->payload variable pointer, which is void pointer. Make a cast to char* variable.
 *
 * @note   In pbuf struct is also length of this packet, so you have a little control about that.
 *
 * @note   To know when you have last time called this function, you can make a compare of params variables.
 * @param  *params: Pointer to @ref TM_ETHERNETPOST_t structure which contains everything about POST connection.
 * @param  *p: Pointer to pbuf structure where data are stored.
 * @retval Receive more data
 *            - 0: STOP receiving POST data
 *            - > 0: Continue with receiveing more POST data from current connection
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint8_t TM_ETHERNETSERVER_PostRequestReceiveDataCallback(TM_ETHERNETPOST_t* params, struct pbuf* p);

/**
 * @brief  Post request end callback
 *
 *         This function is called when POST request is done.
 *
 *         This function must set response_uri variable to set response URL, which will be used to show to user.
 *         If you don't set it, then default will be used.
 * @param  *connection: Pointer to connection parameters, it's type is struct http_state *
 * @param  *response_uri: Pointer to buffer for response uri set
 * @param  response_uri_len: Length of response uri buffer length
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETSERVER_PostRequestEndCallback(void* connection, char* response_uri, u16_t response_uri_len);

/**
 * @brief  PHY low layer custom options callback
 *
 *         Called when ethernet peripheral is ready.
 *         In this function, user can do other stuff, depending on PHY which is used in project.
 *
 * \par Example
 *
 *	- Set LED mode for DP83848 PHY, or something else
 *
 * @param  PHYAddress: PHY address when using ETH_ReadPHYRegister and ETH_WritePHYRegister functions
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_ETHERNETPHY_CustomOptions(uint32_t PHYAddress);

/**
 * @brief  Called on ethernet GPIO initialization
 *         In this function, user can initialize custom pins, which will be used for communication with PHY.
 * @note   Don't implement this function in case you want default pinout
 * @note   With __weak parameter to prevent link errors if not defined by user
 * @retval User status:
 *            - 0: We want default pinout from library
 *            - > 0: We made custom pinout
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint8_t TM_ETHERNET_InitPinsCallback(void);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* Private functions which should not be called from user */
void TM_ETHERNET_INT_CustomOptions(uint32_t PHYAddress);
void TM_ETHERNET_INT_SetIPAddress(uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4, uint8_t dhcp);
void TM_ETHERNET_INT_LinkIsDownCallback(void);
void TM_ETHERNET_INT_LinkIsUpCallback(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

