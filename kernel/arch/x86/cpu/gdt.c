#include <stdint.h>
#include <arch/x86/cpu/gdt.h>

extern void gdt_load(uint16_t size, uint32_t offset);
static struct gdt_entry gdt_entries[3];

/* sets all attributes on the descriptors */
void gdt_set_desc(int desc, 
	uint32_t limit, uint32_t base, 
	uint8_t access, uint8_t flags)
{
	gdt_entries[desc].limit_low = limit & 0xFFFF;
	gdt_entries[desc].base_low = base & 0xFFFF;
	gdt_entries[desc].base_mid = (base >> 16) & 0xFFFF;
	gdt_entries[desc].base_high = (base >> 24) & 0xFFFF;
	gdt_entries[desc].flags = (flags & 0xF) << 4 | (limit & 0xF );
	gdt_entries[desc].access = access;
}

/* loads the kernel entries, null entries and other stuff */
void gdt_init(void)
{
	static struct gdt_desc desc;
	desc.size = sizeof(gdt_entries) - 1;
	desc.offset = (uint32_t)&gdt_entries;
	/* null */
	gdt_set_desc(0, 0, 0, 0, 0);
	/* kernel code */
	gdt_set_desc(1, 0xFFFFF, 0, 0x9A, 0xC);
	/* kernel data  */
	gdt_set_desc(2, 0xFFFFF, 0, 0x92, 0xCF);

	/* load */
	gdt_load(desc.size, desc.offset);
}