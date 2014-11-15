/**
 *	Keil project example for MPU6050 6-axes
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_mpu6050.h"

#include <stdio.h>

int main(void) {
	TM_MPU6050_t MPU6050_Data;
	char str[120];
	
	/* Initialize system */
	SystemInit();

	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize USART, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize MPU6050 sensor */
	if (TM_MPU6050_Init(&MPU6050_Data, TM_MPU6050_Device_0, TM_MPU6050_Accelerometer_8G, TM_MPU6050_Gyroscope_2000s) != TM_MPU6050_Result_Ok) {
		/* Display error to user */
		TM_USART_Puts(USART1, "MPU6050 Error\n");
		
		/* Infinite loop */
		while (1);
	}
	
	while (1) {
		/* Read all data from sensor */
		TM_MPU6050_ReadAll(&MPU6050_Data);
		
		/* Format data */		
		sprintf(str, "Accelerometer\n- X:%d\n- Y:%d\n- Z:%d\nGyroscope\n- X:%d\n- Y:%d\n- Z:%d\nTemperature\n- %3.4f\n\n\n",
			MPU6050_Data.Accelerometer_X,
			MPU6050_Data.Accelerometer_Y,
			MPU6050_Data.Accelerometer_Z,
			MPU6050_Data.Gyroscope_X,
			MPU6050_Data.Gyroscope_Y,
			MPU6050_Data.Gyroscope_Z,
			MPU6050_Data.Temperature
		);
		
		/* Show to usart */
		TM_USART_Puts(USART1, str);
		
		/* Little delay */
		Delayms(500);
	}
}
