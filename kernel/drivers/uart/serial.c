#include <stdint.h>
#include <stdbool.h>
#include <drivers/uart/serial.h>
#include <arch/x86_64/asm.h>

/* our serial port */
static uint16_t serial_port = SERIAL_COM1;

/* initialize serial 
 * default is SERIAL_COM1
 */
bool serial_init(void)
{
	/* disable Interrupts */
	io_outb(serial_port + 1, 0x0);
	/* enable DLAB */
	io_outb(serial_port + 3, 0x80);

	/* set divisor 3 (38400) baud */
	io_outb(serial_port, 0x3);
	io_outb(serial_port + 1, 0x0);
	
	/* 8 bits no parity */
	io_outb(serial_port + 3, 0x3);
	/* enable FIFO, clear with 14b threshold */
	io_outb(serial_port + 2, 0xC7);
	
	/* irqs enabled */
	io_outb(serial_port + 4, 0xB);

	/* set in lo mode */
	io_outb(serial_port + 4, 0x1E);

	/* test */
	io_outb(serial_port, 0xAE);

	/* check if its faulty */
	if (io_inb(serial_port) != 0xAE)
		return false;
	
	/* set to normal mode */
	io_outb(serial_port + 4, 0xF);
	return true;
}

/* check if we can output or not */
static bool check_if_can_out(void)
{
	return io_inb(serial_port + 5) & 0x20;
}
/* sends a character into serial */
void serial_putc(char c)
{
	while (!check_if_can_out());
	io_outb(serial_port, c);
}

/* sends a string into serial (via putc) */
void serial_puts(const char *str)
{
	while (*str)
		serial_putc(*str++);
}

/* check if we recive a signal */
static bool check_if_recived(void)
{
	return io_inb(serial_port) & 1;
}
char serial_readc(void)
{
	while (!check_if_recived());
	return io_inb(serial_port);
}

/* checks if the port is valid */
static bool validate_serial(uint16_t port)
{
	return 	port == SERIAL_COM1 ||
		port == SERIAL_COM2 ||
		port == SERIAL_COM3 ||
		port == SERIAL_COM4 ||
		port == SERIAL_COM5 ||
		port == SERIAL_COM6 ||
		port == SERIAL_COM7 ||
		port == SERIAL_COM8;
}

/* switches serial ports
 * if port is not a valid serial port, COM1 will be used 
 */
void serial_switch(uint16_t port)
{
	if (!validate_serial(port))
		serial_port = SERIAL_COM1;
	serial_port = port;
	serial_init();
}
