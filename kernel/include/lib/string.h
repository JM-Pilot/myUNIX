#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
int strcmp(const char *str_1, const char *str_2);
int strncmp(const char *s1, const char *s2, size_t len);
void *memcpy(void *dest, void *src, size_t len);
void *memmove(void *dest, void *src, size_t len);
void *memset(void *dest, int value, size_t len);

#endif /* STRING_H */