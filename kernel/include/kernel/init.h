#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

#include <stdint.h>
#include <boot/limine.h>

void init(void);
void init_check_requests(void);

#endif /* KERNEL_INIT_H */
