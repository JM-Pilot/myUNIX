#ifndef ARCH_X86_ASM_H
#define ARCH_X86_ASM_H
#include <stdint.h>

static inline void port_outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ("outb %b0, %w1" :: "a"(val), "Nd"(port));
}

static inline uint8_t port_inb(uint16_t port)
{
	uint8_t val;
	__asm__ volatile ("inb %w1, %b0" : "=a"(val) : "Nd"(port));
	return val;
}

static inline void port_wait(void)
{
	port_outb(0x80, 0);
}


/* either use this or gcc <cpuid.h> */
static inline void cpuid(int code, uint32_t *a, uint32_t *d)
{
	__asm__ volatile ("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}
#endif /* ARCH_X86_ASM_H */
