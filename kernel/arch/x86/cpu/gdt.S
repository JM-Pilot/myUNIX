#define ASM_FILE 1
#include <arch/x86/cpu/gdt.h>
.text
.globl gdt_load

gdtr:	.word 0
	.long 0
gdt_load:
	mov 4(%esp), %ax
	mov %ax, gdtr
	mov 8(%esp), %eax
	mov %eax, gdtr + 2
	lgdt gdtr

	jmp $KERNEL_CODE_SEG, $.reload_cs
.reload_cs:
	mov $KERNEL_DATA_SEG, %ax
	mov %ax, %ds 
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret
	
