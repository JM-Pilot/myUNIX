#include <stdint.h>
#include <arch/x86_64/cpu/idt.h>
#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/asm.h>
#include <utils/kprint.h>
/* performance!! */
__attribute__((aligned(0x10)))
static struct idt_entry idt_table[IDT_MAX_ENTRIES];

extern uint64_t isr_stub_table[32];
void idt_set_entry(uint8_t entry, uint64_t offset, 
	uint16_t segment, uint8_t flags)
{
	idt_table[entry].offset_low = offset & 0xFFFF;
	idt_table[entry].offset_mid = (offset >> 16) & 0xFFFF;
	idt_table[entry].offset_high = (offset >> 32) & 0xFFFFFFFF;
	idt_table[entry].flags = flags;
	idt_table[entry].ist = 0;
	idt_table[entry].segment = segment;
	idt_table[entry].rsvd = 0;
}

void idt_init(void)
{
	struct idt_desc desc;
	desc.offset = (uint64_t)&idt_table;
	desc.size = sizeof(idt_table) - 1;
	
	for (int i = 0; i < 32; i++)
		idt_set_entry(i, isr_stub_table[i], KERNEL_CODE, 0x8E);
	__asm__ volatile ("lidt %0" : : "m"(desc));
}

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
	"CPU RESERVED",
	"HYPERVISOR INJECTION EXCEPTION",
	"VMM COMMUNICATION EXCEPTION",
	"SECURITY EXCEPTION",
	"RESERVED"
};


void interrupt_handler(struct interrupt_frame *iframe)
{
	if (iframe->int_number < 32) {
		kprintf("\n\n=============INTERRUPT REACHED!============\n");
		kprintf("INTERRUPT: %s (%#.2lx), ERROR CODE: %lu\n",
			exceptions[iframe->int_number], iframe->int_number,
			iframe->error_code);

		kprintf("=================REGISTERS=================\n");
		kprintf("RAX: %#lx, RBX: %#lx, RCX: %#lx, RDX: %#lx\n",
			iframe->rax, iframe->rbx, iframe->rcx, iframe->rdx);
		
		kprintf("RSI: %#lx, RDI: %#lx, RBP: %#lx, R8: %#lx\n",
			iframe->rsi, iframe->rdi, iframe->rbp, iframe->r8);
		
		kprintf("R9: %#lx,  R10: %#lx, R11: %#lx, R12: %#lx\n",
			iframe->r9, iframe->r10, iframe->r11, iframe->r12);
		
		kprintf("R13: %#lx, R14: %#lx, R15: %#lx, RIP: %#lx\n",
			iframe->r13, iframe->r14, iframe->r15, iframe->rip);
		
		kprintf("CS: %#lx, RFLAGS: %#lx, RSP: %#lx, SS: %#lx\n",
			iframe->cs, iframe->rflags, iframe->rsp, iframe->ss);

		kprintf("\nHALTING!\n");
		hcf();
	}
}