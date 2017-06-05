###############################################################################
# Project Settings
PROJECT=e2c
CORTEX_M=7
CORE=CM$(CORTEX_M)
PROCESSOR=stm32f767zi
.PHONY: clean
.DEFAULT_GOAL := $(PROJECT).bin
###############################################################################


###############################################################################
# Configuration Definitions
DEFINES=-D__STARTUP_CLEAR_BSS -D__START=main -DSTM32F7 -DSTM32F767xx
INCLUDES=-I include -I include/cmsis
###############################################################################


###############################################################################
# Cross Compiler Settings
TOOLCHAIN=arm-none-eabi-
ARCH_FLAGS=-mthumb -mcpu=cortex-m$(CORTEX_M)
CFLAGS=$(ARCH_FLAGS) $(DEFINES) $(INCLUDES) -Wall -Os -flto -ffunction-sections -fdata-sections

# Linker Settings
LFLAGS=--specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(PROJECT).map -Tproc/$(PROCESSOR)/link.ld
###############################################################################


###############################################################################
# Objects
OBJECTS += common/main.o 
OBJECTS += proc/$(PROCESSOR)/start.o

CPUDIR := include/proc
###############################################################################


###############################################################################
# Rules
$(OBJECTS): | $(CPUDIR)

$(CPUDIR):
	ln -s ../proc/$(PROCESSOR) $@

%.o: %.S
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

$(PROJECT).elf: $(OBJECTS)
	$(TOOLCHAIN)gcc $^ $(CFLAGS) $(LFLAGS) -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(TOOLCHAIN)objcopy -O binary $< $@

clean: 
	rm -f $(NAME)*.bin *.map *.o $(CPUDIR)
	find . -name '*.o' -delete
###############################################################################

