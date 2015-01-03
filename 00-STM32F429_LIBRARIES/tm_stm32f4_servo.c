/**	
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
 */
#include "tm_stm32f4_servo.h"

TM_SERVO_Result_t TM_SERVO_Init(TM_SERVO_t* ServoStruct, TIM_TypeDef* TIMx, TM_PWM_Channel_t PWMChannel, TM_PWM_PinsPack_t Pinspack) {
	/* Initialize timer with 50Hz frequency for PWM */
	if (TM_PWM_InitTimer(TIMx, &ServoStruct->PWM, 50) != TM_PWM_Result_Ok) {
		/* Return error */
		return TM_SERVO_Result_Error;
	}
	
	/* Initialize channel */
	if (TM_PWM_InitChannel(&ServoStruct->PWM, PWMChannel, Pinspack) != TM_PWM_Result_Ok) {
		/* Return Error */
		return TM_SERVO_Result_Error;
	}
	
	/* Fill settings */
	ServoStruct->TIM = TIMx;
	ServoStruct->Channel = PWMChannel;
	ServoStruct->Pinspack = Pinspack;
	
	/* Return OK */
	return TM_SERVO_Result_Ok;
}

TM_SERVO_Result_t TM_SERVO_SetDegrees(TM_SERVO_t* ServoStruct, float degrees) {
	/* Set PWM value */
	uint16_t micros;
	
	/* Filter */
	if (degrees < 0 || degrees > 180) {
		/* Return error */
		return TM_SERVO_Result_Error;
	}
	
	/* Generate micros value from degrees */
	micros = (SERVO_MICROS_MAX - SERVO_MICROS_MIN) * degrees / 180 + SERVO_MICROS_MIN;
	
	/* Save micros and degrees values to struct */
	ServoStruct->Micros = micros;
	ServoStruct->Degrees = degrees;
	
	/* Set micros */
	TM_PWM_SetChannelMicros(
		&ServoStruct->PWM,
		ServoStruct->Channel,
		micros
	);
	
	/* Return OK */
	return TM_SERVO_Result_Ok;
}

TM_SERVO_Result_t TM_SERVO_SetMicros(TM_SERVO_t* ServoStruct, uint16_t micros) {
	/* Set PWM value */
	float degrees;
	
	/* Filter */
	if (micros < SERVO_MICROS_MIN || micros > SERVO_MICROS_MAX) {
		/* Return error */
		return TM_SERVO_Result_Error;
	}
	
	/* Generate micros value from degrees */
	degrees = (float)(micros - SERVO_MICROS_MIN) * (float)180 / (float)(SERVO_MICROS_MAX - SERVO_MICROS_MIN);
	
	/* Save micros and degrees values to struct */
	ServoStruct->Micros = micros;
	ServoStruct->Degrees = degrees;
	
	/* Set micros */
	TM_PWM_SetChannelMicros(
		&ServoStruct->PWM,
		ServoStruct->Channel,
		micros
	);
	
	/* Return OK */
	return TM_SERVO_Result_Ok;	
}
