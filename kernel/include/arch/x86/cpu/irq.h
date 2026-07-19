#ifndef ARCH_X86_CPU_IRQ_H
#define ARCH_X86_CPU_IRQ_H
#include <arch/x86/cpu/idt.h>

void irq_install_handler(int irq, void (*handler)(struct interrupt_frame *iframe));
void irq_remove_handler(int irq);
void irq_handle(struct interrupt_frame *iframe);
void irq_init(void);

#endif
