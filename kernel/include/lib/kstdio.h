#include <stdarg.h>
#ifndef KSTDIO_H
#define KSTDIO_H

void kputc(char c);
void kputs(const char *str);

int kvsprintf(char *buffer, const char *fmt, va_list args);
int kprintf(const char *fmt, ...);

#endif