#ifndef ACPI_H
#define ACPI_H

#include <stdint.h> 
#include <stdbool.h>

/* revision 0 (v1.0)*/
struct rsdp_header {
	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt_address;	/* deprecated in v2*/
}__attribute__((packed));

/* revision 2 (v2.0) */
struct xsdp_header {
	struct rsdp_header rsdp;
	uint32_t length;
	uint64_t xsdt_address;
	uint8_t ext_checksum;
	uint8_t rsvd[3];	/* reserved */
}__attribute__((packed));


/* SDT */
struct sdt_header {
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

struct rsdt_header {
	struct sdt_header sdt;
	uint32_t entries[];
}__attribute__((packed));

struct xsdt_header {
	struct sdt_header sdt;
	uint64_t entries[];
}__attribute__((packed));

/* global rsdt */
extern struct rsdt_header *acpi_rsdt;

/* verify the checksum inside of header
 * @returns true or false
 */
bool sdt_verify_checksum(struct sdt_header *header);

/* Initialize ACPI, provides rsdt */
void acpi_init(void);

#endif /* ACPI_H */