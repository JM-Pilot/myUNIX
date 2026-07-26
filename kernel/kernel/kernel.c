#include <kernel/init.h>
#include <boot/limine.h>
#include <stdint.h>
#include <drivers/video/framebuffer.h>

void kernel_main(void)
{
	init_check_requests();
	init();

	hcf();
}
