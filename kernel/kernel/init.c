#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <utils/psf.h>
#include <boot/limine.h>
#include <boot/requests.h>
#include <drivers/video/framebuffer.h>
#include <drivers/uart/serial.h>
#include <drivers/console.h>
#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/cpu/idt.h>
#include <arch/x86_64/asm.h>
#include <kernel/init.h>
#include <kernel/kernel.h>
#include <utils/kprint.h>


/* globals we need to define */
struct console kcon;



/* limine stuff */
__attribute__((used, section(".limine_requests")))
volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;




/* reboot if the request is invalid */
void init_check_requests(void)
{
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
		io_outb(0x64, 0xFE);
	if (framebuffer_request.response == NULL || 
		framebuffer_request.response->framebuffer_count < 1)
		io_outb(0x64, 0xFE);
}

/* initialize important kernel functions */
void init(void)
{
	framebuffer_init();
	serial_init();
	serial_puts("\033[2J\033[H");
	serial_puts("Serial Initialized, Hello World!\n");
	console_init(&kcon, DEFAULT_FONT, 0xFFFFFF, 0, 8);
	kprintf("Console Initialized\n");

	gdt_init();
	kprintf("GDT Initialized\n");

	idt_init();
	kprintf("IDT Initialized\n");
}
