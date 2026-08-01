#include <stdint.h>
#include <stdbool.h>
#include <boot/requests.h>
#include <cpuid.h>
#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <arch/x86_64/cpuid.h>
#include <kernel/kernel.h>
#include <utils/kprint.h>


static volatile uint8_t *lapic_base;


/* sets the base for the lapic */
void set_lapic_base(uintptr_t base) {
	lapic_base = (volatile uint8_t *)base;
}

/* initialize the base for the lapic */
void init_lapic_base(void) {
	uintptr_t physical_base = rdmsr(IA32_APIC_BASE_MSR) & 0xFFFFFFFFFFFFF000ULL;
	
	set_lapic_base(hhdm_request.response->offset + physical_base);

	kprintf("Physical APIC: %lx\n", (void *)physical_base);
	kprintf("Virtual APIC : %lx\n", (void *)lapic_base);
}

uint32_t read_reg(uint32_t offset) {
	return *(volatile uint32_t *)(lapic_base + offset);
}

void write_reg(uint32_t offset, uint32_t value) {
	*(volatile uint32_t *)(lapic_base + offset) = value;
}

/* returns true if the apic is available
 * false if not
 */
static inline bool check_apic(void)
{
	uint32_t eax, edx, unused;
	__get_cpuid(1, &eax, &unused, &unused, &edx);
	return edx & CPUID_FEAT_EDX_APIC;
}

/* disable pic because we use APIC */
void disable_pic(void)
{
	/* mask slave pic and master pic */
	io_outb(0x21, 0xFF);
	io_outb(0xA1, 0xFF);
}

/* sets the base for the apic */
void cpu_set_apic_base(uintptr_t apic)
{
	uint32_t edx = 0;
	uint32_t eax = (apic & 0xFFFFF000) | IA32_APIC_BASE_MSR_ENABLE;

#ifdef __PHYSICAL_MEMORY_EXTENSION__
	edx = (apic >> 32) & 0x0f;
#endif /* __PHYSICAL_MEMORY_EXTENSION__ */
	cpu_set_msr(IA32_APIC_BASE_MSR, eax, edx);
}
/* finds the base for the apic */
uintptr_t cpu_get_apic_base(void)
{
	uint32_t eax, edx;
	cpu_get_msr(IA32_APIC_BASE_MSR, &eax, &edx);
#ifdef __PHYSICAL_MEMORY_EXTENSION__
	return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
	return (eax & 0xfffff000);
#endif
}
void apic_init(void)
{
	kprintf("CHECKING APIC AVAIL\n");
	if (!check_apic()) {
		panic("NO APIC FOUND\n");
		/* todo remove panic and fallback to PIC */
	}

	kprintf("DISABLING PIC\n");
	disable_pic();

	kprintf("SET APIC BASE\n");
	cpu_set_apic_base(cpu_get_apic_base());
	kprintf("INIT LAPIC BASE\n");
	init_lapic_base();

	kprintf("WRITING SVR\n");
	
	write_reg(0xF0, read_reg(0xF0) | 0x100);
}