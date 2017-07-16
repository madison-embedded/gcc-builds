#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli.h"
#include "hal/stm32f7xx_hal.h"

extern ETH_HandleTypeDef EthHandle;
extern void eth_init(void);

command_status do_eth(int argc, char *argv[]) {

	uint16_t phy_reg;
	uint32_t regVal;
	HAL_StatusTypeDef ret;

	if (argc < 2) return USAGE;

	if (!strcmp(argv[1], "phy") && argc == 3) {
		phy_reg = atoi(argv[2]);
		if (phy_reg > 31 || phy_reg < 0) {
			printf("invalid phy register %d\r\n", phy_reg);
			return USAGE;
		}
		ret = HAL_ETH_ReadPHYRegister(&EthHandle, phy_reg, &regVal);
		if (ret != HAL_OK) {
			printf("Could not read register! (%d)\r\n", ret);
			return FAIL;
		}
		printf("register %d value: 0x%lx\r\n", phy_reg, regVal & 0xffff);
	}
	else if (!strcmp(argv[1], "init")) eth_init();
	else return USAGE;

	return SUCCESS;
}
COMMAND_ENTRY("eth", "eth [phy <reg_num>]", "Debug Ethernet capabilities.", do_eth)

