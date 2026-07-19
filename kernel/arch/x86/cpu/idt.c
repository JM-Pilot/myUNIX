#include <stdint.h>
#include <kstdio.h>
#include <arch/x86/cpu/idt.h>
#include <arch/x86/cpu/gdt.h>
#include <arch/x86/cpu/irq.h>
extern uint32_t isr_table[32];

__attribute__((aligned(0x10)))
static struct idt_entry idt_entries[256];
/* sets the entry with the filled out arguments */
void idt_set_entry(int desc, uint32_t offset, 
		   uint16_t segment, uint8_t flags)
{
	idt_entries[desc].offset_low = offset & 0xFFFF;
	idt_entries[desc].offset_high = (offset >> 16) & 0xFFFF;
	idt_entries[desc].segment = segment;
	idt_entries[desc].flags = flags;
	idt_entries[desc].rsvd = 0;
}


/* loads isrs and the descriptor */
void idt_init(void)
{
	static struct idt_desc desc;
	desc.size = sizeof(idt_entries) - 1;
	desc.offset = (uint32_t)&idt_entries;
	
	/* load the isrs */
	for (int i = 0; i < 32; i++)
		idt_set_entry(i, isr_table[i], KERNEL_CODE_SEG, 0x8E);

	/* load the descriptor */
	__asm__ volatile ("lidt %0" :: "m"(desc));
}

/* readable exceptions for us human beings */
static const char *exceptions[] = {
	"DIVISION ERROR",
	"DEBUG",
	"NON-MASKABLE INTERRUPT",
	"BREAKPOINT",
	"OVERFLOW",
	"BOUND RANGE EXCEEDED",
	"INVALID OPCODE",
	"DEVICE NOT AVAILABLE",
	"DOUBLE FAULT",
	"COPROCESSOR SEGMENT OVERRUN",
	"INVALID TSS",
	"SEGMENT NOT PRESENT",
	"STACK-SEGMENT FAULT",
	"GENERAL PROTECTION FAULT",
	"PAGE FAULT",
	"CPU RESERVED",
	"x87 FLOATING-POINT EXCEPTION",
	"ALIGNMENT CHECK",
	"MACHINE CHECK",
	"SIMD FLOATING-POINT EXCEPTION",
	"VIRTUALIZATION EXCEPTION",
	"CONTROL PROTECTION EXCEPTION",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"HYPERVISOR INJECTION EXCEPTION",
	"SECURITY EXCEPTION",
	"RESERVED"
};

/* handles cpu interrupts */
void interrupt_handler(struct interrupt_frame *iframe)
{
	if (iframe->int_num >= 32 && iframe->int_num <= 47)
		irq_handle(iframe);

	if (iframe->int_num < 32) {
		kprintf("Interrupt!\n");
		kprintf("Interrupt Number: %d (%s), Error Code %#.2X\n", 
			iframe->int_num, exceptions[iframe->int_num], iframe->int_error_code);
		/* print registers because they are useful */
		kprintf("REGISTERS: \n");
		kprintf(" EAX: %#x, ECX: %#x, EDX: %#x, ESP: %#x\n",
			iframe->eax, iframe->ecx, iframe->edx, iframe->esp);
		kprintf(" EBP: %#x, ESI: %#x, EDI: %#x\n",
			iframe->ebp, iframe->esi, iframe->edi);
		
		kprintf(" GS: %#x, FS: %#x, ES: %#x, DS: %#x\n",
			iframe->gs, iframe->fs, iframe->es, iframe->ds);
		kprintf(" EFLAGS: %#x, CS: %#x, EIP: %#x\n",
			iframe->eflags, iframe->cs, iframe->eip);
		kprintf("Halting!\nreboot now\n");
		__asm__ volatile ("cli");
		for (;;) __asm__ volatile ("hlt");
	}		
}
