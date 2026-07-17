#include <stdint.h>
#include <stdbool.h>
#include <drivers/uart/serial.h>
#include <arch/x86/asm.h>

/* returns false if serial is faulty 
 * true if it works
 */
bool serial_init(void)
{
	// disable interrupts
	port_outb(DEFAULT_PORT + 1, 0x0);
	// enable DLAB
	port_outb(DEFAULT_PORT + 3, 0x80);
	// set divisor to 3 38400 baud
	port_outb(DEFAULT_PORT + 0, 0x3);
	port_outb(DEFAULT_PORT + 1, 0x0);
	// 8 bits no parity
	port_outb(DEFAULT_PORT + 3, 0x3);
	// Enable FIFO, clear with 14b threshold
	port_outb(DEFAULT_PORT + 2, 0xC7);
	// irqs enabled
	port_outb(DEFAULT_PORT + 4, 0xB);
	// set in lo mode
	port_outb(DEFAULT_PORT + 4, 0x1E);
	// test serial chip
	port_outb(DEFAULT_PORT + 0, 0xAE);

	// check if its faulty
	if (port_inb(DEFAULT_PORT + 0) != 0xAE)
		return false;
	
	// enable irqs and OUT#1 & OUT#2 bits enabled
	port_outb(DEFAULT_PORT + 4, 0x0F);
	
	/* clear screen */
	serial_write("\033[H\033[J");
	serial_write("SERIAL INITIALIZED\n");
	return true;
}
/* check if there is a signal */
bool serial_signal_recived(void)
{
	return port_inb(DEFAULT_PORT + 5) & 1;
}

/* read a char from serial  */
char serial_readch(void)
{
	while (serial_signal_recived() == false);
	return port_inb(DEFAULT_PORT);
}

/* check if its empty */
bool serial_transmit_empty(void)
{
	return port_inb(DEFAULT_PORT + 5) & 0x20;
}

/* put a char into serial */
void serial_putc(char c)
{
	while (serial_transmit_empty() == false);
	port_outb(DEFAULT_PORT, c);
}

/* write a string to serial*/
void serial_write(const char *str)
{
	while (*str)	
		serial_putc(*str++);
}


