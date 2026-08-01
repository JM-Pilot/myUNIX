#ifndef KERNEL_H
#define KERNEL_H

#include <drivers/console.h>

#define KLOG_ERROR 1
#define KLOG_DEBUG 2
#define KLOG_WARN 3

/* the console for the entire kernel */
extern struct console kcon;

/* kernel panic */
void panic(const char* msg);
int kprint(int log, const char *fmt, ...);
#endif /* KERNEL_H */