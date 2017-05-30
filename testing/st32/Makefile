PROJECT = test

SRC_DIR = src
SYS_DIR = $(SRC_DIR)/sys

INCLUDES = -I include -I include/cmsis -I include/st

FLAGS := -std=gnu99 $(INCLUDES) -c -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -fno-delete-null-pointer-checks -fomit-frame-pointer -Os -mthumb -mfloat-abi=softfp -mfpu=fpv5-d16 -mcpu=cortex-m7

OBJECTS += $(SRC_DIR)/main.o
OBJECTS += $(SYS_DIR)/startup_stm32f767xx.o
#OBJECTS += $(SYS_DIR)/startupV2.o
OBJECTS += $(SYS_DIR)/system_stm32f7xx.o

LD_SCRIPT = script.ld

.s.o:
	arm-none-eabi-gcc $(FLAGS) -o $@ $<

.c.o:
	arm-none-eabi-gcc $(FLAGS) -o $@ $<

$(PROJECT).bin: $(PROJECT).elf
	arm-none-eabi-objcopy -O binary $< $@

$(PROJECT).elf: $(OBJECTS)
	arm-none-eabi-gcc -T $(LD_SCRIPT) --specs=nosys.specs --output $@ $(OBJECTS)

.PHONY: clean
clean:
	rm -f test.bin test.elf
	find . -name '*.o' -delete

