#ifndef MM_PMM_H
#define MM_PMM_H

#include <stddef.h>

size_t getmem(void);
void pmm_init(void);
void pmm_free_page(size_t base, size_t length);
void pmm_alloc_page(size_t base, size_t length);
void *pmm_alloc(size_t count);
void pmm_free(void *phys, size_t count);


#endif /* MM_PMM_H */