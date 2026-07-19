#include <stdint.h>
#include <arch/x86/cpu/irq.h>
#include <arch/x86/cpu/idt.h>
#include <arch/x86/cpu/gdt.h>
#include <arch/x86/cpu/pic.h>
extern uint32_t irq_table[16];

static void *irq_handlers[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

/* install a handler so the irqs know where to go */
void irq_install_handler(int irq, void (*handler)(struct interrupt_frame *iframe))
{
	irq_handlers[irq] = handler;
}

/* removes a handler */
void irq_remove_handler(int irq)
{
	irq_handlers[irq] = 0;
}

/* handle an irq, call a handler, then send an EOI */
void irq_handle(struct interrupt_frame *iframe)
{
	void (*handler)(struct interrupt_frame *iframe);
	handler = irq_handlers[iframe->int_num - 32];
	if (handler)
		handler(iframe);
	
	pic_send_eoi(iframe->int_num - 32);
}

/* loads all irqs */
void irq_init(void)
{
	for (int i = 0; i < 16; i++)
		idt_set_entry(32 + i, irq_table[i], KERNEL_CODE_SEG, 0x8E);
}
