#include <kernel/init.h>
#include <arch/x86_64/asm.h>
void kernel_main(void)
{
	init_check_requests();
	init();
	/*
	 * __asm__ volatile ("sti");
	 * while (1);
	 */
	hcf();
}
