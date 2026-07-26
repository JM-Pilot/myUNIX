MAKEFLAGS = \
	    --no-print-directory
BUILD = build
KERNEL_DIR = kernel
BOOT_DIR = boot
TOOLS_DIR = tools
OUTPUT = mynix

QEMU_FLAGS = \
	-m 512M \
	-d int,cpu_reset -D qemu_logs.txt

all: $(BUILD)/myunix.iso

# get limine
tools-limine: $(TOOLS_DIR)/limine-binary
$(TOOLS_DIR)/limine-binary:
	@echo "Getting Limine Binaries"
	@curl -L -s https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz | gunzip | tar -xf - --one-top-level=$(dir $@)
	@$(MAKE) -C $@

# get edk2
tools-edk2: $(TOOLS_DIR)/edk2-ovmf-bins
$(TOOLS_DIR)/edk2-ovmf-bins:
	@echo "Getting edk2-ovmf Binaries"
	@curl -L -s https://github.com/osdev0/edk2-ovmf-stable-bins/releases/latest/download/edk2-ovmf-bins.tar.gz | gunzip | tar -xf - --one-top-level=$(dir $@)

# build the kernel
kernel: build/$(OUTPUT)-kernel
build/$(OUTPUT)-kernel:
	@printf "\033[32mBuilding Kernel\033[0m\n"
	@$(MAKE) -C $(KERNEL_DIR) OUTPUT="$(OUTPUT)"

# build the iso
iso: $(BUILD)/$(OUTPUT).iso
$(BUILD)/$(OUTPUT).iso: tools-limine kernel
	@printf "\033[32mBuilding ISO\033[0m\n"
	mkdir -p $(BUILD)/iso/boot/limine
	cp $(BUILD)/$(OUTPUT)-kernel $(BUILD)/iso/
	cp $(BOOT_DIR)/limine.conf $(BUILD)/iso/boot/limine/
	cp $(TOOLS_DIR)/limine-binary/BOOTX64.EFI $(BUILD)/iso/boot/limine/
	cp $(TOOLS_DIR)/limine-binary/*.bin $(BUILD)/iso/boot/limine/
	cp $(TOOLS_DIR)/limine-binary/limine-bios.sys $(BUILD)/iso/boot/limine
	@xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        	-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        	-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        	-efi-boot-part --efi-boot-image --protective-msdos-label \
        	$(BUILD)/iso -o $@ >/dev/null 2>&1
	$(TOOLS_DIR)/limine-binary/limine bios-install $@ > /dev/null 2>&1
	
run-efi: tools-edk2 iso
	qemu-system-x86_64 \
	    	-cdrom $(BUILD)/$(OUTPUT).iso \
		-drive if=pflash,unit=0,format=raw,file=$(TOOLS_DIR)/edk2-ovmf-bins/ovmf-code-x86_64.fd,readonly=on \
	    	-M q35 \
		$(QEMU_FLAGS) \
		-serial stdio 

# use this if your terminal is broken
#| stdbuf -oL sed -E '/\x1b\[2J\x1b\[H/b; s/\x1b\[[0-9;?]*[a-zA-Z]//g; s/\r//g'

clean:
	@printf "\033[31mCleaning Up\033[0m\n"
	rm -rf $(BUILD)
	@echo "rm -rf kernel/bin"
	@$(MAKE) -C $(KERNEL_DIR) clean
	@rm -rf qemu_logs.txt
	@rm -rf serial_log.txt

clean-tools:
	rm -rf $(TOOLS_DIR)
