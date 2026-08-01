#ifndef ARCH_X86_64_IRQ_H
#define ARCH_X86_64_IRQ_H

#include <arch/x86_64/cpu/idt.h>

void irq_install_handler(int irq, void (*handler)(struct interrupt_frame *iframe));
void irq_remove_handler(int irq);
void irq_handle(int irq, struct interrupt_frame *iframe);
void irq_init(void);

#endif /* ARCH_X86_64_IRQ_H */