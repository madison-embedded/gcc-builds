#ifndef __STM32F7xx_HAL_CONF_H
#define __STM32F7xx_HAL_CONF_H

#include <stdint.h>
#include "hal/stm32f7xx_hal_def.h"

#define HAL_MODULE_ENABLED
/* #define HAL_ADC_MODULE_ENABLED */ 
/* #define HAL_CAN_MODULE_ENABLE */
/* #define HAL_CEC_MODULE_ENABLED */ 
/* #define HAL_CRC_MODULE_ENABLED */ 
/* #define HAL_CRYP_MODULE_ENABLED */ 
/* #define HAL_DAC_MODULE_ENABLED */ 
/* #define HAL_DCMI_MODULE_ENABLED */ 
#define HAL_DMA_MODULE_ENABLED
/* #define HAL_DMA2D_MODULE_ENABLED */
#define HAL_ETH_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
/* #define HAL_NAND_MODULE_ENABLED */ 
/* #define HAL_NOR_MODULE_ENABLED */ 
/* #define HAL_SRAM_MODULE_ENABLED */ 
/* #define HAL_SDRAM_MODULE_ENABLED */ 
/* #define HAL_HASH_MODULE_ENABLED */ 
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
/* #define HAL_I2S_MODULE_ENABLED */
/* #define HAL_IWDG_MODULE_ENABLED */
/* #define HAL_LPTIM_MODULE_ENABLED */
/* #define HAL_LTDC_MODULE_ENABLED */
/* #define HAL_PWR_MODULE_ENABLED */
/* #define HAL_QSPI_MODULE_ENABLED */
#define HAL_RCC_MODULE_ENABLED
/* #define HAL_RNG_MODULE_ENABLED */
/* #define HAL_RTC_MODULE_ENABLED */
/* #define HAL_SAI_MODULE_ENABLED */
/* #define HAL_SD_MODULE_ENABLED */
/* #define HAL_SPDIFRX_MODULE_ENABLED */
/* #define HAL_SPI_MODULE_ENABLED */
/* #define HAL_TIM_MODULE_ENABLED */
/* #define HAL_UART_MODULE_ENABLED */
/* #define HAL_USART_MODULE_ENABLED */
/* #define HAL_IRDA_MODULE_ENABLED */
/* #define HAL_SMARTCARD_MODULE_ENABLED */
/* #define HAL_WWDG_MODULE_ENABLED */
#define HAL_CORTEX_MODULE_ENABLED
/* #define HAL_PCD_MODULE_ENABLED */
/* #define HAL_HCD_MODULE_ENABLED */
/* #define HAL_DFSDM_MODULE_ENABLED */
/* #define HAL_DSI_MODULE_ENABLED */
/* #define HAL_JPEG_MODULE_ENABLED */
/* #define HAL_MDIOS_MODULE_ENABLED */
/* #define HAL_SMBUS_MODULE_ENABLED */
/* #define HAL_MMC_MODULE_ENABLED */

#ifndef	HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT	100U	/* Time out for HSE start up, in ms */
#endif

#ifndef	LSE_STARTUP_TIMEOUT
#define	LSE_STARTUP_TIMEOUT	5000U	/* Time out for LSE start up, in ms */
#endif

/* External clock source for I2S peripheral
 * This value is used by the I2S HAL module to compute the I2S clock source 
 * frequency, this source is inserted directly through I2S_CKIN pad. */
#ifndef	EXTERNAL_CLOCK_VALUE
#define	EXTERNAL_CLOCK_VALUE	12288000U	/* Value of the Internal oscillator in Hz */
#endif

/* ########################### System Configuration ######################### */
#define	VDD_VALUE				3300U	/* Value of VDD in mv */
#define	TICK_INT_PRIORITY		0x0FU	/* tick interrupt priority */
#define	USE_RTOS				0U
#define	PREFETCH_ENABLE			1U
#define	ART_ACCLERATOR_ENABLE	1U		/* To enable instruction cache and prefetch */

/* ########################## Assert Selection ############################## */

/* Uncomment the line below to expanse the "assert_param" macro in the HAL drivers code */
#define USE_FULL_ASSERT	0

/* ################## Ethernet peripheral configuration ##################### */

/* Section 1 : Ethernet peripheral configuration */

/* Definition of the Ethernet driver buffers size and count */   
#define ETH_RX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for receive               */
#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for transmit              */
#define ETH_RXBUFNB                    4U       /* 4 Rx buffers of size ETH_RX_BUF_SIZE  */
#define ETH_TXBUFNB                    4U       /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */

/* STMicro default MAC address */
#define MAC0	0x00
#define MAC1	0x80
#define MAC2	0xE1

/* Section 2: PHY configuration section */

/* DP83848 PHY Address*/ 
#define DP83848_PHY_ADDRESS             0x01U
/* PHY Reset delay these values are based on a 1 ms Systick interrupt*/ 
#define PHY_RESET_DELAY                 0x000000FFU
/* PHY Configuration delay */
#define PHY_CONFIG_DELAY                0x00000FFFU

#define PHY_READ_TO                     0x0000FFFFU
#define PHY_WRITE_TO                    0x0000FFFFU

/* Section 3: Common PHY Registers */

#define PHY_BCR                         ((uint16_t)0x00U)    /*!< Transceiver Basic Control Register   */
#define PHY_BSR                         ((uint16_t)0x01U)    /*!< Transceiver Basic Status Register    */
 
#define PHY_RESET                       ((uint16_t)0x8000U)  /*!< PHY Reset */
#define PHY_LOOPBACK                    ((uint16_t)0x4000U)  /*!< Select loop-back mode */
#define PHY_FULLDUPLEX_100M             ((uint16_t)0x2100U)  /*!< Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M             ((uint16_t)0x2000U)  /*!< Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M              ((uint16_t)0x0100U)  /*!< Set the full-duplex mode at 10 Mb/s  */
#define PHY_HALFDUPLEX_10M              ((uint16_t)0x0000U)  /*!< Set the half-duplex mode at 10 Mb/s  */
#define PHY_AUTONEGOTIATION             ((uint16_t)0x1000U)  /*!< Enable auto-negotiation function     */
#define PHY_RESTART_AUTONEGOTIATION     ((uint16_t)0x0200U)  /*!< Restart auto-negotiation function    */
#define PHY_POWERDOWN                   ((uint16_t)0x0800U)  /*!< Select the power down mode           */
#define PHY_ISOLATE                     ((uint16_t)0x0400U)  /*!< Isolate PHY from MII                 */

#define PHY_AUTONEGO_COMPLETE           ((uint16_t)0x0020U)  /*!< Auto-Negotiation process completed   */
#define PHY_LINKED_STATUS               ((uint16_t)0x0004U)  /*!< Valid link established               */
#define PHY_JABBER_DETECTION            ((uint16_t)0x0002U)  /*!< Jabber condition detected            */
  
/* Section 4: Extended PHY Registers */

#define PHY_SR                          ((uint16_t)0x10U)    /*!< PHY status register Offset                      */
#define PHY_MICR                        ((uint16_t)0x11U)    /*!< MII Interrupt Control Register                  */
#define PHY_MISR                        ((uint16_t)0x12U)    /*!< MII Interrupt Status and Misc. Control Register */
 
#define PHY_LINK_STATUS                 ((uint16_t)0x0001U)  /*!< PHY Link mask                                   */
#define PHY_SPEED_STATUS                ((uint16_t)0x0002U)  /*!< PHY Speed mask                                  */
#define PHY_DUPLEX_STATUS               ((uint16_t)0x0004U)  /*!< PHY Duplex mask                                 */

#define PHY_MICR_INT_EN                 ((uint16_t)0x0002U)  /*!< PHY Enable interrupts                           */
#define PHY_MICR_INT_OE                 ((uint16_t)0x0001U)  /*!< PHY Enable output interrupt events              */

#define PHY_MISR_LINK_INT_EN            ((uint16_t)0x0020U)  /*!< Enable Interrupt on change of link status       */
#define PHY_LINK_INTERRUPT              ((uint16_t)0x2000U)  /*!< PHY link status interrupt mask                  */

/* ################## SPI peripheral configuration ########################## */
/* CRC FEATURE: Use to activate CRC feature inside HAL SPI Driver
 * Activated: CRC code is present inside driver
 * Deactivated: CRC code cleaned from driver */
#define USE_SPI_CRC                     0U

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32f7xx_hal_rcc.h"
#endif
#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32f7xx_hal_gpio.h"
#endif
#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32f7xx_hal_dma.h"
#endif
#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32f7xx_hal_cortex.h"
#endif
#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32f7xx_hal_adc.h"
#endif
#ifdef HAL_CAN_MODULE_ENABLED
#include "stm32f7xx_hal_can.h"
#endif
#ifdef HAL_CEC_MODULE_ENABLED
#include "stm32f7xx_hal_cec.h"
#endif
#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32f7xx_hal_crc.h"
#endif
#ifdef HAL_CRYP_MODULE_ENABLED
#include "stm32f7xx_hal_cryp.h" 
#endif
#ifdef HAL_DMA2D_MODULE_ENABLED
#include "stm32f7xx_hal_dma2d.h"
#endif
#ifdef HAL_DAC_MODULE_ENABLED
#include "stm32f7xx_hal_dac.h"
#endif
#ifdef HAL_DCMI_MODULE_ENABLED
#include "stm32f7xx_hal_dcmi.h"
#endif
#ifdef HAL_ETH_MODULE_ENABLED
#include "stm32f7xx_hal_eth.h"
#endif
#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32f7xx_hal_flash.h"
#endif
#ifdef HAL_SRAM_MODULE_ENABLED
#include "stm32f7xx_hal_sram.h"
#endif
#ifdef HAL_NOR_MODULE_ENABLED
#include "stm32f7xx_hal_nor.h"
#endif
#ifdef HAL_NAND_MODULE_ENABLED
#include "stm32f7xx_hal_nand.h"
#endif
#ifdef HAL_SDRAM_MODULE_ENABLED
#include "stm32f7xx_hal_sdram.h"
#endif
#ifdef HAL_HASH_MODULE_ENABLED
#include "stm32f7xx_hal_hash.h"
#endif
#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32f7xx_hal_i2c.h"
#endif
#ifdef HAL_I2S_MODULE_ENABLED
#include "stm32f7xx_hal_i2s.h"
#endif
#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32f7xx_hal_iwdg.h"
#endif
#ifdef HAL_LPTIM_MODULE_ENABLED
#include "stm32f7xx_hal_lptim.h"
#endif
#ifdef HAL_LTDC_MODULE_ENABLED
#include "stm32f7xx_hal_ltdc.h"
#endif
#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32f7xx_hal_pwr.h"
#endif
#ifdef HAL_QSPI_MODULE_ENABLED
#include "stm32f7xx_hal_qspi.h"
#endif
#ifdef HAL_RNG_MODULE_ENABLED
#include "stm32f7xx_hal_rng.h"
#endif
#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32f7xx_hal_rtc.h"
#endif
#ifdef HAL_SAI_MODULE_ENABLED
#include "stm32f7xx_hal_sai.h"
#endif
#ifdef HAL_SD_MODULE_ENABLED
#include "stm32f7xx_hal_sd.h"
#endif
#ifdef HAL_SPDIFRX_MODULE_ENABLED
#include "stm32f7xx_hal_spdifrx.h"
#endif
#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32f7xx_hal_spi.h"
#endif
#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32f7xx_hal_tim.h"
#endif
#ifdef HAL_UART_MODULE_ENABLED
#include "stm32f7xx_hal_uart.h"
#endif
#ifdef HAL_USART_MODULE_ENABLED
#include "stm32f7xx_hal_usart.h"
#endif
#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32f7xx_hal_irda.h"
#endif
#ifdef HAL_SMARTCARD_MODULE_ENABLED
#include "stm32f7xx_hal_smartcard.h"
#endif
#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32f7xx_hal_wwdg.h"
#endif
#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32f7xx_hal_pcd.h"
#endif
#ifdef HAL_HCD_MODULE_ENABLED
#include "stm32f7xx_hal_hcd.h"
#endif
#ifdef HAL_DFSDM_MODULE_ENABLED
#include "stm32f7xx_hal_dfsdm.h"
#endif
#ifdef HAL_DSI_MODULE_ENABLED
#include "stm32f7xx_hal_dsi.h"
#endif
#ifdef HAL_JPEG_MODULE_ENABLED
#include "stm32f7xx_hal_jpeg.h"
#endif
#ifdef HAL_MDIOS_MODULE_ENABLED
#include "stm32f7xx_hal_mdios.h"
#endif
#ifdef HAL_SMBUS_MODULE_ENABLED
#include "stm32f7xx_hal_smbus.h"
#endif
#ifdef HAL_MMC_MODULE_ENABLED
#include "stm32f7xx_hal_mmc.h"
#endif
   
#ifdef USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif

#endif

