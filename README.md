# STM32F4
## Libraries and projects

Here are listed all libraries for STM32F4 devices.
Libraries are designed to work with STM32F4xx series of MCU.
Every project here is created with Keil uVision, but tested with GCC compiler too (Coocox).
Each project includes 4 targets:

1. STM32F429-Discovery board (STM32F429ZI, 180MHz)
2. STM32F4-Discovery board (STM32F407VG, 168MHz)
3. STM32F401-Discovery (STM32F401, 84MHz)
4. STM32F411C-Discovery (STM32F411, 100MHz)
5. Nucleo F411-RE (STM32F411RE, 100MHz)
6. Nucleo F401-RE (STM32F401RE, 84MHz)
7. STM32F446xx (180MHz)
8. STM32439-Eval (STM32F439NI, 180MHz)

When you download repository and open project, you have to set correct your target, for device you use.
If you use other device in your project, then set your project and copy settings there from target options.

More information below
http://stm32f4-discovery.com


First time with Discovery board?
http://stm32f4-discovery.com

Current librarires:

- All libraries listed at one place for easy editing them from one place and use them for all projects

1. Two default projects are included
	1. Default Keil project, with CMSIS files v.1.5.0 included
	2. Default Keil project, with CMSIS files v.1.5.0 included and clock for STM32F429-Discovery/STM32F446 slowed down to 168MHz and for Nucleo F411 down to 96MHz for USB communication
2. LED_BUTTON - Project example for STM32F4/29 Discovery
http://bit.ly/1nbAHbs
3. DELAY - Delay library for STM32F4/29
http://bit.ly/1gFrBjl
4. USART - USART library for STM32F4/29
http://bit.ly/1npVXHQ
5. SPI - SPI library for STM32F4/29
http://bit.ly/1lgtcjy
6. ADC - ADC library for STM32F4/29
http://bit.ly/1fINniy
7. DAC - DAC library for STM32F4/29
http://bit.ly/1rwGdTR
8. ILI9341 LCD - STM32F429 Discovery ILI9341 LCD library
http://bit.ly/PQQnCm
9. I2C - I2C library for STM32F4/29
http://bit.ly/1o4TS4q
10. STMPE811 touch driver for STM32F4/29 Discovery board
http://bit.ly/1ob7hYK
11. ILI9341 Button - Button library for ILI9341 LCD and STMPE811 Touch driver on STM32F429
http://bit.ly/RdiIDS
12. ONEWIRE - OneWire library for STM32F4/29
http://bit.ly/1nvT6jG
13. DS18B20 - DS18B20 OneWire temperature sensor library for STM32F4/29
http://bit.ly/1viCnSX
14. SDRAM - ISSI 8MByte SDRAM on STM32F429 Discovery board
http://bit.ly/1jVGY96
15. DS1307 - DS1307 RTC modeule for STM32F429 Discovery
http://bit.ly/1oxFaDK
16. HD44780 - HD44780 Alpha numeric LCD driver for STM32F429 discovery
http://bit.ly/1gR6Lzz
17. NRF24L01+ - NRF24L01+ 2.4 GHz transceiver library
http://bit.ly/1pTkrgY
18. ILI8341 LTDC - ILI9341 LCD with LTDC hardware only for STM32F429
http://bit.ly/1jeZaqH
19. Internal RTC - Library for internal RTC on STM32F4xx devices
http://bit.ly/U4wmKW
20. Independent watchdog - Library for watchdog on STM32F4xx devices
http://bit.ly/1w2ru5n
21. FATFS - Read SDcard with STM32F4xx using SDIO or SPI communication
http://bit.ly/1k9rJGo
22. RNG - Use of True random number generator peripheral on STM32F4xx devices
http://bit.ly/1sCyx4w
23. MFRC522 - Read RFID Cards with STM32F4xx devices
http://bit.ly/1ycHq7P
24. USB VCP - Use STM32F4xx as USB VCP device (Virtual COM Port)
http://bit.ly/1sgSy0M
25. AM2301 - AM2301 (DHT21) temperature and humidity sensor
http://bit.ly/1p2mqy1 
26. ROTARY ENCODER - Connect Rotary Encoder to STM32F4xx device
http://bit.ly/1sWSUeh
27. GPS - Read GPS data on STM32F4xx devices
http://bit.ly/1n93Dgb
28. L3GD20 - Measure rotation of all 3 axis with L3GD20 gyro sensor
http://bit.ly/1pNdpt4
29. USB MSC HOST - Read USB flash drive with STM32F4xx
http://bit.ly/1lAs4Im
30. HC-SR04 - Measure distance with ultrasonic sensor on STM32F4xx
http://bit.ly/1tMP7AX
31. USB HID HOST - Read USB mouse and keyboard on STM32F4xx
http://bit.ly/1tKFMIn
32. 4x4 KEYPAD - Read matrix keypad up to 4x4 with STM32F4xx
http://bit.ly/1qvYtyD
33. PWM - Use PWM on all possible timers for STM32F4xx device
http://bit.ly/1CFInc5
34. USB HID Device - Turn STM32F4 into USB HID Device
http://bit.ly/1qRtTkL
35. LIS302DL/LIS3DSH - Accelerometers on STM32F4-Discovery board. Also can be used with any STM32F4 device
http://bit.ly/X8z9UQ
36. DAC SIGNAL - Output predefined signals with STM32F4 using DAC, DMA and TIM
http://bit.ly/1phUdPD
37. BMP180 - Read atmosferic pressure on STM32F4 device
http://bit.ly/1qfGTve
38. EXTI - Use external interrupts with STM32F4
http://bit.ly/10ltRHI
39. LOW POWER - Power consumption modes for STM32F4
http://bit.ly/ZIxAys
40. MCO OUTPUT - Output different clocks from STM32F4
http://bit.ly/1vsPy4y
41. STDIO - Use printf and fprintf/vfprintf on STM32F4
http://bit.ly/1qRc9lQ
42. SERVO - Control RC servo with STM32F4
http://bit.ly/1oMbaYV
43. MPU-6050 - Read accelerometer and gyroscope data with STM32F4
http://bit.ly/ZZlzEE
44. Identification - Read identification bytes and 96-bits long UNIQUE ID from STM32F4
http://bit.ly/1sbPXte
45. BKPSRAM - Interface backup SRAM on STM32F4
http://bit.ly/1qALFKQ
46. SWO DEBUG - Debug STM32F4 using SWO output
http://bit.ly/1vkRqbi
47. CRC - STM32F4 CRC Hardware module
http://bit.ly/1xliSGA
48. PWM IN - Measure input PWM signal on STM32F4
http://bit.ly/1BEWaND
49. OTP - One-time programmable memory on STM32F4
http://bit.ly/1AY3tlE
50. STemWin - STemWin implementation for STM32F429-Discovery
http://bit.ly/1z1znP1
51. DMA2D GRAPHIC- Chrom-ART Accelerator on STM32F429-Discovery board
http://bit.ly/1H7V83S
52. ETHERNET - Connect ethernet to STM32F4xx
http://bit.ly/1EzX4z7
53. GPIO - GPIO pins on STM32F4
http://bit.ly/1wgnhkD
54. GENERAL - General library for STM32F4
http://bit.ly/1D8RPpX
55. USART DMA - Extend USART with DMA TX functionality
http://bit.ly/1HchE9L
56. SPI DMA - Extend SPI with DMA functionality
http://bit.ly/1CQPPyO
57. BUTTON - Use buttons with STM32F4xx
http://bit.ly/1Gbp8po
58. STRING - Dynamic string allocation for STM32F4xx
http://bit.ly/1K9toJE
59. RCC - Dynamic PLL settings for STM32F4xx
http://bit.ly/1ApFUo1
60. CPU LOAD - CPU load monitoring for STM32F4xx
http://bit.ly/1FKF4EJ
61. SSD1306 - Use OLED LCD with STM32F4
http://bit.ly/1IVVR8z
62. FFT - FFT Library for STM32F4xx devices
http://bit.ly/1HIZxu1
63. DMA - DMA library for STM32F4xx
http://bit.ly/1IBlpoT

- PCD8544 LCD (Nokia 5110/3310) - Library for LCD
http://bit.ly/1i7l4tB
