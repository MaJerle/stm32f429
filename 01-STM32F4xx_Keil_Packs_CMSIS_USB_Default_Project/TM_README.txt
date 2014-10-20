This is project template for 3 STM32F4xx series boards:

- STM32F429-Discovery
- STM32F4-Discovery
- Nucleo F401

Projects are set, to be compatible with my libraries
but set for USB speed. 
For that purpose, STM32F429-Discovery board has to be slown down
from 180MHz to 168MHz, to get proper 48MHz for USB speed.

From now, October 21, 2014 I will try to Create objects for each board separatelly,
so if you don't have Keil compiler, you can still download .hex file and use it,
upload with ST-link utility or any other uploading program for ST.

Files are split into different directories.

Folder structures:

- _STM32F429_Discovery_Outputs
	Output files for STM32F429-Discovery board. You need stm32f429_discovery_project.hex 
	if you want to upload file to your board
- _STM32F4_Discovery_Outputs
	Output files for STM32F429-Discovery board. You need stm32f4_discovery_project.hex 
	if you want to upload file to your board
- _Nucleo_F401RE_Outputs
	Output files for Nucleo F401 board. You need nucleo_f401_project.hex 
	if you want to upload file to your board
- RTE 
	Keil uVision files
- User
	User files, there are main.c, defines.h and other files
- TM
	Folder, where you store libraries, which have you download from my site.