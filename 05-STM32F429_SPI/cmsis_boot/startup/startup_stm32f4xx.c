/**
  ******************************************************************************
  * @file      startup_stm32f4xx.s
  * @author    Coocox / UB (angepasst fuer STM32F429)
  * @version   V1.1
  * @date      10/27/2013
  * @brief     STM32F429 Devices vector table for RIDE7 toolchain. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the clock system and the external SRAM mounted on 
  *                  STM324xG-EVAL board to be used as data memory (optional, 
  *                  to be enabled by user)
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  */
    

/*----------Stack Configuration-----------------------------------------------*/
#define STACK_SIZE       0x00000200      /*!< The Stack size suggest using even number    */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];


/*----------Macro definition--------------------------------------------------*/
#define WEAK __attribute__ ((weak))



/*----------Declaration of the default fault handlers-------------------------*/
/* System exception vector handler */
__attribute__ ((used))
void WEAK  Reset_Handler(void);
void WEAK  NMI_Handler(void);
void WEAK  HardFault_Handler(void);
void WEAK  MemManage_Handler(void);
void WEAK  BusFault_Handler(void);
void WEAK  UsageFault_Handler(void);
void WEAK  SVC_Handler(void);
void WEAK  DebugMon_Handler(void);
void WEAK  PendSV_Handler(void);
void WEAK  SysTick_Handler(void);

void WEAK  WWDG_IRQHandler(void);
void WEAK  PVD_IRQHandler(void);
void WEAK  TAMP_STAMP_IRQHandler(void);
void WEAK  RTC_WKUP_IRQHandler(void);
void WEAK  FLASH_IRQHandler(void);
void WEAK  RCC_IRQHandler(void);
void WEAK  EXTI0_IRQHandler(void);
void WEAK  EXTI1_IRQHandler(void);
void WEAK  EXTI2_IRQHandler(void);
void WEAK  EXTI3_IRQHandler(void);
void WEAK  EXTI4_IRQHandler(void);
void WEAK  DMA1_Stream0_IRQHandler(void);
void WEAK  DMA1_Stream1_IRQHandler(void);
void WEAK  DMA1_Stream2_IRQHandler(void);
void WEAK  DMA1_Stream3_IRQHandler(void);
void WEAK  DMA1_Stream4_IRQHandler(void);
void WEAK  DMA1_Stream5_IRQHandler(void);
void WEAK  DMA1_Stream6_IRQHandler(void);
void WEAK  ADC_IRQHandler(void);
void WEAK  CAN1_TX_IRQHandler(void);
void WEAK  CAN1_RX0_IRQHandler(void);
void WEAK  CAN1_RX1_IRQHandler(void);
void WEAK  CAN1_SCE_IRQHandler(void);
void WEAK  EXTI9_5_IRQHandler(void);
void WEAK  TIM1_BRK_TIM9_IRQHandler(void);
void WEAK  TIM1_UP_TIM10_IRQHandler(void);
void WEAK  TIM1_TRG_COM_TIM11_IRQHandler(void);
void WEAK  TIM1_CC_IRQHandler(void);
void WEAK  TIM2_IRQHandler(void);
void WEAK  TIM3_IRQHandler(void);
void WEAK  TIM4_IRQHandler(void);
void WEAK  I2C1_EV_IRQHandler(void);
void WEAK  I2C1_ER_IRQHandler(void);
void WEAK  I2C2_EV_IRQHandler(void);
void WEAK  I2C2_ER_IRQHandler(void);
void WEAK  SPI1_IRQHandler(void);
void WEAK  SPI2_IRQHandler(void);
void WEAK  USART1_IRQHandler(void);
void WEAK  USART2_IRQHandler(void);
void WEAK  USART3_IRQHandler(void);
void WEAK  EXTI15_10_IRQHandler(void);
void WEAK  RTC_Alarm_IRQHandler(void);
void WEAK  OTG_FS_WKUP_IRQHandler(void);
void WEAK  TIM8_BRK_TIM12_IRQHandler(void);
void WEAK  TIM8_UP_TIM13_IRQHandler(void);
void WEAK  TIM8_TRG_COM_TIM14_IRQHandler(void);
void WEAK  TIM8_CC_IRQHandler(void);
void WEAK  DMA1_Stream7_IRQHandler(void);
void WEAK  FMC_IRQHandler(void);
void WEAK  SDIO_IRQHandler(void);
void WEAK  TIM5_IRQHandler(void);
void WEAK  SPI3_IRQHandler(void);
void WEAK  UART4_IRQHandler(void);
void WEAK  UART5_IRQHandler(void);
void WEAK  TIM6_DAC_IRQHandler(void);
void WEAK  TIM7_IRQHandler(void);
void WEAK  DMA2_Stream0_IRQHandler(void);
void WEAK  DMA2_Stream1_IRQHandler(void);
void WEAK  DMA2_Stream2_IRQHandler(void);
void WEAK  DMA2_Stream3_IRQHandler(void);
void WEAK  DMA2_Stream4_IRQHandler(void);
void WEAK  ETH_IRQHandler(void);
void WEAK  ETH_WKUP_IRQHandler(void);
void WEAK  CAN2_TX_IRQHandler(void);
void WEAK  CAN2_RX0_IRQHandler(void);
void WEAK  CAN2_RX1_IRQHandler(void);
void WEAK  CAN2_SCE_IRQHandler(void);
void WEAK  OTG_FS_IRQHandler(void);
void WEAK  DMA2_Stream5_IRQHandler(void);
void WEAK  DMA2_Stream6_IRQHandler(void);
void WEAK  DMA2_Stream7_IRQHandler(void);
void WEAK  USART6_IRQHandler(void);
void WEAK  I2C3_EV_IRQHandler(void);
void WEAK  I2C3_ER_IRQHandler(void);
void WEAK  OTG_HS_EP1_OUT_IRQHandler(void);
void WEAK  OTG_HS_EP1_IN_IRQHandler(void);
void WEAK  OTG_HS_WKUP_IRQHandler(void);
void WEAK  OTG_HS_IRQHandler(void);
void WEAK  DCMI_IRQHandler(void);
void WEAK  CRYP_IRQHandler(void);
void WEAK  HASH_RNG_IRQHandler(void);
void WEAK  FPU_IRQHandler(void);
// neu für STM32F429
void WEAK  UART7_IRQHandler(void);
void WEAK  UART8_IRQHandler(void);
void WEAK  SPI4_IRQHandler(void);
void WEAK  SPI5_IRQHandler(void);
void WEAK  SPI6_IRQHandler(void);
void WEAK  SAI1_IRQHandler(void); 
void WEAK  LTDC_IRQHandler(void);
void WEAK  LTDC_ER_IRQHandler(void);
void WEAK  DMA2D_IRQHandler(void);
      



/*----------Symbols defined in linker script----------------------------------*/
extern unsigned long _sidata;    /*!< Start address for the initialization
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */
extern unsigned long _edata;     /*!< End address for the .data section       */
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */
extern void _eram;               /*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/
extern int main(void);           /*!< The entry point for the application.    */
extern void SystemInit(void);    /*!< Setup the microcontroller system(CMSIS) */
void Default_Reset_Handler(void);   /*!< Default reset handler                */
static void Default_Handler(void);  /*!< Default exception handler            */


/**
  *@brief The minimal vector table for a Cortex M3.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.
  */
__attribute__ ((used,section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
  /*----------Core Exceptions------------------------------------------------ */
  (void *)&pulStack[STACK_SIZE],     /*!< The initial stack pointer         */
  Reset_Handler,             /*!< Reset Handler                               */
  NMI_Handler,               /*!< NMI Handler                                 */
  HardFault_Handler,         /*!< Hard Fault Handler                          */
  MemManage_Handler,         /*!< MPU Fault Handler                           */
  BusFault_Handler,          /*!< Bus Fault Handler                           */
  UsageFault_Handler,        /*!< Usage Fault Handler                         */
  0,0,0,0,                   /*!< Reserved                                    */
  SVC_Handler,               /*!< SVCall Handler                              */
  DebugMon_Handler,          /*!< Debug Monitor Handler                       */
  0,                         /*!< Reserved                                    */
  PendSV_Handler,            /*!< PendSV Handler                              */
  SysTick_Handler,           /*!< SysTick Handler                             */
  
  /*----------External Exceptions---------------------------------------------*/
  WWDG_IRQHandler,           /*!<  0: Window WatchDog                         */
  PVD_IRQHandler,            /*!<  1: PVD through EXTI Line detection         */
  TAMP_STAMP_IRQHandler,     /*!<  2: Tamper and TimeStamps through the EXTI line*/
  RTC_WKUP_IRQHandler,       /*!<  3: RTC Wakeup through the EXTI line        */
  FLASH_IRQHandler,          /*!<  4: FLASH                                   */
  RCC_IRQHandler  ,          /*!<  5: RCC                                     */
  EXTI0_IRQHandler,          /*!<  6: EXTI Line0                              */
  EXTI1_IRQHandler,          /*!<  7: EXTI Line1                              */
  EXTI2_IRQHandler,          /*!<  8: EXTI Line2                              */
  EXTI3_IRQHandler,          /*!<  9: EXTI Line3                              */
  EXTI4_IRQHandler,          /*!< 10: EXTI Line4                              */
  DMA1_Stream0_IRQHandler,   /*!< 11: DMA1 Stream 0                           */
  DMA1_Stream1_IRQHandler,   /*!< 12: DMA1 Stream 1                           */
  DMA1_Stream2_IRQHandler,   /*!< 13: DMA1 Stream 2                           */
  DMA1_Stream3_IRQHandler,   /*!< 14: DMA1 Stream 3                           */
  DMA1_Stream4_IRQHandler,   /*!< 15: DMA1 Stream 4                           */
  DMA1_Stream5_IRQHandler,   /*!< 16: DMA1 Stream 5                           */
  DMA1_Stream6_IRQHandler,   /*!< 17: DMA1 Stream 6                           */
  ADC_IRQHandler,            /*!< 18: ADC1, ADC2 and ADC3s                    */
  CAN1_TX_IRQHandler,        /*!< 19: CAN1 TX                                 */
  CAN1_RX0_IRQHandler,       /*!< 20: CAN1 RX0                                */
  CAN1_RX1_IRQHandler,       /*!< 21: CAN1 RX1                                */
  CAN1_SCE_IRQHandler,       /*!< 22: CAN1 SCE                                */
  EXTI9_5_IRQHandler,        /*!< 23: External Line[9:5]s                     */
  TIM1_BRK_TIM9_IRQHandler,  /*!< 24: TIM1 Break and TIM9                     */
  TIM1_UP_TIM10_IRQHandler,  /*!< 25: TIM1 Update and TIM10                   */
  TIM1_TRG_COM_TIM11_IRQHandler,/*!< 26: TIM1 Trigger and Commutation and TIM11*/
  TIM1_CC_IRQHandler,        /*!< 27: TIM1 Capture Compare                    */
  TIM2_IRQHandler,           /*!< 28: TIM2                                    */
  TIM3_IRQHandler,           /*!< 29: TIM3                                    */
  TIM4_IRQHandler,           /*!< 30: TIM4                                    */
  I2C1_EV_IRQHandler,        /*!< 31: I2C1 Event                              */
  I2C1_ER_IRQHandler,        /*!< 32: I2C1 Error                              */
  I2C2_EV_IRQHandler,        /*!< 33: I2C2 Event                              */
  I2C2_ER_IRQHandler,        /*!< 34: I2C2 Error                              */
  SPI1_IRQHandler,           /*!< 35: SPI1                                    */
  SPI2_IRQHandler,           /*!< 36: SPI2                                    */
  USART1_IRQHandler,         /*!< 37: USART1                                  */
  USART2_IRQHandler,         /*!< 38: USART2                                  */
  USART3_IRQHandler,         /*!< 39: USART3                                  */
  EXTI15_10_IRQHandler,      /*!< 40: External Line[15:10]s                   */
  RTC_Alarm_IRQHandler,      /*!< 41: RTC Alarm (A and B) through EXTI Line   */
  OTG_FS_WKUP_IRQHandler,    /*!< 42: USB OTG FS Wakeup through EXTI line     */
  TIM8_BRK_TIM12_IRQHandler, /*!< 43: TIM8 Break and TIM12                    */
  TIM8_UP_TIM13_IRQHandler,  /*!< 44: TIM8 Update and TIM13                   */
  TIM8_TRG_COM_TIM14_IRQHandler,/*!< 45:TIM8 Trigger and Commutation and TIM14*/
  TIM8_CC_IRQHandler,        /*!< 46: TIM8 Capture Compare                    */
  DMA1_Stream7_IRQHandler,   /*!< 47: DMA1 Stream7                            */
  FMC_IRQHandler,            /*!< 48: FMC                                     */
  SDIO_IRQHandler,           /*!< 49: SDIO                                    */
  TIM5_IRQHandler,           /*!< 50: TIM5                                    */
  SPI3_IRQHandler,           /*!< 51: SPI3                                    */
  UART4_IRQHandler,          /*!< 52: UART4                                   */
  UART5_IRQHandler,          /*!< 53: UART5                                   */
  TIM6_DAC_IRQHandler,       /*!< 54: TIM6 and DAC1&2 underrun errors         */
  TIM7_IRQHandler,           /*!< 55: TIM7                                    */
  DMA2_Stream0_IRQHandler,   /*!< 56: DMA2 Stream 0                           */
  DMA2_Stream1_IRQHandler,   /*!< 57: DMA2 Stream 1                           */
  DMA2_Stream2_IRQHandler,   /*!< 58: DMA2 Stream 2                           */
  DMA2_Stream3_IRQHandler,   /*!< 59: DMA2 Stream 3                           */
  DMA2_Stream4_IRQHandler,   /*!< 60: DMA2 Stream 4                           */
  ETH_IRQHandler,            /*!< 61: Ethernet                                */
  ETH_WKUP_IRQHandler,       /*!< 62: Ethernet Wakeup through EXTI line       */
  CAN2_TX_IRQHandler,        /*!< 63: CAN2 TX                                 */
  CAN2_RX0_IRQHandler,       /*!< 64: CAN2 RX0                                */
  CAN2_RX1_IRQHandler,       /*!< 65: CAN2 RX1                                */
  CAN2_SCE_IRQHandler,       /*!< 66: CAN2 SCE                                */
  OTG_FS_IRQHandler,         /*!< 67: USB OTG FS                              */
  DMA2_Stream5_IRQHandler,   /*!< 68: DMA2 Stream 5                           */
  DMA2_Stream6_IRQHandler,   /*!< 69: DMA2 Stream 6                           */
  DMA2_Stream7_IRQHandler,   /*!< 70: DMA2 Stream 7                           */
  USART6_IRQHandler,         /*!< 71: USART6                                  */
  I2C3_EV_IRQHandler,        /*!< 72: I2C3 event                              */
  I2C3_ER_IRQHandler,        /*!< 73: I2C3 error                              */
  OTG_HS_EP1_OUT_IRQHandler, /*!< 74: USB OTG HS End Point 1 Out              */
  OTG_HS_EP1_IN_IRQHandler,  /*!< 75: USB OTG HS End Point 1 In               */
  OTG_HS_WKUP_IRQHandler,    /*!< 76: USB OTG HS Wakeup through EXTI          */
  OTG_HS_IRQHandler,         /*!< 77: USB OTG HS                              */
  DCMI_IRQHandler,           /*!< 53: DCMI                                    */
  CRYP_IRQHandler,           /*!< 53: CRYP crypto                             */
  HASH_RNG_IRQHandler,       /*!< 53: Hash and Rng                            */
  FPU_IRQHandler,            /*!< 53: FPU                                     */
  UART7_IRQHandler,          /*!< ??: UART7 (STM32F429)                       */ 
  UART8_IRQHandler,          /*!< ??: UART8 (STM32F429)                       */
  SPI4_IRQHandler,           /*!< ??: SPI4 (STM32F429)                        */
  SPI5_IRQHandler,           /*!< ??: SPI5 (STM32F429)                        */
  SPI6_IRQHandler,           /*!< ??: SPI6 (STM32F429)                        */
  SAI1_IRQHandler,           /*!< ??: SAI1 (STM32F429)                        */
  LTDC_IRQHandler,           /*!< ??: LTDC (STM32F429)                        */
  LTDC_ER_IRQHandler,        /*!< ??: LTDC_ER (STM32F429)                     */
  DMA2D_IRQHandler           /*!< ??: DMA2D (STM32F429)                       */
};


/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called.
  * @param  None
  * @retval None
  */
void Default_Reset_Handler(void)
{
  /* Initialize data and bss */
  unsigned long *pulSrc, *pulDest;

  /* Copy the data segment initializers from flash to SRAM */
  pulSrc = &_sidata;

  for(pulDest = &_sdata; pulDest < &_edata; )
  {
    *(pulDest++) = *(pulSrc++);
  }
  
  /* Zero fill the bss segment.  This is done with inline assembly since this
     will clear the value of pulDest if it is not kept in a register. */
  __asm("  ldr     r0, =_sbss\n"
        "  ldr     r1, =_ebss\n"
        "  mov     r2, #0\n"
        "  .thumb_func\n"
        "zero_loop:\n"
        "    cmp     r0, r1\n"
        "    it      lt\n"
        "    strlt   r2, [r0], #4\n"
        "    blt     zero_loop");
#ifdef __FPU_USED
  /* Enable FPU.*/ 
  __asm("  LDR.W R0, =0xE000ED88\n"
        "  LDR R1, [R0]\n"
        "  ORR R1, R1, #(0xF << 20)\n"
        "  STR R1, [R0]");
#endif	

  //Configure CLOCK
  //SystemInit();
 
  /* Call the application's entry point.*/
  main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler.
  *       As they are weak aliases, any function with the same name will override
  *       this definition.
  */
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler

#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak PVD_IRQHandler = Default_Handler
#pragma weak TAMP_STAMP_IRQHandler = Default_Handler
#pragma weak RTC_WKUP_IRQHandler = Default_Handler
#pragma weak FLASH_IRQHandler = Default_Handler
#pragma weak RCC_IRQHandler = Default_Handler
#pragma weak EXTI0_IRQHandler = Default_Handler
#pragma weak EXTI1_IRQHandler = Default_Handler
#pragma weak EXTI2_IRQHandler = Default_Handler
#pragma weak EXTI3_IRQHandler = Default_Handler
#pragma weak EXTI4_IRQHandler = Default_Handler
#pragma weak DMA1_Stream0_IRQHandler = Default_Handler
#pragma weak DMA1_Stream1_IRQHandler = Default_Handler
#pragma weak DMA1_Stream2_IRQHandler = Default_Handler
#pragma weak DMA1_Stream3_IRQHandler = Default_Handler
#pragma weak DMA1_Stream4_IRQHandler = Default_Handler
#pragma weak DMA1_Stream5_IRQHandler = Default_Handler
#pragma weak DMA1_Stream6_IRQHandler = Default_Handler
#pragma weak ADC_IRQHandler = Default_Handler
#pragma weak CAN1_TX_IRQHandler = Default_Handler
#pragma weak CAN1_RX0_IRQHandler = Default_Handler
#pragma weak CAN1_RX1_IRQHandler = Default_Handler
#pragma weak CAN1_SCE_IRQHandler = Default_Handler
#pragma weak EXTI9_5_IRQHandler = Default_Handler
#pragma weak TIM1_BRK_TIM9_IRQHandler = Default_Handler
#pragma weak TIM1_UP_TIM10_IRQHandler = Default_Handler
#pragma weak TIM1_TRG_COM_TIM11_IRQHandler = Default_Handler
#pragma weak TIM1_CC_IRQHandler = Default_Handler
#pragma weak TIM2_IRQHandler = Default_Handler
#pragma weak TIM3_IRQHandler = Default_Handler
#pragma weak TIM4_IRQHandler = Default_Handler
#pragma weak I2C1_EV_IRQHandler = Default_Handler
#pragma weak I2C1_ER_IRQHandler = Default_Handler
#pragma weak I2C2_EV_IRQHandler = Default_Handler
#pragma weak I2C2_ER_IRQHandler = Default_Handler
#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak USART1_IRQHandler = Default_Handler
#pragma weak USART2_IRQHandler = Default_Handler
#pragma weak USART3_IRQHandler = Default_Handler
#pragma weak EXTI15_10_IRQHandler = Default_Handler
#pragma weak RTC_Alarm_IRQHandler = Default_Handler
#pragma weak OTG_FS_WKUP_IRQHandler = Default_Handler
#pragma weak TIM8_BRK_TIM12_IRQHandler = Default_Handler
#pragma weak TIM8_UP_TIM13_IRQHandler = Default_Handler
#pragma weak TIM8_TRG_COM_TIM14_IRQHandler = Default_Handler
#pragma weak TIM8_CC_IRQHandler = Default_Handler
#pragma weak DMA1_Stream7_IRQHandler = Default_Handler
#pragma weak FMC_IRQHandler = Default_Handler
#pragma weak SDIO_IRQHandler = Default_Handler
#pragma weak TIM5_IRQHandler = Default_Handler
#pragma weak SPI3_IRQHandler = Default_Handler
#pragma weak UART4_IRQHandler = Default_Handler
#pragma weak UART5_IRQHandler = Default_Handler
#pragma weak TIM6_DAC_IRQHandler = Default_Handler
#pragma weak TIM7_IRQHandler = Default_Handler
#pragma weak DMA2_Stream0_IRQHandler = Default_Handler
#pragma weak DMA2_Stream1_IRQHandler = Default_Handler
#pragma weak DMA2_Stream2_IRQHandler = Default_Handler
#pragma weak DMA2_Stream3_IRQHandler = Default_Handler
#pragma weak DMA2_Stream4_IRQHandler = Default_Handler
#pragma weak ETH_IRQHandler = Default_Handler
#pragma weak ETH_WKUP_IRQHandler = Default_Handler
#pragma weak CAN2_TX_IRQHandler = Default_Handler
#pragma weak CAN2_RX0_IRQHandler = Default_Handler
#pragma weak CAN2_RX1_IRQHandler = Default_Handler
#pragma weak CAN2_SCE_IRQHandler = Default_Handler
#pragma weak OTG_FS_IRQHandler = Default_Handler
#pragma weak DMA2_Stream5_IRQHandler = Default_Handler
#pragma weak DMA2_Stream6_IRQHandler = Default_Handler
#pragma weak DMA2_Stream7_IRQHandler = Default_Handler
#pragma weak USART6_IRQHandler = Default_Handler
#pragma weak I2C3_EV_IRQHandler = Default_Handler
#pragma weak I2C3_ER_IRQHandler = Default_Handler
#pragma weak OTG_HS_EP1_OUT_IRQHandler = Default_Handler
#pragma weak OTG_HS_EP1_IN_IRQHandler = Default_Handler
#pragma weak OTG_HS_WKUP_IRQHandler = Default_Handler
#pragma weak OTG_HS_IRQHandler = Default_Handler
#pragma weak DCMI_IRQHandler = Default_Handler
#pragma weak CRYP_IRQHandler = Default_Handler
#pragma weak HASH_RNG_IRQHandler = Default_Handler
#pragma weak FPU_IRQHandler = Default_Handler
// neu für STM32F429 
#pragma weak UART7_IRQHandler = Default_Handler
#pragma weak UART8_IRQHandler = Default_Handler
#pragma weak SPI4_IRQHandler = Default_Handler
#pragma weak SPI5_IRQHandler = Default_Handler
#pragma weak SPI6_IRQHandler = Default_Handler
#pragma weak SAI1_IRQHandler = Default_Handler
#pragma weak LTDC_IRQHandler = Default_Handler
#pragma weak LTDC_ER_IRQHandler = Default_Handler
#pragma weak DMA2D_IRQHandler = Default_Handler



/**
  * @brief  This is the code that gets called when the processor receives an
  *         unexpected interrupt.  This simply enters an infinite loop,
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None
  */
static void Default_Handler(void)
{
	/* Go into an infinite loop. */
	while (1)
	{
	}
}

/*********************** (C) COPYRIGHT 2009 Coocox ************END OF FILE*****/
