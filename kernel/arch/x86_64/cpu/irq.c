#include <stdint.h>
#include <arch/x86_64/cpu/irq.h>
#include <arch/x86_64/cpu/idt.h>
#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/apic.h>
extern uint64_t irq_stub_table[16];

/* irq_handle will call the corresponding handler */
void *irq_handlers[16] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

/* install a handler for the irq */
void irq_install_handler(int irq, void (*handler)(struct interrupt_frame *iframe))
{
	irq_handlers[irq] = handler;
}

/* remove a handler for the irq */
void irq_remove_handler(int irq)
{
	irq_handlers[irq] = 0;
}

/* handles an irq 
 * this automatically sends an EOI
 */
void irq_handle(int irq, struct interrupt_frame *iframe)
{
	void (*handler)(struct interrupt_frame *iframe);
	handler = irq_handlers[irq];

	if (handler)
		handler(iframe);
	
	/* always send an eoi */
	lapic_eoi();
}

void irq_init(void)
{
	for (int i = 0; i < 16; i++)
		idt_set_entry(32 + i, irq_stub_table[i], KERNEL_CODE, 0x8E);	
}