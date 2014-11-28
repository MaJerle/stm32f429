/**
 *	Keil project example for 2 MPU6050 6-axes sensors
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
	TM_MPU6050_t MPU6050_Data0;
	TM_MPU6050_t MPU6050_Data1;
	uint8_t sensor1 = 0, sensor2 = 0;
	char str[120];
	
	/* Initialize system */
	SystemInit();

	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize USART, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize MPU6050 sensor 0, address = 0xD0, AD0 pin on sensor is low */
	if (TM_MPU6050_Init(&MPU6050_Data0, TM_MPU6050_Device_0, TM_MPU6050_Accelerometer_8G, TM_MPU6050_Gyroscope_250s) == TM_MPU6050_Result_Ok) {
		/* Display message to user */
		TM_USART_Puts(USART1, "MPU6050 sensor 0 is ready to use!\n");
		
		/* Sensor 1 OK */
		sensor1 = 1;
	}
	
	/* Initialize MPU6050 sensor 1, address = 0xD2, AD0 pin on sensor is high */
	if (TM_MPU6050_Init(&MPU6050_Data1, TM_MPU6050_Device_1, TM_MPU6050_Accelerometer_8G, TM_MPU6050_Gyroscope_250s) == TM_MPU6050_Result_Ok) {
		/* Display message to user */
		TM_USART_Puts(USART1, "MPU6050 sensor 1 is ready to use!\n");
		
		/* Sensor 2 OK */
		sensor2 = 1;
	}
	
	while (1) {
		/* Every 500ms */
		if (TM_DELAY_Time() >= 500) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			
			/* If sensor 1 is connected */
			if (sensor1) {
				/* Read all data from sensor 1 */
				TM_MPU6050_ReadAll(&MPU6050_Data0);
				
				/* Format data */
				sprintf(str, "1. Accelerometer\n- X:%d\n- Y:%d\n- Z:%d\nGyroscope\n- X:%d\n- Y:%d\n- Z:%d\nTemperature\n- %3.4f\n\n\n",
					MPU6050_Data0.Accelerometer_X,
					MPU6050_Data0.Accelerometer_Y,
					MPU6050_Data0.Accelerometer_Z,
					MPU6050_Data0.Gyroscope_X,
					MPU6050_Data0.Gyroscope_Y,
					MPU6050_Data0.Gyroscope_Z,
					MPU6050_Data0.Temperature
				);
				
				/* Show to usart */
				TM_USART_Puts(USART1, str);
			}
			
			/* If sensor 2 is connected */
			if (sensor2) {
				/* Read all data from sensor 1 */
				TM_MPU6050_ReadAll(&MPU6050_Data1);
				
				/* Format data */
				sprintf(str, "2. Accelerometer\n- X:%d\n- Y:%d\n- Z:%d\nGyroscope\n- X:%d\n- Y:%d\n- Z:%d\nTemperature\n- %3.4f\n\n\n",
					MPU6050_Data1.Accelerometer_X,
					MPU6050_Data1.Accelerometer_Y,
					MPU6050_Data1.Accelerometer_Z,
					MPU6050_Data1.Gyroscope_X,
					MPU6050_Data1.Gyroscope_Y,
					MPU6050_Data1.Gyroscope_Z,
					MPU6050_Data1.Temperature
				);
				
				/* Show to usart */
				TM_USART_Puts(USART1, str);
			}
		}
	}
}

