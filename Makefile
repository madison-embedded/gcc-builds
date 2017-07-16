include makefile.conf

###############################################################################
# Default Settings
PROJECT=e2c
.PHONY: clean
.DEFAULT_GOAL := $(PROJECT).bin
PROC_DIR=proc/$(PROCESSOR)
INCLUDES=-I include -I include/cmsis
TERMINAL=gnome-terminal
OBJDUMP_FILE=output.txt
DEFINES := -D__STARTUP_CLEAR_BSS -D__START=main
CORE=CM$(CORTEX_M)
###############################################################################


###############################################################################
# Cross Compiler Settings

FPU = -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

TOOLCHAIN=arm-none-eabi-
CFLAGS=$(FPU) $(ARCH_FLAGS) $(DEFINES) $(CPU_DEFINES) $(INCLUDES) -Wall -ffunction-sections -fdata-sections -fno-builtin -Os 
# -Os -nostdlib -lnosys

# Linker Settings
LFLAGS=--specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(PROJECT).map -T$(PROC_DIR)/link.ld
###############################################################################


###############################################################################
# Global Objects
OBJECTS += common/main.o 
OBJECTS += common/post.o
OBJECTS += common/retarget.o
OBJECTS += common/pcbuffer.o
OBJECTS += common/handlers.o
OBJECTS += common/gpio_alias.o

# Commands
OBJECTS += common/cli.o
OBJECTS += common/cmd_mem.o
OBJECTS += common/cmd_boot.o
OBJECTS += common/cmd_gpio.o
OBJECTS += common/cmd_analog.o
OBJECTS += common/cmd_reset.o
OBJECTS += common/cmd_float.o
OBJECTS += common/cmd_eth.o
OBJECTS += drivers/timer.o

# Conditional Objects
OBJECTS += common/nuc144.o
OBJECTS += $(PROC_DIR)/start.o
OBJECTS += $(PROC_DIR)/system_init.o
OBJECTS += drivers/$(PROC_PREFIX)gpio.o
OBJECTS += drivers/$(PROC_PREFIX)rcc.o
OBJECTS += drivers/$(PROC_PREFIX)usart.o
OBJECTS += drivers/$(PROC_PREFIX)adc.o
OBJECTS += drivers/$(PROC_PREFIX)exti.o
OBJECTS += drivers/$(PROC_PREFIX)tim.o

# HAL Drivers
#OBJECTS += drivers/hal/$(PROC_PREFIX)eth.o
OBJECTS += drivers/hal/stm32f7xx_hal.o
OBJECTS += drivers/hal/stm32f7xx_hal_eth.o
OBJECTS += drivers/hal/stm32f7xx_hal_cortex.o
OBJECTS += drivers/hal/stm32f7xx_hal_flash.o
OBJECTS += drivers/hal/stm32f7xx_hal_rcc.o
OBJECTS += drivers/hal/stm32f7xx_hal_gpio.o

CPUDIR := include/proc
###############################################################################


###############################################################################
# Source Rules
%.o: %.S
	+@echo "building $(notdir $<)"
	@$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

%.o: %.c
	+@echo "building $(notdir $<)"
	@$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

$(PROJECT).elf: $(OBJECTS)
	+@echo "linking $(notdir $@)"
	@$(TOOLCHAIN)gcc $(LFLAGS) $^ $(CFLAGS) -o $@

$(PROJECT).bin: $(PROJECT).elf
	@$(TOOLCHAIN)objcopy -O binary $< $@
	+@echo "Ready to flash $@."

# Project Rules
$(OBJECTS): | $(CPUDIR)

$(OBJDUMP_FILE): $(PROJECT).bin
	$(TOOLCHAIN)objdump -D $(PROJECT).elf > $(OBJDUMP_FILE)

$(CPUDIR):
	+@echo "Creating $@"
	@ln -s ../$(PROC_DIR) $@

clean: 
	@rm -f *.bin *.map *.elf $(CPUDIR) output.txt
	@find . -name '*.o' -delete

install: $(PROJECT).bin
	./$(PROC_DIR)/install.sh

debug: $(PROJECT).bin
	./$(PROC_DIR)/debug.sh

dump: $(OBJDUMP_FILE)
	$(TERMINAL) -e "vim $(OBJDUMP_FILE)"

%_config:
	@echo $@
###############################################################################

