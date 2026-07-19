#include <stdint.h>
#include <stdbool.h>
#include <drivers/uart/serial.h>
#include <arch/x86/asm.h>

static uint16_t current_serial = SERIAL_COM1;

/* returns false if serial is faulty 
 * true if it works
 */
bool serial_init(void)
{
	// disable interrupts
	port_outb(current_serial + 1, 0x0);
	// enable DLAB
	port_outb(current_serial + 3, 0x80);
	// set divisor to 3 38400 baud
	port_outb(current_serial + 0, 0x3);
	port_outb(current_serial + 1, 0x0);
	// 8 bits no parity
	port_outb(current_serial + 3, 0x3);
	// Enable FIFO, clear with 14b threshold
	port_outb(current_serial + 2, 0xC7);
	// irqs enabled
	port_outb(current_serial + 4, 0xB);
	// set in lo mode
	port_outb(current_serial + 4, 0x1E);
	// test serial chip
	port_outb(current_serial + 0, 0xAE);

	// check if its faulty
	if (port_inb(current_serial + 0) != 0xAE)
		return false;
	
	// enable irqs and OUT#1 & OUT#2 bits enabled
	port_outb(current_serial + 4, 0x0F);
	
	return true;
}

/* defaults current_serial to com1 */
static void invalid_serial(void)
{
	current_serial = SERIAL_COM1;
	serial_init();
	serial_write("INVALID SERIAL, USING COM1\n");
}

/* returns true if valid */
static bool check_if_correct_serial(void)
{
	return current_serial == SERIAL_COM1 ||
		current_serial == SERIAL_COM2 ||
		current_serial == SERIAL_COM3 ||
		current_serial == SERIAL_COM4 ||
		current_serial == SERIAL_COM5 ||
		current_serial == SERIAL_COM6 ||
		current_serial == SERIAL_COM7 ||
		current_serial == SERIAL_COM8;
}

/* check if there is a signal */
bool serial_signal_recived(void)
{
	if (!check_if_correct_serial()) invalid_serial();
	return port_inb(current_serial + 5) & 1;
}

/* read a char from serial  */
char serial_readch(void)
{
	if (!check_if_correct_serial()) invalid_serial();
	while (serial_signal_recived() == false);
	return port_inb(current_serial);
}

/* check if its empty */
bool serial_transmit_empty(void)
{
	if (!check_if_correct_serial()) invalid_serial();
	return port_inb(current_serial + 5) & 0x20;
}

/* put a char into serial */
void serial_putc(char c)
{
	if (!check_if_correct_serial()) invalid_serial();
	while (serial_transmit_empty() == false);
	port_outb(current_serial, c);
}

/* write a string to serial*/
void serial_write(const char *str)
{
	if (!check_if_correct_serial()) invalid_serial();
	while (*str)	
		serial_putc(*str++);
}

void serial_switch(uint32_t port)
{
	current_serial = port;
	if (!check_if_correct_serial()) invalid_serial();
}

