BUILD = ../build
CC = i686-elf-gcc
OBJCOPY = i686-elf-objcopy
C_FLAGS = \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-nostdlib \
	-Iinclude \
	-Iinclude/lib
LD_FLAGS = \
	-ffreestanding \
	-nostdlib \
	-T arch/x86/boot/linker.ld

C_SRCS = $(shell find . -iname "*.c" -printf "%P\n")
S_SRCS = $(shell find . -iname "*.S" -printf "%P\n")
FONT_SRCS = $(shell find . -iname "*.psfu" -printf "%P\n")

C_OBJS = $(patsubst %.c, $(BUILD)/kernel/%.c.o, $(C_SRCS))
S_OBJS = $(patsubst %.S, $(BUILD)/kernel/%.S.o, $(S_SRCS))
FONT_OBJS = $(patsubst %.psfu, $(BUILD)/kernel/%.psfu.o, $(FONT_SRCS))

OBJS = $(S_OBJS) $(C_OBJS) $(FONT_OBJS)

all: $(BUILD)/myUnix-kernel

$(BUILD)/kernel/%.psfu.o: %.psfu
	@printf "OBJCOPY (FONT) $<\n"
	@mkdir -p $(dir $@)
	@$(OBJCOPY) -O elf32-i386 -B i386 -I binary $< $@
#	@readelf -sW $@

$(BUILD)/kernel/%.c.o: %.c
	@printf "CC $<\n"
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) -c $< -o $@

$(BUILD)/kernel/%.S.o: %.S
	@printf "CC (S) $<\n"
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) -c $< -o $@

$(BUILD)/myUnix-kernel: $(OBJS)
	@printf "CC (LD) %s\n" $(patsubst ../%, %, $@);
	@$(CC) $(LD_FLAGS) $^ -o $@
	@if grub2-file --is-x86-multiboot2 $@; then \
		echo "KERNEL IS MULTIBOOT2"; \
	else \
		echo "KERNEL IS NOT MULTIBOOT2, ABORTING"; \
		exit 1; \
	fi