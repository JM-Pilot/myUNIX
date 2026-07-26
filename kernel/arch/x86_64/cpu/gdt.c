#include <arch/x86_64/cpu/gdt.h>
#include <stdint.h>

extern void gdt_load(struct gdt_desc *gdtr);

static struct gdt_entry gdt_table[3];

/* sets the entry with the fields 
 * entry must not exceede 3
 */
void gdt_set_entry(uint8_t entry, 
		uint32_t limit, 
		uint32_t base, 
		uint8_t access, 
		uint8_t flags)
{
	if (entry >= 3) return;
	gdt_table[entry].base_low = base & 0xFFFF;
	gdt_table[entry].base_mid = (base >> 16) & 0xFF;
	gdt_table[entry].base_high = (base >> 24) & 0xFF;

	gdt_table[entry].limit_low = limit & 0xFFFF;
	gdt_table[entry].flags = (flags << 4) | ((limit >> 16) & 0x0F);
	gdt_table[entry].access = access;
}

/* sets all entry and loads it */
void gdt_init(void)
{
	struct gdt_desc gdtr;
	gdtr.offset = (uint64_t)&gdt_table;
	gdtr.size = sizeof(gdt_table) - 1;
	
	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0xFFFFF, 0, 0x9A, 0xA);
	gdt_set_entry(2, 0xFFFFF, 0, 0x92, 0xC);

	gdt_load(&gdtr);
}
