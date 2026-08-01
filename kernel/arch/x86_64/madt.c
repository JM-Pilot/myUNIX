#include <arch/x86_64/madt.h>
#include <arch/x86_64/apic.h>
#include <kernel/kernel.h>
#include <stdint.h>

void madt_parse(struct madt_header *madt)
{
	uint8_t *ptr = (uint8_t*)madt + sizeof(struct madt_header);
	uint8_t *end = (uint8_t*)madt + madt->madt.length;

	kprint(KLOG_DEBUG, "Parsing MADT\n");
	while (ptr < end) {
		struct madt_entry *entry = (struct madt_entry*)ptr;

		switch (entry->type) {
			/* LAPIC */
			case 0: {
				struct madt_lapic *lapic = (struct madt_lapic*)entry;
				kprint(KLOG_DEBUG, "MADT LAPIC: \n");
				kprint(KLOG_DEBUG, "CPU: %u\nAPIC: %u\nFLAGS: %x\n",
					lapic->acpi_proc_id, lapic->apic_id, lapic->flags);
				break;
			}
			/* IOAPIC */
			case 1: {
				struct madt_ioapic *ioapic = (struct madt_ioapic*)entry;
				ioapic_init(ioapic);
				/* warn bcuz why not */
				kprint(KLOG_WARN, "MADT: IOAPIC Initialized\n");
				break;
			}
		}
		ptr += entry->length;
	}
}