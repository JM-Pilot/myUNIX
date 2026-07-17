#include <kstdio.h>
#include <stdint.h>
#include <drivers/console/console.h>
#include <drivers/uart/serial.h>
#include <kernel/kernel.h>

/* outputs to kcon (kernel console) */
void kputc(char c)
{
	console_putc(kcon, c);
	serial_putc(c);
}
void kputs(const char *str)
{
	while (*str)
		kputc(*str++);	
}