###############################################################################
#                          Project Settings                                   #
###############################################################################
MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
RM = '$(SHELL)' -c "rm -rf \"$(1)\""

PROJECT := gcc_testing

OBJECTS += main.o
OBJECTS += system.o
OBJECTS += start.o

OBJDIR := BUILD
###############################################################################


###############################################################################
#                          Toolchain Settings                                 #
###############################################################################
AS      = 'arm-none-eabi-gcc' '-x' 'assembler-with-cpp' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-d16' '-mfloat-abi=softfp'
CC      = 'arm-none-eabi-gcc' '-std=gnu99' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-Os' '-mcpu=cortex-m7' '-mthumb' '-mfpu=fpv5-d16' '-mfloat-abi=softfp'
LD      = 'arm-none-eabi-gcc'
ELF2BIN = 'arm-none-eabi-objcopy'

C_FLAGS += -std=gnu99

LD_FLAGS :=-Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp 
LD_SYS_LIBS :=-Wl,--start-group -lm -lc -lgcc -lnosys -Wl,--end-group
###############################################################################


###############################################################################
#                                 Rules                                       #
###############################################################################
all: $(PROJECT).bin
	echo "Done"
	
clean:
	$(call RM,$(OBJDIR))

.S.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

.c.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) -o $@ $<

$(PROJECT).elf: $(OBJECTS) link.ld 
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(filter %.ld, $^) --output $@ $(filter %.o, $^) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@
	+@echo "===== bin file ready to flash: $(OBJDIR)/$@ =====" 
###############################################################################

