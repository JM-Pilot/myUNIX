#ifndef KERNEL_MALLOC_H
#define KERNEL_MALLOC_H

#include <stdint.h>
#include <mm/heap.h>

extern KHEAPBM kheap;

void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif /* KERNEL_KALLOC_H */