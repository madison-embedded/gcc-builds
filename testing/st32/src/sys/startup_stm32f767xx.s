	.syntax unified
	.cpu cortex-m7
	.fpu softvfp
	.thumb

	.global g_pfnVectors
	.global Default_Handler

	.word  _sidata	/* start address for the initialization values of the .data section */
	.word  _sdata	/* start address for the .data section	*/
	.word  _edata	/* end address for the .data section	*/
	.word  _sbss	/* start address for the .bss section	*/
	.word  _ebss	/* end address for the .bss section		*/

/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/*
 * This is the code that gets called when the processor first
 * starts execution following a reset event. Only the absolutely
 * necessary set is performed, after which the application
 * supplied main() routine is called. 
 */
	.section .text.Reset_Handler
	.weak Reset_Handler
	.type Reset_Handler, %function
Reset_Handler:  
	ldr   sp, =_estack      /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */  
	movs  r1, #0
	b  LoopCopyDataInit

CopyDataInit:
	ldr  r3, =_sidata
	ldr  r3, [r3, r1]
	str  r3, [r0, r1]
	adds  r1, r1, #4
    
LoopCopyDataInit:
	ldr  r0, =_sdata
	ldr  r3, =_edata
	adds  r2, r0, r1
	cmp  r2, r3
	bcc  CopyDataInit
	ldr  r2, =_sbss
	b  LoopFillZerobss

/* Zero fill the bss segment. */  
FillZerobss:
	movs  r3, #0
	str  r3, [r2], #4
    
LoopFillZerobss:
	ldr  r3, = _ebss
	cmp  r2, r3
	bcc  FillZerobss
/* Call the clock system initialization function.*/
	bl  SystemInit   
/* Call static constructors */
	bl __libc_init_array
/* Call the application's entry point.*/
	bl  main
	bx  lr    

	.size  Reset_Handler, .-Reset_Handler


/* Gets called when the processor receives an 
 * unexpected interrupt. Enters an infinite loop, preserving
 * the system state. */
	.section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b  Infinite_Loop
	.size  Default_Handler, .-Default_Handler


/******************************************************************************
* The minimal vector table for a Cortex M7. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*******************************************************************************/
	.section  .isr_vector,"a",%progbits
	.type  g_pfnVectors, %object
	.size  g_pfnVectors, .-g_pfnVectors
   
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler

  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     WWDG_IRQHandler                   /* Window WatchDog              */
  .word     PVD_IRQHandler                    /* PVD through EXTI Line detection */
  .word     TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */
  .word     RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */
  .word     FLASH_IRQHandler                  /* FLASH                        */
  .word     RCC_IRQHandler                    /* RCC                          */
  .word     EXTI0_IRQHandler                  /* EXTI Line0                   */
  .word     EXTI1_IRQHandler                  /* EXTI Line1                   */
  .word     EXTI2_IRQHandler                  /* EXTI Line2                   */
  .word     EXTI3_IRQHandler                  /* EXTI Line3                   */
  .word     EXTI4_IRQHandler                  /* EXTI Line4                   */
  .word     DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */
  .word     DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */
  .word     DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */
  .word     DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */
  .word     DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */
  .word     DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */
  .word     DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */
  .word     ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */
  .word     CAN1_TX_IRQHandler                /* CAN1 TX                      */
  .word     CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
  .word     CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
  .word     CAN1_SCE_IRQHandler               /* CAN1 SCE                     */
  .word     EXTI9_5_IRQHandler                /* External Line[9:5]s          */
  .word     TIM1_BRK_TIM9_IRQHandler          /* TIM1 Break and TIM9          */
  .word     TIM1_UP_TIM10_IRQHandler          /* TIM1 Update and TIM10        */
  .word     TIM1_TRG_COM_TIM11_IRQHandler     /* TIM1 Trigger and Commutation and TIM11 */
  .word     TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
  .word     TIM2_IRQHandler                   /* TIM2                         */
  .word     TIM3_IRQHandler                   /* TIM3                         */
  .word     TIM4_IRQHandler                   /* TIM4                         */
  .word     I2C1_EV_IRQHandler                /* I2C1 Event                   */
  .word     I2C1_ER_IRQHandler                /* I2C1 Error                   */
  .word     I2C2_EV_IRQHandler                /* I2C2 Event                   */
  .word     I2C2_ER_IRQHandler                /* I2C2 Error                   */
  .word     SPI1_IRQHandler                   /* SPI1                         */
  .word     SPI2_IRQHandler                   /* SPI2                         */
  .word     USART1_IRQHandler                 /* USART1                       */
  .word     USART2_IRQHandler                 /* USART2                       */
  .word     USART3_IRQHandler                 /* USART3                       */
  .word     EXTI15_10_IRQHandler              /* External Line[15:10]s        */
  .word     RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
  .word     OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI line */
  .word     TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */
  .word     TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */
  .word     TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
  .word     TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */
  .word     DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */
  .word     FMC_IRQHandler                    /* FMC                          */
  .word     SDMMC1_IRQHandler                 /* SDMMC1                       */
  .word     TIM5_IRQHandler                   /* TIM5                         */
  .word     SPI3_IRQHandler                   /* SPI3                         */
  .word     UART4_IRQHandler                  /* UART4                        */
  .word     UART5_IRQHandler                  /* UART5                        */
  .word     TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */
  .word     TIM7_IRQHandler                   /* TIM7                         */
  .word     DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */
  .word     DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */
  .word     DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */
  .word     DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */
  .word     DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */
  .word     ETH_IRQHandler                    /* Ethernet                     */
  .word     ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line */
  .word     CAN2_TX_IRQHandler                /* CAN2 TX                      */
  .word     CAN2_RX0_IRQHandler               /* CAN2 RX0                     */
  .word     CAN2_RX1_IRQHandler               /* CAN2 RX1                     */
  .word     CAN2_SCE_IRQHandler               /* CAN2 SCE                     */
  .word     OTG_FS_IRQHandler                 /* USB OTG FS                   */
  .word     DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */
  .word     DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */
  .word     DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */
  .word     USART6_IRQHandler                 /* USART6                       */
  .word     I2C3_EV_IRQHandler                /* I2C3 event                   */
  .word     I2C3_ER_IRQHandler                /* I2C3 error                   */
  .word     OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */
  .word     OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */
  .word     OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */
  .word     OTG_HS_IRQHandler                 /* USB OTG HS                   */
  .word     DCMI_IRQHandler                   /* DCMI                         */
  .word     0                                 /* Reserved                     */
  .word     RNG_IRQHandler                    /* RNG                          */
  .word     FPU_IRQHandler                    /* FPU                          */
  .word     UART7_IRQHandler                  /* UART7                        */
  .word     UART8_IRQHandler                  /* UART8                        */
  .word     SPI4_IRQHandler                   /* SPI4                         */
  .word     SPI5_IRQHandler                   /* SPI5                         */
  .word     SPI6_IRQHandler                   /* SPI6                         */
  .word     SAI1_IRQHandler                   /* SAI1                         */
  .word     LTDC_IRQHandler                   /* LTDC                         */
  .word     LTDC_ER_IRQHandler                /* LTDC error                   */
  .word     DMA2D_IRQHandler                  /* DMA2D                        */
  .word     SAI2_IRQHandler                   /* SAI2                         */
  .word     QUADSPI_IRQHandler                /* QUADSPI                      */
  .word     LPTIM1_IRQHandler                 /* LPTIM1                       */
  .word     CEC_IRQHandler                    /* HDMI_CEC                     */
  .word     I2C4_EV_IRQHandler                /* I2C4 Event                   */
  .word     I2C4_ER_IRQHandler                /* I2C4 Error                   */
  .word     SPDIF_RX_IRQHandler               /* SPDIF_RX                     */
  .word     0                                 /* Reserved                     */
  .word     DFSDM1_FLT0_IRQHandler            /* DFSDM1 Filter 0 global Interrupt */
  .word     DFSDM1_FLT1_IRQHandler            /* DFSDM1 Filter 1 global Interrupt */
  .word     DFSDM1_FLT2_IRQHandler            /* DFSDM1 Filter 2 global Interrupt */
  .word     DFSDM1_FLT3_IRQHandler            /* DFSDM1 Filter 3 global Interrupt */
  .word     SDMMC2_IRQHandler                 /* SDMMC2                       */
  .word     CAN3_TX_IRQHandler                /* CAN3 TX                      */
  .word     CAN3_RX0_IRQHandler               /* CAN3 RX0                     */
  .word     CAN3_RX1_IRQHandler               /* CAN3 RX1                     */
  .word     CAN3_SCE_IRQHandler               /* CAN3 SCE                     */
  .word     JPEG_IRQHandler                   /* JPEG                         */
  .word     MDIOS_IRQHandler                  /* MDIOS                        */
  
/*******************************************************************************
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
*******************************************************************************/

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	def_irq_handler NMI_Handler
	def_irq_handler	HardFault_Handler
	def_irq_handler	MemManage_Handler
	def_irq_handler	BusFault_Handler
	def_irq_handler	UsageFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	DebugMon_Handler
	def_irq_handler	PendSV_Handler
	def_irq_handler	SysTick_Handler
	def_irq_handler	WWDG_IRQHandler
	def_irq_handler PVD_IRQHandler      
	def_irq_handler TAMP_STAMP_IRQHandler            
	def_irq_handler RTC_WKUP_IRQHandler                  
	def_irq_handler FLASH_IRQHandler         
	def_irq_handler RCC_IRQHandler      
	def_irq_handler EXTI0_IRQHandler         
	def_irq_handler EXTI1_IRQHandler         
	def_irq_handler EXTI2_IRQHandler         
	def_irq_handler EXTI3_IRQHandler         
	def_irq_handler EXTI4_IRQHandler         
	def_irq_handler DMA1_Stream0_IRQHandler               
	def_irq_handler DMA1_Stream1_IRQHandler               
	def_irq_handler DMA1_Stream2_IRQHandler               
	def_irq_handler DMA1_Stream3_IRQHandler               
	def_irq_handler DMA1_Stream4_IRQHandler              
	def_irq_handler DMA1_Stream5_IRQHandler               
	def_irq_handler DMA1_Stream6_IRQHandler               
	def_irq_handler ADC_IRQHandler      
	def_irq_handler CAN1_TX_IRQHandler   
	def_irq_handler CAN1_RX0_IRQHandler                  
	def_irq_handler CAN1_RX1_IRQHandler                  
	def_irq_handler CAN1_SCE_IRQHandler                  
	def_irq_handler EXTI9_5_IRQHandler   
	def_irq_handler TIM1_BRK_TIM9_IRQHandler            
	def_irq_handler TIM1_UP_TIM10_IRQHandler            
	def_irq_handler TIM1_TRG_COM_TIM11_IRQHandler      
	def_irq_handler TIM1_CC_IRQHandler   
	def_irq_handler TIM2_IRQHandler            
	def_irq_handler TIM3_IRQHandler            
	def_irq_handler TIM4_IRQHandler            
	def_irq_handler I2C1_EV_IRQHandler   
	def_irq_handler I2C1_ER_IRQHandler   
	def_irq_handler I2C2_EV_IRQHandler   
	def_irq_handler I2C2_ER_IRQHandler   
	def_irq_handler SPI1_IRQHandler            
	def_irq_handler SPI2_IRQHandler            
	def_irq_handler USART1_IRQHandler      
	def_irq_handler USART2_IRQHandler      
	def_irq_handler USART3_IRQHandler      
	def_irq_handler EXTI15_10_IRQHandler               
	def_irq_handler RTC_Alarm_IRQHandler               
	def_irq_handler OTG_FS_WKUP_IRQHandler         
	def_irq_handler TIM8_BRK_TIM12_IRQHandler         
	def_irq_handler TIM8_UP_TIM13_IRQHandler            
	def_irq_handler TIM8_TRG_COM_TIM14_IRQHandler      
	def_irq_handler TIM8_CC_IRQHandler   
	def_irq_handler DMA1_Stream7_IRQHandler               
	def_irq_handler FMC_IRQHandler            
	def_irq_handler SDMMC1_IRQHandler            
	def_irq_handler TIM5_IRQHandler            
	def_irq_handler SPI3_IRQHandler            
	def_irq_handler UART4_IRQHandler         
	def_irq_handler UART5_IRQHandler         
	def_irq_handler TIM6_DAC_IRQHandler                  
	def_irq_handler TIM7_IRQHandler            
	def_irq_handler DMA2_Stream0_IRQHandler               
	def_irq_handler DMA2_Stream1_IRQHandler               
	def_irq_handler DMA2_Stream2_IRQHandler               
	def_irq_handler DMA2_Stream3_IRQHandler               
	def_irq_handler DMA2_Stream4_IRQHandler               
	def_irq_handler ETH_IRQHandler   
	def_irq_handler ETH_WKUP_IRQHandler   
	def_irq_handler CAN2_TX_IRQHandler   
	def_irq_handler CAN2_RX0_IRQHandler                  
	def_irq_handler CAN2_RX1_IRQHandler                  
	def_irq_handler CAN2_SCE_IRQHandler                  
	def_irq_handler OTG_FS_IRQHandler      
	def_irq_handler DMA2_Stream5_IRQHandler               
	def_irq_handler DMA2_Stream6_IRQHandler               
	def_irq_handler DMA2_Stream7_IRQHandler               
	def_irq_handler USART6_IRQHandler      
	def_irq_handler I2C3_EV_IRQHandler   
	def_irq_handler I2C3_ER_IRQHandler   
	def_irq_handler OTG_HS_EP1_OUT_IRQHandler         
	def_irq_handler OTG_HS_EP1_IN_IRQHandler            
	def_irq_handler OTG_HS_WKUP_IRQHandler         
	def_irq_handler OTG_HS_IRQHandler      
	def_irq_handler DCMI_IRQHandler            
	def_irq_handler RNG_IRQHandler            
	def_irq_handler FPU_IRQHandler                  
	def_irq_handler UART7_IRQHandler                  
	def_irq_handler UART8_IRQHandler                  
	def_irq_handler SPI4_IRQHandler            
	def_irq_handler SPI5_IRQHandler            
	def_irq_handler SPI6_IRQHandler            
	def_irq_handler SAI1_IRQHandler            
	def_irq_handler LTDC_IRQHandler            
	def_irq_handler LTDC_ER_IRQHandler            
	def_irq_handler DMA2D_IRQHandler            
	def_irq_handler SAI2_IRQHandler            
	def_irq_handler QUADSPI_IRQHandler            
	def_irq_handler LPTIM1_IRQHandler            
	def_irq_handler CEC_IRQHandler            
	def_irq_handler I2C4_EV_IRQHandler            
	def_irq_handler I2C4_ER_IRQHandler            
	def_irq_handler SPDIF_RX_IRQHandler            
	def_irq_handler DFSDM1_FLT0_IRQHandler            
	def_irq_handler DFSDM1_FLT1_IRQHandler            
	def_irq_handler DFSDM1_FLT2_IRQHandler            
	def_irq_handler DFSDM1_FLT3_IRQHandler            
	def_irq_handler SDMMC2_IRQHandler            
	def_irq_handler CAN3_TX_IRQHandler            
	def_irq_handler CAN3_RX0_IRQHandler            
	def_irq_handler CAN3_RX1_IRQHandler            
	def_irq_handler CAN3_SCE_IRQHandler            
	def_irq_handler JPEG_IRQHandler            
	def_irq_handler MDIOS_IRQHandler            
 
