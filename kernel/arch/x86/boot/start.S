#define ASM_FILE 1
#include <boot/multiboot2.h>

/* align first */
.align 8
multiboot_header_start:
	.long MULTIBOOT2_HEADER_MAGIC
	.long MULTIBOOT_ARCHITECTURE_I386
	.long multiboot_header_end - multiboot_header_start
	.long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header_start))

.framebuffer_req:
	.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER 
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long .framebuffer_req_end - .framebuffer_req
	.long 1280
	.long 720
	.long 32
.framebuffer_req_end:

/* align the end too, so grub will see it*/
.align 8
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8
multiboot_header_end:

.bss
/* initialize our stack */
stack_btm:
	.skip 16384
stack_top:

.text
.global _kernstart

_kernstart:
	mov $stack_top, %esp

	push %ebx
	push %eax
	
	.extern kernel_main
	call kernel_main
	
	cli
1:	hlt
	jmp 1b
