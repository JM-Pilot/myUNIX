#include <stdint.h>
#ifndef ARCH_X86_CPU_PIC_H
#define ARCH_X86_CPU_PIC_H

#define PIC1_COM 0x20
#define PIC1_DATA 0x21
#define PIC2_COM 0xA0
#define PIC2_DATA 0x21

#define PIC_EOI 0x20

#define ICW1_ICW4 0x1		/* indicate that ICW4 will be present */
#define ICW1_SINGLE 0x2	 	/* single cascade mode */
#define ICW1_INTERVAL4 0x4	/* call address interval */
#define ICW1_LEVEL 0x8		/* level triggered mode */
#define ICW1_INIT 0x10		/* init */

#define ICW4_8086 0x1		/* 8086/8088 mode */
#define ICW4_AUTO 0x2		/* normal eoi */
#define ICW4_BUF_SLAVE 0x8	
#define ICW4_BUF_MASTER 0xC
#define ICW4_SFNM 0x10		/* special fully nested */

#define CASCADE_IRQ 2

void pic_send_eoi(uint8_t irq);
void pic_remap(int offset1, int offset2, 
		uint8_t mask1, uint8_t mask2);
void pic_disable(void);

#endif /* ARCH_X86_CPU_PIC_H */
