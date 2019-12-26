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
https://stm32f4-discovery.net

First time with Discovery board?
https://stm32f4-discovery.net

Current librarires:

- All libraries listed at one place for easy editing them from one place and use them for all projects

1. Two default projects are included
2. [LED_BUTTON - Project example for STM32F4/29 Discovery](https://stm32f4-discovery.net/2014/04/stm32f429-discovery-gpio-tutorial-with-onboard-leds-and-button/)
3. [DELAY - Delay library for STM32F4/29](https://stm32f4-discovery.net/2014/04/03-stm32f429-discovery-system-clock-and-pretty-precise-delays/)
4. [USART - USART library for STM32F4/29](https://stm32f4-discovery.net/2014/04/04-connect-stm32f429-discovery-to-computer-with-usart/)
5. [SPI - SPI library for STM32F4/29](https://stm32f4-discovery.net/2014/04/05-stm32f429-discovery-spi-library/)
6. [ADC - ADC library for STM32F4/29](https://stm32f4-discovery.net/2014/04/06-ad-converter-on-stm32f429/)
7. [DAC - DAC library for STM32F4/29](https://stm32f4-discovery.net/2014/04/07-da-converter-on-stm32f429/)
8. [ILI9341 LCD - STM32F429 Discovery ILI9341 LCD library](https://stm32f4-discovery.net/2014/04/08-ili9341-lcd-on-stm32f429-discovery-board/)
9. [I2C - I2C library for STM32F4/29](https://stm32f4-discovery.net/2014/05/09-i2c-library-for-stm32f429/)
10. [STMPE811 touch driver for STM32F4/29 Discovery board](https://stm32f4-discovery.net/2014/05/10-stmpe811-touch-screen-driver-for-stm32f429-discovery-board/)
11. [ILI9341 Button - Button library for ILI9341 LCD and STMPE811 Touch driver on STM32F429](https://stm32f4-discovery.net/2014/05/11-button-library-for-ili9341-lcd-and-stmpe811-touch-controller-on-stm32f429-discovery-board/)
12. [ONEWIRE - OneWire library for STM32F4/29](https://stm32f4-discovery.net/2014/05/12-onewire-library-for-stm32f429-discovery/)
13. [DS18B20 - DS18B20 OneWire temperature sensor library for STM32F4/29](https://stm32f4-discovery.net/2014/05/13-reading-temperature-with-dallas-ds18b20-on-stm32f429-discovery-board/)
14. [SDRAM - ISSI 8MByte SDRAM on STM32F429 Discovery board](https://stm32f4-discovery.net/2014/05/14-working-with-sdram-on-stm32f429-discovery/)
15. [DS1307 - DS1307 RTC modeule for STM32F429 Discovery](https://stm32f4-discovery.net/2014/05/library-15-ds1307-real-time-clock-for-stm32f429-discovery/)
16. [HD44780 - HD44780 Alpha numeric LCD driver for STM32F429 discovery](https://stm32f4-discovery.net/2014/06/library-16-interfacing-hd44780-lcd-controller-with-stm32f429-discovery/)
17. [NRF24L01+ - NRF24L01+ 2.4 GHz transceiver library](https://stm32f4-discovery.net/2014/06/library-17-nrf24l01-stm32f4xx/)
18. [ILI8341 LTDC - ILI9341 LCD with LTDC hardware only for STM32F429](https://stm32f4-discovery.net/2014/06/library-18-ili9341-ltdc-stm32f429-discovery/)
19. [Internal RTC - Library for internal RTC on STM32F4xx devices](https://stm32f4-discovery.net/2014/07/library-19-of-internal-rtc-on-stm32f4xx-devices/)
20. [Independent watchdog - Library for watchdog on STM32F4xx devices](https://stm32f4-discovery.net/2014/07/library-20-independent-watchdog-timer-stm32f4xx/)
21. [FATFS - Read SDcard with STM32F4xx using SDIO or SPI communication](https://stm32f4-discovery.net/2014/07/library-21-read-sd-card-fatfs-stm32f4xx-devices/)
22. [RNG - Use of True random number generator peripheral on STM32F4xx devices](https://stm32f4-discovery.net/2014/07/library-22-true-random-number-generator-stm32f4xx/)
23. [MFRC522 - Read RFID Cards with STM32F4xx devices](https://stm32f4-discovery.net/2014/07/library-23-read-rfid-tag-mfrc522-stm32f4xx-devices/)
24. [USB VCP - Use STM32F4xx as USB VCP device (Virtual COM Port)](https://stm32f4-discovery.net/2014/08/library-24-virtual-com-port-vcp-stm32f4xx/)
25. [AM2301 - AM2301 (DHT21) temperature and humidity sensor](https://stm32f4-discovery.net/2014/08/library-25-am2301-dht21-sensor-stm32f4xx/)
26. [ROTARY ENCODER - Connect Rotary Encoder to STM32F4xx device](https://stm32f4-discovery.net/2014/08/library-26-rotary-encoder-stm32f4/)
27. [GPS - Read GPS data on STM32F4xx devices](https://stm32f4-discovery.net/2014/08/library-27-gps-stm32f4-devices/)
28. [L3GD20 - Measure rotation of all 3 axis with L3GD20 gyro sensor](https://stm32f4-discovery.net/2014/08/library-28-l3gd20-3-axis-gyroscope/)
29. [USB MSC HOST - Read USB flash drive with STM32F4xx](https://stm32f4-discovery.net/2014/08/library-29-usb-msc-host-usb-flash-drive-stm32f4xx-devices/)
30. [HC-SR04 - Measure distance with ultrasonic sensor on STM32F4xx](https://stm32f4-discovery.net/2014/08/library-30-measure-distance-hc-sr04-stm32f4xx/)
31. [USB HID HOST - Read USB mouse and keyboard on STM32F4xx](https://stm32f4-discovery.net/2014/08/library-31-usb-hid-host-for-stm32f4xx-devices/)
32. [4x4 KEYPAD - Read matrix keypad up to 4x4 with STM32F4xx](https://stm32f4-discovery.net/2014/09/library-32-matrix-keypad-stm32f4xx/)
33. [PWM - Use PWM on all possible timers for STM32F4xx device](https://stm32f4-discovery.net/2014/09/library-33-pwm-stm32f4xx/)
34. [USB HID Device - Turn STM32F4 into USB HID Device](https://stm32f4-discovery.net/2014/09/library-34-stm32f4-usb-hid-device/)
35. [LIS302DL/LIS3DSH - Accelerometers on STM32F4-Discovery board. Also can be used with any STM32F4 device](https://stm32f4-discovery.net/2014/09/library-35-lis302dl-or-lis3dsh-accelerometer/)
36. [DAC SIGNAL - Output predefined signals with STM32F4 using DAC, DMA and TIM](https://stm32f4-discovery.net/2014/09/library-36-dac-signal-generator-stm32f4/)
37. [BMP180 - Read atmosferic pressure on STM32F4 device](https://stm32f4-discovery.net/2014/09/library-37-bmp180-pressure-sensor-stm32f4/)
38. [EXTI - Use external interrupts with STM32F4](https://stm32f4-discovery.net/2014/10/library-38-external-interrupts-stm32f4/)
39. [LOW POWER - Power consumption modes for STM32F4](https://stm32f4-discovery.net/2014/10/library-39-power-consumption-modes-for-stm32f4/)
40. [MCO OUTPUT - Output different clocks from STM32F4](https://stm32f4-discovery.net/2014/10/library-40-output-clocks-stm32f4/)
41. [STDIO - Use printf and fprintf/vfprintf on STM32F4](https://stm32f4-discovery.net/2014/10/library-41-stdio-implementation-for-stm32f4/)
42. [SERVO - Control RC servo with STM32F4](https://stm32f4-discovery.net/2014/10/library-42-control-rc-servo-stm32f4/)
43. [MPU-6050 - Read accelerometer and gyroscope data with STM32F4](https://stm32f4-discovery.net/2014/10/library-43-mpu-6050-6-axes-gyro-accelerometer-stm32f4/)
44. [Identification - Read identification bytes and 96-bits long UNIQUE ID from STM32F4](https://stm32f4-discovery.net/2014/12/library-44-stm32f4-unique-id-and-flash-size/)
45. [BKPSRAM - Interface backup SRAM on STM32F4](https://stm32f4-discovery.net/2014/12/library-45-interface-backup-sram-on-stm32f4/)
46. [SWO DEBUG - Debug STM32F4 using SWO output](https://stm32f4-discovery.net/2014/12/library-46-debug-stm32f4-device-swo-feature/)
47. [CRC - STM32F4 CRC Hardware module](https://stm32f4-discovery.net/2014/12/library-47-crc-module-stm32f4/)
48. [PWM IN - Measure input PWM signal on STM32F4](https://stm32f4-discovery.net/2015/01/library-48-measure-pwm-input-signal-stm32f4/)
49. [OTP - One-time programmable memory on STM32F4](https://stm32f4-discovery.net/2015/01/library-49-one-time-programmable-otp-bytes-stm32f4xx/)
50. [STemWin - STemWin implementation for STM32F429-Discovery](https://stm32f4-discovery.net/2015/01/library-50-stemwin-stm32f429-discovery/)
51. [DMA2D GRAPHIC- Chrom-ART Accelerator on STM32F429-Discovery board](https://stm32f4-discovery.net/2015/01/library-51-chrom-art-accelerator-dma2d-graphic-library-on-stm32f429-discovery/)
52. [ETHERNET - Connect ethernet to STM32F4xx](https://stm32f4-discovery.net/2015/02/library-52-ethernet-peripheral-on-stm32f4xx/)
53. [GPIO - GPIO pins on STM32F4](https://stm32f4-discovery.net/2015/03/library-53-gpio-for-stm32f4/)
54. [GENERAL - General library for STM32F4](https://stm32f4-discovery.net/2015/03/library-54-general-library-for-stm32f4xx-devices/)
55. [USART DMA - Extend USART with DMA TX functionality](https://stm32f4-discovery.net/2015/04/library-55-extend-usart-with-tx-dma/)
56. [SPI DMA - Extend SPI with DMA functionality](https://stm32f4-discovery.net/2015/04/library-56-extend-spi-with-dma-for-stm32f4xx/)
57. [BUTTON - Use buttons with STM32F4xx](https://stm32f4-discovery.net/2015/04/library-57-buttons-for-stm32f4xx/)
58. [STRING - Dynamic string allocation for STM32F4xx](https://stm32f4-discovery.net/2015/05/library-58-dynamic-strings-on-stm32f4xx/)
59. [RCC - Dynamic PLL settings for STM32F4xx](https://stm32f4-discovery.net/2015/05/library-59-change-pll-settings-while-stm32f4xx-is-running/)
60. [CPU LOAD - CPU load monitoring for STM32F4xx](https://stm32f4-discovery.net/2015/05/library-60-cpu-load-monitor-for-stm32f4xx-devices/)
61. [SSD1306 - Use OLED LCD with STM32F4](https://stm32f4-discovery.net/2015/05/library-61-ssd1306-oled-i2c-lcd-for-stm32f4xx/)
62. [FFT - FFT Library for STM32F4xx devices](https://stm32f4-discovery.net/2015/05/library-62-fast-fourier-transform-fft-for-stm32f4xx/)
63. [DMA - DMA library for STM32F4xx](https://stm32f4-discovery.net/2015/06/library-63-dma-for-stm32f4xx/)


General library

1. [PCD8544 LCD (Nokia 5110/3310) - Library for LCD](https://stm32f4-discovery.net/pcd8544-nokia-33105110-lcd-stm32f429-discovery-library/)
