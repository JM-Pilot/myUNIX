#include <stdint.h>
#include <stdbool.h>
#include <drivers/input/ps2.h>
#include <arch/x86/cpu/irq.h>
#include <arch/x86/cpu/idt.h>
#include <arch/x86/asm.h>

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
static uint8_t scancode;
static bool is_shift = false;
void irq1_kbd_handler(struct interrupt_frame *iframe)
{
	(void)iframe;
	int sc = port_inb(0x60);
	
	/* lshift and rshift */
	if (sc == 0x2A || sc == 0x36)
		is_shift = true;
	else if (sc == 0xAA || sc == 0xB6)
		is_shift = false;
	else if (sc & 0x80) {
		/* k release */
	} else { 
		/* k pressed */
		scancode = sc;
	}
}

/* returns the scancode returned by the handler */
uint8_t ps2_getsc(void)
{
	uint8_t sc = scancode;
	scancode = 0;
	return sc;
}

/* returns char based on us keyboard layout */
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

/* loads irq1 handler into irq handlers */
void ps2_init(void)
{
	irq_install_handler(1, irq1_kbd_handler);
}

