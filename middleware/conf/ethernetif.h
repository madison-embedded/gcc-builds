#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "hal/stm32f7xx_hal.h"

extern struct netif gnetif;
extern ETH_HandleTypeDef EthHandle;

void ethernetif_input(struct netif *netif);
void Netif_Config(void);

#endif

