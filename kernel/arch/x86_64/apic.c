#include <stdint.h>
#include <stdbool.h>
#include <boot/requests.h>
#include <cpuid.h>
#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <arch/x86_64/cpuid.h>
#include <arch/x86_64/madt.h>
#include <kernel/kernel.h>
#include <utils/kprint.h>
#include <mm/vmm.h>
static volatile uint8_t *lapic_base;

/* sets the base for the lapic */
void set_lapic_base(uintptr_t base) 
{
	lapic_base = (volatile uint8_t *)base;
}

/* initialize the base for the lapic */
void init_lapic_base(void) 
{
	uintptr_t physical_base = rdmsr(IA32_APIC_BASE_MSR) & 0xFFFFFFFFFFFFF000ULL;
	uintptr_t virtual_base = hhdm_request.response->offset + physical_base;

	vmm_map_page(virtual_base, physical_base, PTE_MMIO_FLAGS);
	set_lapic_base(virtual_base);
}

/* Read Register for LAPIC */
uint32_t lapic_read_reg(uint32_t offset) 
{
	return *(volatile uint32_t *)(lapic_base + offset);
}

/* Write to Register for LAPIC */
void lapic_write_reg(uint32_t offset, uint32_t value) 
{
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
void lapic_init(void)
{
	if (!check_apic()) {
		panic("NO APIC FOUND\n");
		/* todo remove panic and fallback to PIC */
	}
	cpu_set_apic_base(cpu_get_apic_base());
	init_lapic_base();
	lapic_write_reg(0xF0, lapic_read_reg(0xF0) | 0x100);
	lapic_write_reg(0x80, 0); 
}

/* Send an END OF EOI to apic eoi (0xB0)*/
void lapic_eoi(void)
{
	lapic_write_reg(0xB0, 0);
}


uint32_t ioapic_read(void *ioapic_addr, uint32_t reg)
{
	volatile uint32_t *ioapic = (volatile uint32_t*)ioapic_addr;
	ioapic[0] = (reg & 0xff);
	return ioapic[4];
}

void ioapic_write(void *ioapic_addr, uint32_t reg, uint32_t value)
{
	volatile uint32_t *ioapic = (volatile uint32_t*)ioapic_addr;
	ioapic[0] = (reg & 0xff);
	ioapic[4] = value;
}

static void *ioapic_addr;

/* initialize the ioapic */
void ioapic_init(struct madt_ioapic *ioapic)
{
	disable_pic();
    
	uint64_t phys_addr = ioapic->ioapic_addr;

	uint64_t virt_addr = hhdm_request.response->offset + phys_addr;
	uint64_t flags = PTE_PRESENT | PTE_MMIO_FLAGS; 

	vmm_map_page(virt_addr, phys_addr, flags);

	ioapic_addr = (void*)virt_addr;

	uint32_t ver = ioapic_read(ioapic_addr, IOAPICVER);
	uint8_t max_redir_entry = ((ver >> 16) & 0xFF) + 1;
	for (uint8_t i = 0; i < max_redir_entry; i++) {
		uint8_t vector = 0x20 + i;
		uint32_t reg_low = IOAPICTBL + (i * 2);
		uint32_t reg_high = reg_low + 1;

		ioapic_write(ioapic_addr, reg_low, 0x00010000 | vector);
		ioapic_write(ioapic_addr, reg_high, 0x00000000);
		ioapic_write(ioapic_addr, reg_low, vector);
	}
}

void ioapic_route(uint8_t pin, uint8_t vector)
{
	uint32_t reg_low = IOAPICTBL + (pin * 2);
	uint32_t reg_high = reg_low + 1;

	ioapic_write(ioapic_addr, reg_high, 0x00000000);
	ioapic_write(ioapic_addr, reg_low, vector);
}