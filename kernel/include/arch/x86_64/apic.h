#ifndef ARCH_X86_64_APIC_H
#define ARCH_X86_64_APIC_H

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#include <arch/x86_64/madt.h>

/* initialize lapic */
void lapic_init(void);
void lapic_eoi(void);

uint32_t lapic_read_reg(uint32_t offset);
void lapic_write_reg(uint32_t offset, uint32_t value);
void set_lapic_base(uintptr_t base);
void init_lapic_base(void);

#define IOAPICID 0x0
#define IOAPICVER 0x1
#define IOAPICARB 0x2
#define IOAPICTBL 0x10
#define IOAPICREDTBL(n) (0x10 + 2 * n);

void ioapic_init(struct madt_ioapic *ioapic);

/* disable the legacy PIC (i8259) */
void disable_pic(void);

#endif /* ARCH_X86_64_APIC_H */