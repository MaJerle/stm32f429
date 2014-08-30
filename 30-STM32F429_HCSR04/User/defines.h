#ifndef TM_DEFINES
#define TM_DEFINES

/* Place your custom library settings */

/* Trigger PIN */
/* Uncomment lines, if you want to select your custom pin for TRIGGER */
//#define HCSR04_TRIGGER_RCC		RCC_AHB1Periph_GPIOB
//#define HCSR04_TRIGGER_PORT		GPIOB
//#define HCSR04_TRIGGER_PIN		GPIO_Pin_2

/* Echo PIN */
/* Uncomment lines, if you want to select your custom pin for ECHO */
//#define HCSR04_ECHO_RCC			RCC_AHB1Periph_GPIOB
//#define HCSR04_ECHO_PORT			GPIOB
//#define HCSR04_ECHO_PIN			GPIO_Pin_3

/* Default timeout pulses for  */
/* Uncomment and change only if you know what are you doing */
/* These are clock pulses, how many time we get to sensor to response to our action */
//#define HCSR04_TIMEOUT			1000000

#endif

