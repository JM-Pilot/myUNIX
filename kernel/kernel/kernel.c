#include <kernel/init.h>
#include <arch/x86_64/asm.h>
#include <utils/kprint.h>
#include <string.h>
#include <kernel/kernel.h>
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
	kprint(KLOG_ERROR, "\n\nPANIC!!!\n");
	kprint(KLOG_ERROR, "AN UNRECOVERABLE ERROR HAS OCCURED!\n\n");

	kprint(KLOG_ERROR, "MSG: %s\n", msg);

	kprint(KLOG_ERROR, "MYNIX-V2 KERNEL PANIC\nREBOOT NOW\n");
	hcf();
}