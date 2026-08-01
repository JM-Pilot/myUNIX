#ifndef KERNEL_H
#define KERNEL_H

#include <drivers/console.h>
/* the console for the entire kernel */
extern struct console kcon;

/* kernel panic */
void panic(const char* msg);
#endif /* KERNEL_H */