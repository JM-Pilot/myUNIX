#include <drivers/timers/pit.h>
#include <arch/x86/asm.h>
#include <arch/x86/cpu/idt.h>
#include <arch/x86/cpu/irq.h>
#include <stdint.h>

static volatile uint32_t ticks;

void pit_handler(struct interrupt_frame *iframe)
{
	(void)iframe;
	/* do something here */
	ticks++;
}

/* how many hz */
void pit_set_hz(uint32_t hz)
{
	uint16_t divisor = PIT_FREQUENCY / hz;
	port_outb(PIT_COMMAND, 0x36);
	port_outb(PIT_CHANNEL, (uint8_t)(divisor & 0xFF));
	port_outb(PIT_CHANNEL, (uint8_t)((divisor >> 8) & 0xFF));
}

/* returns ticks */
uint32_t pit_get_ticks(void)
{
	return ticks;
}

/* waits for (n) ms */
void pit_wait(uint32_t ms)
{
	uint32_t start = pit_get_ticks();
	uint32_t ticks = (ms * PIT_HZ + 999) / 1000;

	/* or pause */
	while ((pit_get_ticks() - start) < ticks)
		__asm__ volatile ("hlt");
}

/* sets pit hz to 250 and load the pit_handler */
void pit_init(void)
{
	pit_set_hz(250);
	irq_install_handler(0, pit_handler);
}

