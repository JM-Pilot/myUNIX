#include <kernel/init.h>
#include <arch/x86_64/asm.h>
#include <utils/kprint.h>
#include <string.h>
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

void panic(const char* msg)
{
	kprintf("\n\nPANIC!!!\n");
	kprintf("AN UNRECOVERABLE ERROR HAS OCCURED!\n\n");

	kprintf("MSG: %s\n", msg);

	kprintf("MYNIX-V2 KERNEL PANIC\nREBOOT NOW\n");
	hcf();
}