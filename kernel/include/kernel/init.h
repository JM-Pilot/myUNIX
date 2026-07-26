#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

#include <stdint.h>
#include <boot/limine.h>


static inline void hcf(void)
{
	__asm__ volatile ("cli");
	for (;;) __asm__ volatile ("hlt");
}

void init(void);
void init_check_requests(void);

#endif /* KERNEL_INIT_H */
