#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli.h"
#include "ethernetif.h"
#include "hal/stm32f7xx_hal.h"

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

