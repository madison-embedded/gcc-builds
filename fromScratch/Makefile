
# Toolchain settings
AS      = 'arm-none-eabi-gcc'
CC      = 'arm-none-eabi-gcc'
LD      = 'arm-none-eabi-gcc'
ELF2BIN = 'arm-none-eabi-objcopy'

C_FLAGS = 
ASM_FLAGS = 
LD_FLAGS = 

OBJECTS += main.o
OBJECTS += start.o

LINK_SCRIPT = script.ld

.S.o:
	echo "Assemble: $<"
	$(AS) -c $(ASM_FLAGS) -o $@ $<

.c.o:
	echo "Compile: $<"
	$(CC) $(C_FLAGS) -o $@ $<

test.bin: test.elf
	$(ELF2BIN) -O binary $< $@
	echo "===== bin file ready to flash: $@ =====" 

test.elf: $(OBJECTS)
	echo "link: $@"
	$(LD) $(LD_FLAGS) -T $(LINK_SCRIPT) --output $@ $(OBJECTS)

.PHONY : clean
clean:
	rm test.bin test.elf $(OBJECTS)
