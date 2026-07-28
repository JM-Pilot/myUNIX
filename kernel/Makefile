BIN = bin
BUILD = ../build
CC = x86_64-elf-gcc
ASM = nasm

# filled by the root makefile
OUTPUT = 

C_FLAGS = \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	-Iinclude \
	-Iinclude/lib


LD_FLAGS = \
	   -Wall \
	   -Wextra \
	   -ffreestanding \
	   -nostdlib \
	   -O2 \
	   -T arch/x86_64/boot/linker.ld 

C_SRCS = $(shell find . -iname "*.c" -printf "%P\n")
C_OBJS = $(patsubst %.c, $(BIN)/%.o, $(C_SRCS))

ASM_SRCS = $(shell find . -iname "*.asm" -printf "%P\n")
ASM_OBJS = $(patsubst %.asm, $(BIN)/%.o, $(ASM_SRCS))

# v1
PSF_SRCS = $(shell find . -iname "*.psf" -printf "%P\n")
PSF_OBJS = $(patsubst %.psf, $(BIN)/%.o, $(PSF_SRCS))

# v2
PSFU_SRCS = $(shell find . -iname "*.psfu" -printf "%P\n")
PSFU_OBJS = $(patsubst %.psfu, $(BIN)/%.o, $(PSFU_SRCS))

# all objs that will be used to link in kernel
OBJS = $(C_OBJS) $(ASM_OBJS) $(PSF_OBJS) $(PSFU_OBJS)

# what we want to build
all = $(BUILD)/$(OUTPUT)-kernel

$(BIN)/%.o: %.psf
	@printf "OBJCOPY (FONT) $@\n"
	@mkdir -p $(dir $@)
	@objcopy -O elf64-x86-64 -B i386 -I binary $< $@
$(BIN)/%.o: %.psfu
	@printf "OBJCOPY (FONT) $@\n"
	@mkdir -p $(dir $@)
	@objcopy -O elf64-x86-64 -B i386 -I binary $< $@
$(BIN)/%.o: %.c
	@printf "CC\t$@\n"
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) -c $< -o $@

$(BIN)/%.o: %.asm
	@printf "AS\t$@\n"
	@mkdir -p $(dir $@)
	@$(ASM) -f elf64 $< -o $@

$(BUILD)/$(OUTPUT)-kernel: $(OBJS)
	@printf "LD\t$@\n"
	@mkdir -p $(dir $@)
	@$(CC) $(LD_FLAGS) $^ -o $@
# just remove bin
clean:
	@rm -rf $(BIN)
