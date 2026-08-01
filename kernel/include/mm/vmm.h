#ifndef MM_VMM_H
#define MM_VMM_H

#include <stdint.h>

#define PTE_PRESENT  (1ULL << 0)
#define PTE_WRITABLE (1ULL << 1)
#define PTE_USER     (1ULL << 2)
#define PTE_ADDR_MASK 0x000FFFFFFFFFF000ULL

#define PTE_PWT (1ULL << 3)
#define PTE_PCD (1ULL << 4)

#define PTE_MMIO_FLAGS (PTE_WRITABLE | PTE_PCD | PTE_PWT)

#define PAGE_SIZE 0x1000

void vmm_init(void);
void vmm_unmap_page(uint64_t virt_addr);
void vmm_map_page(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);

#endif /* MM_VMM_H */