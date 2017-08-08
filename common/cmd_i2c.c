#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal/hal_conf.h"
#include "hal/stm32f7xx_hal_i2c.h"
#include "cli.h"

extern I2C_HandleTypeDef hi2c;


char * getStatus(HAL_StatusTypeDef status){

    switch (status){
        case HAL_OK:
            return "OK";
        case HAL_ERROR:
            return "ERROR";
        case HAL_BUSY:
            return "BUSY";
        case HAL_TIMEOUT:
            return "TIMEOUT";
        default:
            return "NOTGOOD";
    }
    return "NOTGOOD";
}

command_status do_i2c(int argc, char *argv[]) {
    uint16_t address;
    uint8_t *pData;
    uint16_t MemAdd;
    uint16_t MemAddSize;

    printf("%s\r\n", getStatus(HAL_I2C_Init(&hi2c)));

    if (argc == 1) {
        for (address = 0; address < 128; address++)
            printf("%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000)));
    }

    if (argc != 6)
        return USAGE;

    address = atoi((const char *) &argv[2]);
    MemAdd = atoi((const char *) &argv[3]);
    MemAddSize = atoi((const char *) &argv[4]);
    pData = (uint8_t *)atoi((const char *) &argv[5]);

    if (strcmp(argv[1], "write") == 0) {
        getStatus(HAL_I2C_Mem_Write(&hi2c, address, MemAdd, MemAddSize, pData, sizeof(pData), 500));
        return SUCCESS;
    }
    if (strcmp(argv[1], "read") == 0) {
        getStatus(HAL_I2C_Mem_Read(&hi2c, address, MemAdd, MemAddSize, pData, sizeof(pData), 500));
        return SUCCESS;
    }
    return USAGE;
}
COMMAND_ENTRY("i2c", "i2c <read | write> <address> <offset> <Memory Address Size> [value]", "Perform live interaction with the i2c devices default list all devices", do_i2c)

