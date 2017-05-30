COMMON_FLAGS := -mthumb -mfloat-abi=softfp -mfpu=fpv5-d16 -mcpu=cortex-m7

FLAGS := -std=gnu99 -c -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -fno-delete-null-pointer-checks -fomit-frame-pointer -Os

OBJECTS += main.o
OBJECTS += startup_stm32f767xx.o
OBJECTS += system_stm32f7xx.o

.s.o:
	arm-none-eabi-gcc $(FLAGS) $(COMMON_FLAGS) -o $@ $<

.c.o:
	arm-none-eabi-gcc $(FLAGS) $(COMMON_FLAGS) -o $@ $<

test.bin: test.elf
	arm-none-eabi-objcopy -O binary $< $@

test.elf: $(OBJECTS)
	arm-none-eabi-gcc -T script.ld --specs=nosys.specs --output $@ $(OBJECTS)

.PHONY: clean
clean:
	rm -f *.o test.bin test.elf

