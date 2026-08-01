#include <kernel/malloc.h>
#include <stdint.h>
#include <mm/heap.h>

void *kmalloc(uint32_t size)
{
	return k_heapBMAlloc(&kheap, size);
}

void kfree(void *ptr)
{
	k_heapBMFree(&kheap, ptr);
}