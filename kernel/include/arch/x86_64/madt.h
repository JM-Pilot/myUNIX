#ifndef ARCH_X86_64_MADT_H
#define ARCH_X86_64_MADT_H
#include <stdint.h>
struct madt_sdt {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemid[6];
	char oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
}__attribute__((packed));

struct madt_header {
	struct madt_sdt madt;
	uint32_t apic_address;
	uint32_t flags;
}__attribute__((packed));

/* MADT ENTRIES */
struct madt_entry {
	uint8_t type;
	uint8_t length;
}__attribute__((packed));

struct madt_lapic {
	struct madt_entry e;
	uint8_t acpi_proc_id;
	uint8_t apic_id;
	uint32_t flags;
}__attribute__((packed));

struct madt_ioapic {
	struct madt_entry e;
	uint8_t ioapic_id;
	uint8_t rsvd;
	uint32_t ioapic_addr;
	uint32_t global_sys_int_base;
}__attribute__((packed));

/* ioapic interrupt source override */
struct madt_ioapic_iso {
	struct madt_entry e;
	uint8_t bus_src;
	uint8_t irq_src;
	uint32_t global_sys_int;
	uint16_t flags;
}__attribute__((packed));

/* ioapic non maskable interrupt src */
struct madt_ioapic_nmi_src {
	struct madt_entry e;
	uint8_t nmi_src;
	uint8_t rsvd;
	uint16_t flags;
	uint32_t global_sys_int;
}__attribute__((packed));

/* huge different between these two */

struct madt_lapic_nmi {
	struct madt_entry e;
	uint8_t acpi_proc_id;
	uint16_t flags;
	uint8_t lint;
}__attribute__((packed));

struct madt_lapic_addr_override {
	struct madt_entry e;
	uint16_t rsvd;
	uint64_t lapic_phys_addr;
}__attribute__((packed));

struct madt_proc_x2apic {
	struct madt_entry e;
	uint16_t rsvd;
	uint32_t lapic_id;
	uint32_t flags;
	uint32_t acpi_id;
}__attribute__((packed));

/* parse the madt */
void madt_parse(struct madt_header *madt);

#endif /* ARCH_X86_64_MADT_H */