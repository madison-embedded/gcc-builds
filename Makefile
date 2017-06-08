include makefile.conf

###############################################################################
# Default Settings
PROJECT=e2c
.PHONY: clean
.DEFAULT_GOAL := $(PROJECT).bin
PROC_DIR=proc/$(PROCESSOR)
INCLUDES=-I include -I include/cmsis
DEFINES=-D__STARTUP_CLEAR_BSS -D__START=main
CORE=CM$(CORTEX_M)
###############################################################################


###############################################################################
# Cross Compiler Settings
TOOLCHAIN=arm-none-eabi-
ARCH_FLAGS=-mthumb -mcpu=cortex-m$(CORTEX_M)
CFLAGS=$(ARCH_FLAGS) $(DEFINES) $(CPU_DEFINES) $(INCLUDES) -Wall -ffunction-sections -fdata-sections
# -Os -flto -nostdlib

# Linker Settings
LFLAGS=--specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(PROJECT).map -T$(PROC_DIR)/link.ld
###############################################################################


###############################################################################
# Global Objects
OBJECTS += common/main.o 
OBJECTS += common/cli.o
OBJECTS += common/pcbuffer.o
OBJECTS += drivers/timer.o

# Conditional Objects
OBJECTS += common/nuc144.o
OBJECTS += $(PROC_DIR)/start.o
OBJECTS += $(PROC_DIR)/system_init.o
OBJECTS += drivers/$(PROC_PREFIX)gpio.o
OBJECTS += drivers/$(PROC_PREFIX)rcc.o
OBJECTS += drivers/$(PROC_PREFIX)usart.o

CPUDIR := include/proc
###############################################################################


###############################################################################
# Source Rules
%.o: %.S
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

$(PROJECT).elf: $(OBJECTS)
	$(TOOLCHAIN)gcc $^ $(CFLAGS) $(LFLAGS) -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(TOOLCHAIN)objcopy -O binary $< $@

# Project Rules
$(OBJECTS): | $(CPUDIR)

$(CPUDIR):
	ln -s ../$(PROC_DIR) $@

clean: 
	rm -f *.bin *.map *.elf $(CPUDIR) output.txt
	find . -name '*.o' -delete

install: $(PROJECT).bin
	./$(PROC_DIR)/install.sh

dump: $(PROJECT).bin
	$(TOOLCHAIN)objdump -D $(PROJECT).elf > output.txt
	# TODO: automatically open this? Make other rules (i.e. install) create this as well?

%_config:
	@echo $@
###############################################################################

