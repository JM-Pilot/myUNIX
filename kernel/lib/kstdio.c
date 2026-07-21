#include <kstdio.h>
#include <stddef.h>
#include <stdint.h>
#include <drivers/console/console.h>
#include <drivers/uart/serial.h>
#include <drivers/input/ps2.h>
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

/* recive input from ps2 */

char kgetc(void)
{
	char c = ps2_getc();
	kputc(c);
	return c;
}


int kgets(char *buf, size_t len)
{
	char c = 0;
	int i = 0;
	while ((c = ps2_getc()) != '\n' && len-- > 0) {
		if (c == '\b') {
			if (i == 0) continue;
			kputs("\b \b");
			i--;
			continue;
		}
		kputc(c);
		buf[i++] = c;
	}
	buf[i] = '\0';
	return i;
}
