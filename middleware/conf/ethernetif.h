#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "hal/stm32f7xx_hal.h"

#define LINK_UP_CHECK_TIME	5000

extern struct netif gnetif;
extern ETH_HandleTypeDef EthHandle;

void ethernetif_input(struct netif *netif);
void Netif_Config(void);

void eth_print_status(HAL_ETH_StateTypeDef stat);
inline void lwip_loop_handler(void);

#endif

