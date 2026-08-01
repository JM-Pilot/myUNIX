#include <acpi/acpi.h>
#include <stdint.h>
#include <string.h>
#include <utils/kprint.h>
#include <kernel/kernel.h>
#include <boot/requests.h>
#include <arch/x86_64/madt.h>
#include <drivers/timers/hpet.h>

void acpi_init(void)
{
	struct limine_rsdp_response *rsdp_response = rsdp_request.response;
	
	struct xsdp_header *xsdp = (struct xsdp_header*)rsdp_response->address;

	uint64_t hhdm_offset = hhdm_request.response->offset;
	
	/* PARSE XSDT OR RSDT */
	if (xsdp->rsdp.revision >= 2) {

		uint64_t xsdt_phys = (uint64_t)xsdp->xsdt_address;
		struct xsdt_header *xsdt = (struct xsdt_header*)(xsdt_phys + hhdm_offset);

		if (!sdt_verify_checksum(&xsdt->sdt)) {
			kprintf("XSDT.SDT INVALID\n");
			return;
		}
		uint64_t entries = (xsdt->sdt.length - sizeof(xsdt->sdt)) / 8;

		/* print all entries */
		for (uint64_t i = 0; i < entries; i++) {
			uint64_t phys = xsdt->entries[i];
			struct sdt_header *entry = (struct sdt_header*)(phys + hhdm_offset);
			/* select the entry then do what you need to do */

			if (strncmp(entry->signature, "APIC", 4) == 0) {
				madt_parse((struct madt_header*)entry);
			}
			else if (strncmp(entry->signature, "HPET", 4) == 0) {
				hpet_init((struct hpet_header*)entry);
				kprint(KLOG_WARN, "HPET Initialized\n");
			}
		}

	} else {
		uint32_t rsdt_phys = (uint32_t)xsdp->rsdp.rsdt_address;
		
		struct rsdt_header *rsdt = (struct rsdt_header*)(rsdt_phys + hhdm_offset);
		if (!sdt_verify_checksum(&rsdt->sdt)) {
			return;
		}
		uint32_t entries = (rsdt->sdt.length - sizeof(rsdt->sdt)) / 4;

		for (uint32_t i = 0; i < entries; i++) {
			uint32_t phys = rsdt->entries[i];
			struct sdt_header *entry = (struct sdt_header*)(phys + hhdm_offset);
			/* select the entry then do what you need to do */
			(void)entry;
		}
	}
}

bool sdt_verify_checksum(struct sdt_header *header)
{
	uint8_t sum = 0;
	for (uint32_t i = 0; i < header->length; i++)
		sum += ((uint8_t*)header)[i];
	return sum == 0;
}