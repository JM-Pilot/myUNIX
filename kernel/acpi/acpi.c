#include <stdint.h>
#include <acpi/acpi.h>
#include <boot/multiboot2.h>
#include <drivers/uart/serial.h>
void acpi_init(struct multiboot_tag_old_acpi *tag)
{
	struct rsdp_header *rsdp = (struct rsdp_header*)tag->rsdp;

	/* print out all information given to us by grub, this will be handy 
	 * when debugging */
	serial_write("RSDP SIGNATURE: ");
	for (int i = 0; i < 8; i++)
		serial_putc(rsdp->signature[i]);
	serial_write("\n");

	serial_write("RSDP OEMID: ");
	for (int i = 0; i < 6; i++)
		serial_putc(rsdp->OEMID[i]);
	serial_write("\n");

	serial_printf("RSDP INFO: \n"
			"CHECKSUM %#x\n"
			"REVISION %#x\n"
			"RSDT_ADDR %#x\n",
			rsdp->checksum,
			rsdp->revision,
			rsdp->rsdt_addr);

	/* do rsdt */
	struct rsdt_header *rsdt = (struct rsdt_header*)rsdp->rsdt_addr;
	/* (sdt.length - sizeof(sdt)) / 4 */
	uint32_t entry_count = (rsdt->sdt.length - sizeof(rsdt->sdt)) / 4;
	
	/* check entries */
	serial_printf("RSDT ENTRIES: %u\n", entry_count);

	for (uint32_t i = 0; i < entry_count; i++) {
		struct sdt_header *sdt = (struct sdt_header*)rsdt->entries[i];
		serial_printf("ENTRY [%u]: ", i);
		for (int j = 0; j < 4; j++)
			serial_putc(sdt->signature[j]);
		serial_putc('\n');
	}
}
