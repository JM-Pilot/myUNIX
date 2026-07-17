MAKEFLAGS += \
	-j$(nproc) \
	--no-print-directory
KERNEL_DIR = kernel
BOOT_DIR = boot
BUILD_DIR = build
ISO_DIR = $(BUILD_DIR)/iso_build

kernel: build/myUnix-kernel

build/myUnix-kernel:
	@echo "BUILDING KERNEL"
	@$(MAKE) -C $(KERNEL_DIR) all
	@echo "FINISHED"

iso: build/myUnix.iso
build/myUnix.iso: build/myUnix-kernel
	@echo "BUILDING ISO"
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(BOOT_DIR)/grub/grub.cfg $(ISO_DIR)/boot/grub/
	@cp build/myUnix-kernel $(ISO_DIR)/boot/
	@grub2-mkrescue $(ISO_DIR) -o $@
	@echo "FINISHED"

run: iso
	@echo "RUNNING ON QEMU-SYSTEM-I386"
	@qemu-system-i386 -cdrom build/myUnix.iso \
		-d int,cpu_reset -D build/QEMU_LOGS.txt \
		-serial stdio
clean:
	rm -rf build