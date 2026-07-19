.macro ISR_NO_ERROR num
isr_stub_\num:
	push $0		/* we must push a dummy error code */
	push \num
	jmp _isr_handler
.endm

.macro ISR_ERROR num
isr_stub_\num:
	/* do not push a dummy error code because the cpu will do it */
	push \num
	jmp _isr_handler
.endm

/* define all of the isrs so the cpu can use them */
ISR_NO_ERROR 0
ISR_NO_ERROR 1
ISR_NO_ERROR 2
ISR_NO_ERROR 3
ISR_NO_ERROR 4
ISR_NO_ERROR 5
ISR_NO_ERROR 6
ISR_NO_ERROR 7
ISR_ERROR 8
ISR_NO_ERROR 9
ISR_ERROR 10
ISR_ERROR 11
ISR_ERROR 12
ISR_ERROR 13
ISR_ERROR 14
ISR_NO_ERROR 15
ISR_NO_ERROR 16
ISR_ERROR 17
ISR_NO_ERROR 18
ISR_NO_ERROR 19
ISR_NO_ERROR 20
ISR_NO_ERROR 21
ISR_NO_ERROR 22
ISR_NO_ERROR 23
ISR_NO_ERROR 24
ISR_NO_ERROR 25
ISR_NO_ERROR 26
ISR_NO_ERROR 27
ISR_NO_ERROR 28
ISR_NO_ERROR 29
ISR_NO_ERROR 30
ISR_ERROR 31

/* make a table so idt.c can access it and load it */
.globl isr_table
isr_table:
.set i, 0
.rept 32
	.long isr_stub_0 + (i)
	.set i, i + 1
.endr

/* we make the cpu know whats going on, push registers and call the C Handler */
.extern interrupt_handler
_isr_handler:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

/* this does not work at all
	mov %esp, %eax
	push %eax
	mov interrupt_handler, %eax 
	call *%eax
	pop %eax
*/

	push %esp
	call interrupt_handler
	add $4, %esp

	pop %gs
	pop %fs
	pop %es
	pop %ds

	add $8, %esp
	popa
	iret
