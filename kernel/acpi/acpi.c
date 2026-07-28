#include <acpi/acpi.h>
#include <stdint.h>
#include <utils/kprint.h>
#include <boot/requests.h>



/* putc len 
 * just a helper function
 */
static void _putcl(const char *msg, int len)
{
	while (len-- && *msg)
		kprintf("%c", *msg++);
}
void acpi_init(void)
{
	struct limine_rsdp_response *rsdp_response = rsdp_request.response;
	
	struct xsdp_header *xsdp = (struct xsdp_header*)rsdp_response->address;

	uint64_t hhdm_offset = hhdm_request.response->offset;
	
	/* DEBUG ACPI */
	kprintf("ACPI PARSING: \n");
	kprintf("XSDP.RSDP Signature: ");
	_putcl(xsdp->rsdp.signature, 8);
	kprintf("\n");

	kprintf("XSDP.RSDP Checksum: %u\n", xsdp->rsdp.checksum);
	kprintf("XSDP.RSDP oemid: ");
	_putcl(xsdp->rsdp.oemid, 6);
	kprintf("\n");

	kprintf("XSDP.RSDP Revision: %u\n", xsdp->rsdp.revision);

	/* PARSE XSDT OR RSDT */
	if (xsdp->rsdp.revision >= 2) {
		kprintf("XSDP XSDT Address: %p\n", xsdp->xsdt_address);

		uint64_t xsdt_phys = (uint64_t)xsdp->xsdt_address;
		struct xsdt_header *xsdt = (struct xsdt_header*)(xsdt_phys + hhdm_offset);

		if (!sdt_verify_checksum(&xsdt->sdt)) {
			kprintf("XSDT.SDT INVALID\n");
			return;
		}
		uint64_t entries = (xsdt->sdt.length - sizeof(xsdt->sdt)) / 8;
		kprintf("XSDT ENTRIES: %u\n", entries);

		/* print all entries */
		for (uint64_t i = 0; i < entries; i++) {
			kprintf("XSDT ENTRY %#u: ", i);
			uint64_t phys = xsdt->entries[i];
			struct sdt_header *h = (struct sdt_header*)(phys + hhdm_offset);
			_putcl(h->signature, 4);
			kprintf("\n");

			/* select the entry then do what you need to do */
		}

	} else {
		kprintf("XSDP RSDP Address: %p\n", xsdp->rsdp.rsdt_address);
		uint32_t rsdt_phys = (uint32_t)xsdp->rsdp.rsdt_address;
		struct rsdt_header *rsdt = (struct rsdt_header*)(rsdt_phys + hhdm_offset);
		if (!sdt_verify_checksum(&rsdt->sdt)) {
			kprintf("RSDT.SDT INVALID\n");
			return;
		}
		uint32_t entries = (rsdt->sdt.length - sizeof(rsdt->sdt)) / 4;
		kprintf("RSDT ENTRIES: %u\n", entries);

		for (uint32_t i = 0; i < entries; i++) {
			kprintf("RSDT ENTRY %#u: ", i);
			uint32_t phys = rsdt->entries[i];
			struct sdt_header *h = (struct sdt_header*)(phys + hhdm_offset);
			_putcl(h->signature, 4);
			kprintf("\n");
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