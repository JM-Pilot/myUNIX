#include <kernel/init.h>
#include <arch/x86_64/asm.h>
#include <utils/kprint.h>
#include <string.h>
#include <kernel/kernel.h>
#include <drivers/input/ps2.h>
void kernel_main(void)
{
	init_check_requests();
	init();
	
	/* for some reason i like the orange color */
	kprint(KLOG_WARN, "Welcome to mynix-v2!\n");

	/* we have stuff to turn on interrupts now 
	 * todo: Make a scheduler and start process 1
	 */
	__asm__ volatile ("sti");

	while (1) {
		kprintf("%c", ps2_getc());
	}

	/* not reached */
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
