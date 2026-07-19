#include <stdint.h>
#include <boot/multiboot2.h>
#include <drivers/uart/serial.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/psf.h>
#include <kfonts.h>
#include <drivers/console/console.h>
#include <kernel/kernel.h>
#include <kstdio.h>
#include <arch/x86/cpu/gdt.h>
#include <arch/x86/cpu/idt.h>
#include <arch/x86/cpu/pic.h>
#include <arch/x86/cpu/irq.h>
#include <arch/x86/asm.h>
struct console *kcon;

static inline void hcf(void)
{
	for (;;) __asm__ volatile ("hlt");
}
void kernel_main(uint32_t magic, uint32_t boot_info)
{
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
	{
		hcf();
	}

	struct multiboot_tag *tag;

	if (serial_init() == false)
		hcf();

	/* check requests */
	for (tag = (struct multiboot_tag*)(boot_info + 8);
		tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7)))
	{
		switch (tag->type)
		{
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				framebuffer_init((struct multiboot_tag_framebuffer*)tag);
				break;
		}
	}

	/* initialize the console */
	console_init(&kcon, FONT_TER_V16B, 8, 0xFFFFFF, 0, false);
	kputs("Hello From Kernel STDIO (kstdio)\n");
	
	/* initialize the Global Descriptor Table */
	gdt_init();
	kprintf("[BOOT] GDT Initialized!\n");

	/* initialize the IDT and Interrupts */
	idt_init();
	kprintf("[BOOT] IDT Initialized\n");
	
	pic_remap(0x20, 0x28, 0xFE, 0xFF);
	kprintf("[BOOT] PIC Remaped\n");

	irq_init();
	kprintf("[BOOT] IRQ Initialized\n");

	__asm__ volatile ("sti");

	while (1) {};

	/* halt for now */
	serial_write("Halting System\n");
	hcf();
}
