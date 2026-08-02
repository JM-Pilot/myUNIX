#include <stdint.h>
#include <stdbool.h>
#include <arch/x86_64/cpu/irq.h>
#include <arch/x86_64/cpu/idt.h>
#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <drivers/input/ps2.h>
#include <utils/kprint.h>

/* GOD BLESS AMERICA!!!! */
const char kbd_us[] = {
	0, 27,
	'1','2','3','4','5','6','7','8','9','0','-','=',
	'\b','\t',
	'q','w','e','r','t','y','u','i','o','p','[',']',
	'\n',0,
	'a','s','d','f','g','h','j','k','l',';','\'','`',
	0,'\\',
	'z','x','c','v','b','n','m',',','.','/',
	0,'*',0,' ',
};

const char kbd_us_shift[] = {
	0, 27,
	'!','@','#','$','%','^','&','*','(',')','_','+',
	'\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}',
	'\n',0,
	'A','S','D','F','G','H','J','K','L',':','"','~',
	0,'|',
	'Z','X','C','V','B','N','M','<','>','?',
	0,'*',0,' ',
};

/* global scancode (pressed), used by ps2_getc */
static uint8_t scancode;
/* global scancode (released) */
static uint8_t scancode_rel;
static bool is_shift;
void irq1_handler(struct interrupt_frame *iframe)
{
	(void)iframe;
	uint8_t sc = io_inb(0x60);

	if (sc == 0x2A || sc == 0x36)
		is_shift = true;
	else if (sc == 0xAA || sc == 0xB6)
		is_shift = false;
	else if (sc & 0x80) {
		/* key released */
		scancode_rel = sc;
	}
	else {
		/* key pressed */
		scancode = sc;
	}
}

/* PS2 Get Scancode Pressed */
uint8_t ps2_getsc(void)
{
	uint8_t sc = scancode;
	scancode = 0;
	return sc;
}


/* PS2 Get Released Scancode */
uint8_t ps2_getsc_rel(void)
{
	uint8_t sc = scancode_rel;
	scancode_rel = 0;
	return sc;
}

/* PS2 Get Char
 * this uses the US Keyboard Layout 
 */
char ps2_getc(void)
{
	uint8_t sc = 0;
	while ((sc = ps2_getsc()) == 0);
	char c = 0;
	if (is_shift)
		c = kbd_us_shift[sc];
	else
		c = kbd_us[sc];
	return c;
}
void ps2_init(void)
{
	irq_install_handler(1, irq1_handler);
	ioapic_route(1, 33);
}