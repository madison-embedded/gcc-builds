#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli.h"
#include "ethernetif.h"
#include "hal/stm32f7xx_hal.h"

#include "lwip/err.h"
#include "lwip/dhcp.h"

command_status do_eth(int argc, char *argv[]) {

	uint16_t phy_reg;
	uint32_t regVal;
	HAL_StatusTypeDef ret;
	err_t lwip_error = ERR_OK;

	if (argc < 2) return USAGE;

	if (!strcmp(argv[1], "phy") && argc == 3) {
		phy_reg = atoi(argv[2]);
		if (EthHandle.Instance != ETH) {
			printf("Ethernet not initialized!\r\n");
			return FAIL;
		}
		if (phy_reg > 31 || phy_reg < 0) {
			printf("invalid phy register %d\r\n", phy_reg);
			return USAGE;
		}
		ret = HAL_ETH_ReadPHYRegister(&EthHandle, phy_reg, &regVal);
		if (ret != HAL_OK) {
			printf("Could not read register! ");
			hal_print_status(ret);
			printf("\r\n");
			return FAIL;
		}
		printf("register %d value: 0x%lx\r\n", phy_reg, regVal & 0xffff);
	}
	else if (!strcmp(argv[1], "init")) Netif_Config();
	else if (!strcmp(argv[1], "check")) {
		/*
		ret = HAL_ETH_GetReceivedFrame(&EthHandle);
		printf("Return status: ");
		hal_print_status(ret);
		printf("\r\n");
		*/
		ethernetif_input(&gnetif);
	}
	else if (!strcmp(argv[1], "status")) {
		//printf("Link status: %lu\r\n", EthHandle.LinkStatus);
		
	}
	else if (!strcmp(argv[1], "dhcp")) {
		lwip_error = dhcp_start(&gnetif);
		printf("return: %s\r\n", lwip_strerr(lwip_error));
	}
	else if (!strcmp(argv[1], "state")) {
		printf("State: ");
		eth_print_status(EthHandle.State);
		printf("\r\n");
	}
	else if (!strcmp(argv[1], "send") && argc >= 3) {
		printf("Not yet implemented\r\n");
		//ret =  HAL_ETH_TransmitFrame(&EthHandle, uint32_t FrameLength);
	}
	else return USAGE;

	return SUCCESS;
}
COMMAND_ENTRY("eth", "eth [phy <reg_num>]", "Debug Ethernet capabilities.", do_eth)


  /** IP address configuration in network byte order */
  //ip_addr_t ip_addr;
  //ip_addr_t netmask;
  //ip_addr_t gw;

  /** This function is called by the network device driver
   *  to pass a packet up the TCP/IP stack. */
  //netif_input_fn input;

  /** This function is called by the IP module when it wants
   *  to send a packet on the interface. This function typically
   *  first resolves the hardware address, then sends the packet.
   *  For ethernet physical layer, this is usually etharp_output() */
  //netif_output_fn output;

  /** This function is called by ethernet_output() when it wants
   *  to send a packet on the interface. This function outputs
   *  the pbuf as-is on the link medium. */
  //netif_linkoutput_fn linkoutput;

#if LWIP_NETIF_STATUS_CALLBACK
  /** This function is called when the netif state is set to up or down */
  //netif_status_callback_fn status_callback;
#endif

#if LWIP_NETIF_LINK_CALLBACK
  /** This function is called when the netif link is set to up or down */
  //netif_status_callback_fn link_callback;
#endif

#if LWIP_NETIF_REMOVE_CALLBACK
  /** This function is called when the netif has been removed */
  //netif_status_callback_fn remove_callback;
#endif

  /** This field can be set by the device driver and could point
   *  to state information for the device. */
  //void *state;

#ifdef netif_get_client_data
  //void* client_data[LWIP_NETIF_CLIENT_DATA_INDEX_MAX + LWIP_NUM_NETIF_CLIENT_DATA];
#endif

#if LWIP_NETIF_HOSTNAME
  /* the hostname for this netif, NULL is a valid value */
  //const char* hostname;
#endif

#if LWIP_CHECKSUM_CTRL_PER_NETIF
  //u16_t chksum_flags;
#endif
  
  //u16_t mtu;			/** maximum transfer unit (in bytes) */
  //u8_t hwaddr_len;	/** number of bytes used in hwaddr */
  //u8_t hwaddr[NETIF_MAX_HWADDR_LEN]; /** link level hardware address of this interface */
  //u8_t flags;			/** flags (@see @ref netif_flags) */
  //char name[2];		/** descriptive abbreviation */
  //u8_t num;			/** number of this interface */

#if LWIP_IPV4 && LWIP_IGMP
  /** This function could be called to add or delete an entry in the multicast
      filter table of the ethernet MAC.*/
  //netif_igmp_mac_filter_fn igmp_mac_filter;
#endif

#if LWIP_NETIF_HWADDRHINT
  //u8_t *addr_hint;
#endif

