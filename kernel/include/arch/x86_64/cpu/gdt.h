#ifndef ARCH_X86_64_CPU_GDT_H
#define ARCH_X86_64_CPU_GDT_H

#define KERNEL_CODE 0x8
#define KERNEL_DATA 0x10

#include <stdint.h>

struct gdt_desc {
	uint16_t size;
	uint64_t offset; /* not 32, remember we are in x86_64 */
}__attribute__((packed));

/* same fields as x86_32 */
struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
}__attribute__((packed));

void gdt_set_entry(uint8_t entry, 
		uint32_t limit, 
		uint32_t base, 
		uint8_t access, 
		uint8_t flags);

void gdt_init(void);

#endif /* ARCH_X86_64_CPU_GDT_H */
