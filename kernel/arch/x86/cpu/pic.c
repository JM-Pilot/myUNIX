#include <stdint.h>
#include <arch/x86/cpu/pic.h>
#include <arch/x86/asm.h>

void pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
		port_outb(PIC2_COM, PIC_EOI);
	port_outb(PIC1_COM, PIC_EOI);
}

/* remaps the pic */
void pic_remap(int offset1, int offset2, 
		uint8_t mask1, uint8_t mask2)
{
	/* start init with cascade mode
	 * using PIC1_COM and PIC2_COM 
	 */
	port_outb(PIC1_COM, ICW1_INIT | ICW1_ICW4);
	port_wait();
	port_outb(PIC2_COM, ICW1_INIT | ICW1_ICW4);
	port_wait();

	/* put the offsets */
	port_outb(PIC1_DATA, offset1);
	port_wait();
	port_outb(PIC2_DATA, offset2);
	port_wait();
	/* tell the master that there is a slave at irq2 */
	port_outb(PIC1_DATA, 1 << CASCADE_IRQ);
	port_wait();
	/* tell the slave its cascade id */
	port_outb(PIC2_DATA, CASCADE_IRQ);
	port_wait();

	port_outb(PIC1_DATA, ICW4_8086);
	port_wait();
	port_outb(PIC2_DATA, ICW4_8086);
	port_wait();

	port_outb(PIC1_COM, mask1);
	port_outb(PIC2_COM, mask2);
}

/* disable the pic if you want to use APIC */
void pic_disable(void)
{
	port_outb(PIC1_DATA, 0xFF);
	port_outb(PIC2_DATA, 0xFF);
}
