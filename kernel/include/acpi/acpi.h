#ifndef ACPI_H
#define ACPI_H
#include <stdint.h>
#include <boot/multiboot2.h>
/* old rsdp */
struct rsdp_header {
	char signature[8];
	uint8_t checksum;
	char OEMID[6];
	uint8_t revision;
	uint32_t rsdt_addr;
} __attribute__ ((packed));

/* we are in x86_32 / IA32 i cannot guarantee this will be used */
/* new rsdp */
struct xsdp_header {
	struct rsdp_header rsdp;
	uint32_t length;
	uint64_t xsdt_addr;
	uint8_t ext_checksum;
	uint8_t rsvd[3];
}__attribute__((packed));

/* sdt_header */
struct sdt_header {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char OEMID[6];
	char OEM_table_ID[8];
	uint32_t OEM_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
}__attribute__((packed));

struct rsdt_header {
	struct sdt_header sdt;
	uint32_t entries[];
}__attribute__((packed));

void acpi_init(struct multiboot_tag_old_acpi *tag);

#endif /* ACPI_H */
