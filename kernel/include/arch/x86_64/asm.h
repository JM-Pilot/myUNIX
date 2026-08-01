#ifndef ARCH_X86_64_ASM_H
#define ARCH_X86_64_ASM_H

#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

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

#define CPUID_FLAG_MSR 1 << 5;

static inline bool cpu_has_msr()
{
	uint32_t eax, edx, unused;
	__get_cpuid(1, &eax, &unused, &unused, &edx);
	return edx & CPUID_FLAG_MSR;
}

static inline void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
	asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

static inline void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
	asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}


static inline uint64_t rdmsr(uint32_t msr)
{
	uint32_t lo;
	uint32_t hi;

	asm volatile (
		"rdmsr"
		: "=a"(lo), "=d"(hi)
		: "c"(msr)
	);

	return ((uint64_t)hi << 32) | lo;
}

static inline void wrmsr(uint32_t msr, uint64_t value)
{
	uint32_t lo = (uint32_t)value;
	uint32_t hi = (uint32_t)(value >> 32);

	asm volatile (
		"wrmsr"
		:
		: "c"(msr), "a"(lo), "d"(hi)
		: "memory"
	);
}

static inline void invlpg(uint64_t addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
#endif /* ARCH_X86_64_ASM_H */
