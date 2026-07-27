#ifndef ARCH_X86_64_ASM_H
#define ARCH_X86_64_ASM_H

#include <stdint.h>

static inline void io_outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ("outb %b0, %w1" :: "a"(val), "Nd"(port));
}

static inline uint8_t io_inb(uint16_t port)
{
	uint8_t val;
	__asm__ volatile ("inb %w1, %b0" :"=a"(val) : "Nd"(port));
	return val;
}

/* Halt Catch Fire
 * @warning this is unrecoverable
 */
static inline void hcf(void)
{
	__asm__ volatile ("cli");
	for (;;) __asm__ volatile ("hlt");
}


#endif /* ARCH_X86_64_ASM_H */
