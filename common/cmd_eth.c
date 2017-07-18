#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli.h"
#include "ethernetif.h"
#include "hal/stm32f7xx_hal.h"

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

void hal_print_status(HAL_StatusTypeDef stat) {
	switch (stat) {
		case HAL_OK: printf("OK"); break;
		case HAL_ERROR: printf("ERROR"); break;
		case HAL_BUSY: printf("BUSY"); break;
		case HAL_TIMEOUT: printf("TIMEOUT"); break;
	}
}

command_status do_eth(int argc, char *argv[]) {

	uint16_t phy_reg;
	uint32_t regVal;
	HAL_StatusTypeDef ret;

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
	else if (!strcmp(argv[1], "status")) printf("Link status: %lu\r\n", EthHandle.LinkStatus);
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

