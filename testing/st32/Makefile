###############################################################################
#                               Project Settings                              #
###############################################################################
PROJECT = test
DEFINES = -DSTM32F767xx -DSTM32F7 -D__STARTUP_CLEAR_BSS -D__START=main
###############################################################################


###############################################################################
#                             Toolchain Settings                              #
###############################################################################
TOOLCHAIN=arm-none-eabi-
CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)gcc
OBJCOPY=$(TOOLCHAIN)objcopy
###############################################################################


###############################################################################
#                          Build & Directory Settings                         #
###############################################################################
SRC_DIR = src
SYS_DIR = $(SRC_DIR)/sys
INCLUDES = -I include -I include/cmsis -I include/st
###############################################################################


###############################################################################
#                               Toolchain Flags                               #
###############################################################################
ARCH_FLAGS = -mthumb -mcpu=cortex-m7

CC_FLAGS = -std=gnu99 -ffunction-sections -fdata-sections -Wall -Os -flto $(ARCH_FLAGS) $(INCLUDES) $(DEFINES)
#-Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-exceptions -fno-builtin -funsigned-char -fno-delete-null-pointer-checks -fomit-frame-pointer

LD_FLAGS = --specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(PROJECT).map -T $(LD_SCRIPT)

LD_SCRIPT = script.ld
###############################################################################


###############################################################################
#                               Object Settings                               #
###############################################################################
OBJECTS += $(SRC_DIR)/main.o
#OBJECTS += $(SYS_DIR)/startup_stm32f767xx.o
OBJECTS += $(SYS_DIR)/startup_ARMCM7.o
#OBJECTS += $(SYS_DIR)/system_stm32f7xx.o
###############################################################################


###############################################################################
#                                     Rules                                   #
###############################################################################
%.o: %.s
	$(CC) $(CC_FLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

$(PROJECT).elf: $(OBJECTS)
	$(LD) $^ $(CC_FLAGS) $(LD_FLAGS) -o $@
$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
###############################################################################


###############################################################################
#                             Make Arguments                                  #
###############################################################################
.PHONY: clean
clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).map
	find . -name '*.o' -delete

all: $(PROJECT).bin
###############################################################################

