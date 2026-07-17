OUTPUT_FORMAT(elf32-i386)

ENTRY(_kernstart)
SECTIONS 
{
	. = 2M;
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.bss)
	}
}
