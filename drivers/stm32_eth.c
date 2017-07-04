#include "eth.h"
#define ETH_TIMEOUT_SWRESET                 ((uint32_t)500)
#define ETH_TIMEOUT_LINKED_STATE          ((uint32_t)5000)
#define ETH_TIMEOUT_AUTONEGO_COMPLETED    ((uint32_t)5000)
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


/**
  * @}
  */
/* Private functions ---------------------------------------------------------*/

/** @defgroup ETH_Exported_Functions ETH Exported Functions
  * @{
  */

/** @defgroup ETH_Exported_Functions_Group1 Initialization and de-initialization functions 
  *  @brief   Initialization and Configuration functions 
  *
  @verbatim    
  ===============================================================================
            ##### Initialization and de-initialization functions #####
  ===============================================================================
  [..]  This section provides functions allowing to:
      (+) Initialize and configure the Ethernet peripheral
      (+) De-initialize the Ethernet peripheral

  @endverbatim
  * @{
  */

/**
  * @brief  Initializes the Ethernet MAC and DMA according to default
  *         parameters.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
StatusTypeDef ETH_Init(ETH_HandleTypeDef *heth)
{
  uint32_t tempreg = 0, phyreg = 0;
  uint32_t hclk = 60000000;
  uint32_t tickstart = 0;
  uint32_t err = ETH_SUCCESS;
  
  /* Check the ETH peripheral state */
  if(heth == NULL)
  {
    return ERROR;
  }
  
  /* Check parameters */
  assert_param(IS_ETH_AUTONEGOTIATION(heth->Init.AutoNegotiation));
  assert_param(IS_ETH_RX_MODE(heth->Init.RxMode));
  assert_param(IS_ETH_CHECKSUM_MODE(heth->Init.ChecksumMode));
  assert_param(IS_ETH_MEDIA_INTERFACE(heth->Init.MediaInterface));  
  
  if(heth->State == ETH_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    heth->Lock = UNLOCKED;
    /* Init the low level hardware : GPIO, CLOCK, NVIC. */
    ETH_MspInit(heth);
  }
  
  /* Enable SYSCFG Clock */
  __RCC_SYSCFG_CLK_ENABLE();
  
  /* Select MII or RMII Mode*/
  SYSCFG->PMC &= ~(SYSCFG_PMC_MII_RMII_SEL);
  SYSCFG->PMC |= (uint32_t)heth->Init.MediaInterface;
  
  /* Ethernet Software reset */
  /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
  /* After reset all the registers holds their respective reset values */
  (heth->Instance)->DMABMR |= ETH_DMABMR_SR;
  
  /* Get tick */
  tickstart = GetTick();
  
  /* Wait for software reset */
  while (((heth->Instance)->DMABMR & ETH_DMABMR_SR) != (uint32_t)RESET)
  {
    /* Check for the Timeout */
    if((GetTick() - tickstart ) > ETH_TIMEOUT_SWRESET)
    {     
      heth->State= ETH_STATE_TIMEOUT;
  
      /* Process Unlocked */
      __UNLOCK(heth);
    
      /* Note: The SWR is not performed if the ETH_RX_CLK or the ETH_TX_CLK are  
         not available, please check your external PHY or the IO configuration */
               
      return TIMEOUT;
    }
  }
  
  /*-------------------------------- MAC Initialization ----------------------*/
  /* Get the ETHERNET MACMIIAR value */
  tempreg = (heth->Instance)->MACMIIAR;
  /* Clear CSR Clock Range CR[2:0] bits */
  tempreg &= ETH_MACMIIAR_CR_MASK;
  
  /* Get hclk frequency value */
  hclk = RCC_GetHCLKFreq();
  
  /* Set CR bits depending on hclk value */
  if((hclk >= 20000000)&&(hclk < 35000000))
  {
    /* CSR Clock Range between 20-35 MHz */
    tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div16;
  }
  else if((hclk >= 35000000)&&(hclk < 60000000))
  {
    /* CSR Clock Range between 35-60 MHz */ 
    tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div26;
  }  
  else if((hclk >= 60000000)&&(hclk < 100000000))
  {
    /* CSR Clock Range between 60-100 MHz */ 
    tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;
  }  
  else if((hclk >= 100000000)&&(hclk < 150000000))
  {
    /* CSR Clock Range between 100-150 MHz */ 
    tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div62;
  }
  else /* ((hclk >= 150000000)&&(hclk <= 216000000)) */
  {
    /* CSR Clock Range between 150-216 MHz */ 
    tempreg |= (uint32_t)ETH_MACMIIAR_CR_Div102;    
  }
  
  /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
  (heth->Instance)->MACMIIAR = (uint32_t)tempreg;
  
  /*-------------------- PHY initialization and configuration ----------------*/
  /* Put the PHY in reset mode */
  if((ETH_WritePHYRegister(heth, PHY_BCR, PHY_RESET)) != OK)
  {
    /* In case of write timeout */
    err = ETH_ERROR;
    
    /* Config MAC and DMA */
    ETH_MACDMAConfig(heth, err);
    
    /* Set the ETH peripheral state to READY */
    heth->State = ETH_STATE_READY;
    
    /* Return ERROR */
    return ERROR;
  }
  
  /* Delay to assure PHY reset */
  Delay(PHY_RESET_DELAY);
  
  if((heth->Init).AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE)
  {
    /* Get tick */
    tickstart = GetTick();
    
    /* We wait for linked status */
    do
    {
      ETH_ReadPHYRegister(heth, PHY_BSR, &phyreg);
      
      /* Check for the Timeout */
      if((GetTick() - tickstart ) > ETH_TIMEOUT_LINKED_STATE)
      {
        /* In case of write timeout */
        err = ETH_ERROR;
      
        /* Config MAC and DMA */
        ETH_MACDMAConfig(heth, err);
        
        heth->State= ETH_STATE_READY;
  
        /* Process Unlocked */
        __UNLOCK(heth);
    
        return TIMEOUT;
      }
    } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));

    
    /* Enable Auto-Negotiation */
    if((ETH_WritePHYRegister(heth, PHY_BCR, PHY_AUTONEGOTIATION)) != OK)
    {
      /* In case of write timeout */
      err = ETH_ERROR;
      
      /* Config MAC and DMA */
      ETH_MACDMAConfig(heth, err);
      
      /* Set the ETH peripheral state to READY */
      heth->State = ETH_STATE_READY;
      
      /* Return ERROR */
      return ERROR;   
    }
    
    /* Get tick */
    tickstart = GetTick();
    
    /* Wait until the auto-negotiation will be completed */
    do
    {
      ETH_ReadPHYRegister(heth, PHY_BSR, &phyreg);
      
      /* Check for the Timeout */
      if((GetTick() - tickstart ) > ETH_TIMEOUT_AUTONEGO_COMPLETED)
      {
        /* In case of write timeout */
        err = ETH_ERROR;
      
        /* Config MAC and DMA */
        ETH_MACDMAConfig(heth, err);
        
        heth->State= ETH_STATE_READY;
  
        /* Process Unlocked */
        __UNLOCK(heth);
    
        return TIMEOUT;
      }
      
    } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));
    
    /* Read the result of the auto-negotiation */
    if((ETH_ReadPHYRegister(heth, PHY_SR, &phyreg)) != OK)
    {
      /* In case of write timeout */
      err = ETH_ERROR;
      
      /* Config MAC and DMA */
      ETH_MACDMAConfig(heth, err);
      
      /* Set the ETH peripheral state to READY */
      heth->State = ETH_STATE_READY;
      
      /* Return ERROR */
      return ERROR;   
    }
    
    /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
    if((phyreg & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
    {
      /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
      (heth->Init).DuplexMode = ETH_MODE_FULLDUPLEX;  
    }
    else
    {
      /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
      (heth->Init).DuplexMode = ETH_MODE_HALFDUPLEX;           
    }
    /* Configure the MAC with the speed fixed by the auto-negotiation process */
    if((phyreg & PHY_SPEED_STATUS) == PHY_SPEED_STATUS)
    {  
      /* Set Ethernet speed to 10M following the auto-negotiation */
      (heth->Init).Speed = ETH_SPEED_10M; 
    }
    else
    {   
      /* Set Ethernet speed to 100M following the auto-negotiation */ 
      (heth->Init).Speed = ETH_SPEED_100M;
    }
  }
  else /* AutoNegotiation Disable */
  {
    /* Check parameters */
    assert_param(IS_ETH_SPEED(heth->Init.Speed));
    assert_param(IS_ETH_DUPLEX_MODE(heth->Init.DuplexMode));
    
    /* Set MAC Speed and Duplex Mode */
    if(ETH_WritePHYRegister(heth, PHY_BCR, ((uint16_t)((heth->Init).DuplexMode >> 3) |
                                                (uint16_t)((heth->Init).Speed >> 1))) != OK)
    {
      /* In case of write timeout */
      err = ETH_ERROR;
      
      /* Config MAC and DMA */
      ETH_MACDMAConfig(heth, err);
      
      /* Set the ETH peripheral state to READY */
      heth->State = ETH_STATE_READY;
      
      /* Return ERROR */
      return ERROR;
    }  
    
    /* Delay to assure PHY configuration */
    Delay(PHY_CONFIG_DELAY);
  }
  
  /* Config MAC and DMA */
  ETH_MACDMAConfig(heth, err);
  
  /* Set ETH HAL State to Ready */
  heth->State= ETH_STATE_READY;
  
  /* Return function status */
  return OK;
}


/**
  * @brief  Writes to a PHY register.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module  
  * @param  PHYReg: PHY register address, is the index of one of the 32 PHY register. 
  *          This parameter can be one of the following values: 
  *             PHY_BCR: Transceiver Control Register.  
  *             More PHY register could be written depending on the used PHY
  * @param  RegValue: the value to write
  * @retval HAL status
  */
StatusTypeDef ETH_WritePHYRegister(ETH_HandleTypeDef *heth, uint16_t PHYReg, uint32_t RegValue)
{
  uint32_t tmpreg = 0;
  uint32_t tickstart = 0;
  
  /* Check parameters */
  assert_param(IS_ETH_PHY_ADDRESS(heth->Init.PhyAddress));
  
  /* Check the ETH peripheral state */
  if(heth->State == ETH_STATE_BUSY_WR)
  {
    return BUSY;
  }
  /* Set ETH HAL State to BUSY_WR */
  heth->State = ETH_STATE_BUSY_WR;
  
  /* Get the ETHERNET MACMIIAR value */
  tmpreg = heth->Instance->MACMIIAR;
  
  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= ~ETH_MACMIIAR_CR_MASK;
  
  /* Prepare the MII register address value */
  tmpreg |=(((uint32_t)heth->Init.PhyAddress<<11) & ETH_MACMIIAR_PA); /* Set the PHY device address */
  tmpreg |=(((uint32_t)PHYReg<<6) & ETH_MACMIIAR_MR);                 /* Set the PHY register address */
  tmpreg |= ETH_MACMIIAR_MW;                                          /* Set the write mode */
  tmpreg |= ETH_MACMIIAR_MB;                                          /* Set the MII Busy bit */
  
  /* Give the value to the MII data register */
  heth->Instance->MACMIIDR = (uint16_t)RegValue;
  
  /* Write the result value into the MII Address register */
  heth->Instance->MACMIIAR = tmpreg;
  
  /* Get tick */
  tickstart = GetTick();
  
  /* Check for the Busy flag */
  while((tmpreg & ETH_MACMIIAR_MB) == ETH_MACMIIAR_MB)
  {
    /* Check for the Timeout */
    if((GetTick() - tickstart ) > PHY_WRITE_TO)
    {
      heth->State= ETH_STATE_READY;
  
      /* Process Unlocked */
      __UNLOCK(heth);
    
      return TIMEOUT;
    }
    
    tmpreg = heth->Instance->MACMIIAR;
  }
  
  /* Set ETH HAL State to READY */
  heth->State = ETH_STATE_READY;
  
  /* Return function status */
  return OK; 
}

/**
  * @brief  Reads a PHY register
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module                  
  * @param PHYReg: PHY register address, is the index of one of the 32 PHY register. 
  *                This parameter can be one of the following values: 
  *                   PHY_BCR: Transceiver Basic Control Register, 
  *                   PHY_BSR: Transceiver Basic Status Register.   
  *                   More PHY register could be read depending on the used PHY
  * @param RegValue: PHY register value                  
  * @retval HAL status
  */
StatusTypeDef ETH_ReadPHYRegister(ETH_HandleTypeDef *heth, uint16_t PHYReg, uint32_t *RegValue)
{
  uint32_t tmpreg = 0;     
  uint32_t tickstart = 0;
  
  /* Check parameters */
  assert_param(IS_ETH_PHY_ADDRESS(heth->Init.PhyAddress));
  
  /* Check the ETH peripheral state */
  if(heth->State == ETH_STATE_BUSY_RD)
  {
    return BUSY;
  }
  /* Set ETH HAL State to BUSY_RD */
  heth->State = ETH_STATE_BUSY_RD;
  
  /* Get the ETHERNET MACMIIAR value */
  tmpreg = heth->Instance->MACMIIAR;
  
  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg &= ~ETH_MACMIIAR_CR_MASK;
  
  /* Prepare the MII address register value */
  tmpreg |=(((uint32_t)heth->Init.PhyAddress << 11) & ETH_MACMIIAR_PA); /* Set the PHY device address   */
  tmpreg |=(((uint32_t)PHYReg<<6) & ETH_MACMIIAR_MR);                   /* Set the PHY register address */
  tmpreg &= ~ETH_MACMIIAR_MW;                                           /* Set the read mode            */
  tmpreg |= ETH_MACMIIAR_MB;                                            /* Set the MII Busy bit         */
  
  /* Write the result value into the MII Address register */
  heth->Instance->MACMIIAR = tmpreg;
  
  /* Get tick */
  tickstart = GetTick();
  
  /* Check for the Busy flag */
  while((tmpreg & ETH_MACMIIAR_MB) == ETH_MACMIIAR_MB)
  {
    /* Check for the Timeout */
    if((GetTick() - tickstart ) > PHY_READ_TO)
    {
      heth->State= ETH_STATE_READY;
  
      /* Process Unlocked */
      __UNLOCK(heth);
    
      return TIMEOUT;
    }
    
    tmpreg = heth->Instance->MACMIIAR;
  }
  
  /* Get MACMIIDR value */
  *RegValue = (uint16_t)(heth->Instance->MACMIIDR);
  
  /* Set ETH HAL State to READY */
  heth->State = ETH_STATE_READY;
  
  /* Return function status */
  return OK;
}
	
