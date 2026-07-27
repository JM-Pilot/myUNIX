#ifndef ARCH_X86_64_IDT_H
#define ARCH_X86_64_IDT_H

#define IDT_MAX_ENTRIES 256

#include <stdint.h>

struct idt_desc {
	uint16_t size;
	uint64_t offset;
}__attribute__((packed));

struct idt_entry {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t ist;
	uint8_t flags;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t rsvd;
}__attribute__((packed));

struct interrupt_frame {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
	uint64_t int_number, error_code;
	uint64_t rip, cs, rflags, rsp, ss; /* iretq */
};

/* sets the idt entry, 
 * @param offset -> offset of the handler, isr, or irq
 * @param segment -> kernel code
 * @param flags -> flags you want in the entry 
 * @note for flags here are the bits
 * @note P -> present bit
 * @note DPL -> privilege levels
 * @note Gate Type -> 0xE for interrupt gate, 0xF for trap gate
 */
void idt_set_entry(uint8_t entry, uint64_t offset, 
	uint16_t segment, uint8_t flags);

/* initializes the idt descriptor and loads the isrs into place */
void idt_init(void);

#endif /* ARCH_X86_64_IDT_H */