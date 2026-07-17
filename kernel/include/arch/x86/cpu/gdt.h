
#ifndef ARCH_X86_BOOT_GDT_H
#define ARCH_X86_BOOT_GDT_H

/* we only need this in some parts and in gdt.S */

#define KERNEL_CODE_SEG 0x8
#define KERNEL_DATA_SEG 0x10

/* parts down here will be irrelevant to gdt.S */
#ifndef ASM_FILE 
#include <stdint.h>

struct gdt_desc {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
}__attribute__((packed));

void gdt_set_desc(int desc, 
	uint32_t limit, uint32_t base, 
	uint8_t access, uint8_t flags);

void gdt_init(void);
#endif /* ASM_FILE*/

#endif /* ARCH_X86_BOOT_GDT_H */
