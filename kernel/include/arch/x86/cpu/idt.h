#ifndef ARCH_X86_CPU_IDT_H
#define ARCH_X86_CPU_IDT_H

#include <stdint.h>

struct idt_desc {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct idt_entry {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t rsvd;
	uint8_t flags;
	uint16_t offset_high;
}__attribute__((packed));

struct interrupt_frame {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_num, int_error_code;
	uint32_t eip, cs, eflags, useresp, ss;
};
void idt_set_entry(int desc, uint32_t offset, 
		   uint16_t segment, uint8_t flags);

void idt_init(void);

#endif /* ARCH_X86_CPU_IDT_H */
