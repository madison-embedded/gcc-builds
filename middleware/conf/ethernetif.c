#include "hal/stm32f7xx_hal.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include <string.h>

/* Global Ethernet handle*/
ETH_HandleTypeDef EthHandle;

__ALIGN_BEGIN ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END; /* Ethernet Rx DMA Descriptor */
__ALIGN_BEGIN ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END; /* Ethernet Tx DMA Descriptor */
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif) {
	uint32_t word0 = *(uint32_t *) UID_BASE;
    uint8_t MACAddr[6];
    HAL_StatusTypeDef hal_eth_init_status;

    MACAddr[0] = MAC0;
    MACAddr[1] = MAC1;
    MACAddr[2] = MAC2;
    MACAddr[3] = (word0 & 0x00ff0000) >> 16;
    MACAddr[4] = (word0 & 0x0000ff00) >> 8;
    MACAddr[5] = (word0 & 0x000000ff);

    /* Init ETH */
    EthHandle.Instance = ETH;
    EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    EthHandle.Init.Speed = ETH_SPEED_100M;
    EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    EthHandle.Init.PhyAddress = 0x0;
    EthHandle.Init.MACAddr = MACAddr;
    EthHandle.Init.RxMode = ETH_RXPOLLING_MODE;
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

	printf("Initializing ethernet . . . ");
	fflush(stdout);

    hal_eth_init_status = HAL_ETH_Init(&EthHandle);

	if (hal_eth_init_status != HAL_OK) {
		printf("failed: ");
		eth_print_status(hal_eth_init_status);
		printf("\r\n\n");
		return;
	}
	printf("success!\r\n\n");

	/* Set netif link flag */
	netif->flags |= NETIF_FLAG_LINK_UP;

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	/* set netif MAC hardware address length */
	netif->hwaddr_len = ETH_HWADDR_LEN;

	/* set netif MAC hardware address */
	netif->hwaddr[0] =  MACAddr[0];
	netif->hwaddr[1] =  MACAddr[1];
	netif->hwaddr[2] =  MACAddr[2];
	netif->hwaddr[3] =  MACAddr[3];
	netif->hwaddr[4] =  MACAddr[4];
	netif->hwaddr[5] =  MACAddr[5];

	/* set netif maximum transfer unit */
	netif->mtu = 1500;

	/* Accept broadcast address and ARP traffic */
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&EthHandle);
}

/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p) {
	err_t errval;
	struct pbuf *q;
	uint8_t *buffer = (uint8_t *)(EthHandle.TxDesc->Buffer1Addr);
	__IO ETH_DMADescTypeDef *DmaTxDesc;
	uint32_t framelength = 0, bufferoffset = 0;
	uint32_t byteslefttocopy = 0, payloadoffset = 0;

	DmaTxDesc = EthHandle.TxDesc;
	bufferoffset = 0;

	/* copy frame from pbufs to driver buffers */
	for (q = p; q != NULL; q = q->next) {

		/* Is this buffer available? If not, goto error */
		if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t) RESET) {
			errval = ERR_USE;
			goto error;
		}

		/* Get bytes in current lwIP buffer */
		byteslefttocopy = q->len;
		payloadoffset = 0;

		/* Check if the length of data to copy is bigger than Tx buffer size*/
		while((byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE) {

			/* Copy data to Tx buffer*/
			memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

			/* Point to next descriptor */
			DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

			/* Check if the buffer is available */
			if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET) {
				errval = ERR_USE;
				goto error;
			}

			buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);

			byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
			payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
			framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
			bufferoffset = 0;
		}

		/* Copy the remaining bytes */
		memcpy((uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy);
		bufferoffset = bufferoffset + byteslefttocopy;
		framelength = framelength + byteslefttocopy;
	}

	/* Prepare transmit descriptors to give to DMA */ 
	HAL_ETH_TransmitFrame(&EthHandle, framelength);

	errval = ERR_OK;
  
error:
	/* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
	if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t) RESET) {
		EthHandle.Instance->DMASR = ETH_DMASR_TUS; /* Clear TUS ETHERNET DMA flag */
		EthHandle.Instance->DMATPDR = 0; /* Resume DMA transmission*/
	}

	return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf * low_level_input(struct netif *netif) {
	struct pbuf *p = NULL, *q = NULL;
	uint16_t len = 0;
	uint8_t *buffer;
	__IO ETH_DMADescTypeDef *dmarxdesc;
	uint32_t bufferoffset = 0, payloadoffset = 0;
	uint32_t byteslefttocopy = 0, i = 0;

	/* get received frame */
	if (HAL_ETH_GetReceivedFrame_IT(&EthHandle) != HAL_OK) return NULL;

	/* Obtain the size of the packet and put it into the "len" variable. */
	len = EthHandle.RxFrameInfos.length;
	buffer = (uint8_t *)EthHandle.RxFrameInfos.buffer;

	/* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
	if (len > 0)
		p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	if (p != NULL) {
		dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
		bufferoffset = 0;

		for (q = p; q != NULL; q = q->next) {
			byteslefttocopy = q->len;
			payloadoffset = 0;

			/* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
			while((byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE) {
				/* Copy data to pbuf */
				memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

				/* Point to next descriptor */
				dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
				buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);

				byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
				payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
				bufferoffset = 0;
			}

			/* Copy remaining data in pbuf */
			memcpy((uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
			bufferoffset = bufferoffset + byteslefttocopy;
		}
	}

	/* Release descriptors to DMA */
	/* Point to first descriptor */
	dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;

	/* Set Own bit in Rx descriptors: gives the buffers back to DMA */
	for (i=0; i< EthHandle.RxFrameInfos.SegCount; i++) {  
		dmarxdesc->Status |= ETH_DMARXDESC_OWN;
		dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
	}

	/* Clear Segment_Count */
	EthHandle.RxFrameInfos.SegCount = 0;

	/* When Rx Buffer unavailable flag is set: clear it and resume reception */
	if ((EthHandle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t) RESET) {
		/* Clear RBUS ETHERNET DMA flag */
		EthHandle.Instance->DMASR = ETH_DMASR_RBUS;
		/* Resume DMA reception */
		EthHandle.Instance->DMARPDR = 0;
	}

	return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(struct netif *netif) {
	struct pbuf *p;

	/* move received packet into a new pbuf */
	p = low_level_input(netif);

	/* if no packet could be read, silently ignore this */
	if (p != NULL) {

		/* pass all packets to ethernet_input, which decides what packets it supports */
		if (netif->input(p, netif) != ERR_OK) {
			LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
			pbuf_free(p);
			p = NULL;
		}
	}
}

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif) {

	LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	/* We directly use etharp_output() here to save a function call.
	* You can instead declare your own function an call etharp_output()
	* from it if you have to do some checks before sending (e.g. if link
	* is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;

	/* initialize the hardware */
	low_level_init(netif);

	return ERR_OK;
}

/* Initializes the lwIP stack */
struct netif gnetif;
void Netif_Config(void) {
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	/* need to use DHCP */
	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);

	/* add the network interface */    
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &netif_input);

	/*  Registers the default network interface. */
	netif_set_default(&gnetif);

	/* When the netif is fully configured this function must be called.*/
	if (netif_is_link_up(&gnetif)) netif_set_up(&gnetif);

	/* When the netif link is down this function must be called */
	else netif_set_down(&gnetif);
}

void HAL_ETH_MspInit(ETH_HandleTypeDef* heth) {

    GPIO_InitTypeDef GPIO_InitStructure;
    if (heth->Instance == ETH) {

        /* Enable GPIOs clocks */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
         */
        /* Configure PA1, PA2 and PA7 */
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL; 
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure PB13 */
        GPIO_InitStructure.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure PC1, PC4 and PC5 */
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* Configure PG2, PG11 and PG13 */
        GPIO_InitStructure.Pin =  GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

        /* Enable the Ethernet global Interrupt */
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        
        /* Enable ETHERNET clock  */
        __HAL_RCC_ETH_CLK_ENABLE();
    }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth) {

    if (heth->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13);

        /* Disable the Ethernet global Interrupt */
        NVIC_DisableIRQ(ETH_IRQn);
    }
}

void eth_print_status(HAL_ETH_StateTypeDef stat) {
	switch (stat) {
		case HAL_ETH_STATE_RESET: printf("RESET"); break;
		case HAL_ETH_STATE_READY: printf("READY"); break;
		case HAL_ETH_STATE_BUSY: printf("BUSY"); break;
		case HAL_ETH_STATE_BUSY_RX: printf("BUSY_RX"); break;
		case HAL_ETH_STATE_BUSY_TX: printf("BUSY_TX"); break;
		case HAL_ETH_STATE_BUSY_TX_RX: printf("BUSY_TX_RX"); break;
		case HAL_ETH_STATE_BUSY_WR: printf("BUSY_WR"); break;
		case HAL_ETH_STATE_BUSY_RD: printf("BUSY_RD"); break;
		case HAL_ETH_STATE_TIMEOUT: printf("TIMEOUT"); break;
		case HAL_ETH_STATE_ERROR: printf("ERROR"); break;
	}
}

sys_prot_t sys_arch_protect(void) { return 0; }
void sys_arch_unprotect(sys_prot_t pval) { UNUSED(pval); }

uint32_t sys_now(void) { return ticks; }

void assert_printf(char *msg, int line, char *file) {
	printf("%s %d: %s\r\n", file, line, msg);
}
