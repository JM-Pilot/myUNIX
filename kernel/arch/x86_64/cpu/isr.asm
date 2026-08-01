[BITS 64]

; push all 64 bit registers
%macro PUSHAQ 0
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

; pop all 64 bit registers
%macro POPAQ 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro


%macro ISR_ERROR 1
isr_stub_%+%1:
	; dont push dummy here
	push %1
	jmp isr_handler
%endmacro

%macro ISR_NO_ERROR 1
isr_stub_%+%1:
	push 0 	; dummy because we dont have the use for the error code
	push %1
	jmp isr_handler
%endmacro

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
ISR_ERROR 30
ISR_NO_ERROR 31

global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
	dq isr_stub_%+i
%assign i i+1 
%endrep

; IRQS 
ISR_NO_ERROR 32
ISR_NO_ERROR 33
ISR_NO_ERROR 34
ISR_NO_ERROR 35
ISR_NO_ERROR 36
ISR_NO_ERROR 37
ISR_NO_ERROR 38
ISR_NO_ERROR 39
ISR_NO_ERROR 40
ISR_NO_ERROR 41
ISR_NO_ERROR 42
ISR_NO_ERROR 43
ISR_NO_ERROR 44
ISR_NO_ERROR 45
ISR_NO_ERROR 46
ISR_NO_ERROR 47

global irq_stub_table
irq_stub_table:
%assign i 32
%rep    16
	dq isr_stub_%+i
%assign i i+1 
%endrep

extern interrupt_handler
isr_handler:
	cld 	; sysv requires
	PUSHAQ
	mov rdi, rsp
	call interrupt_handler
	POPAQ
	add rsp, 16
	iretq