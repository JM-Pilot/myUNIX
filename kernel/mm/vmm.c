#include <mm/vmm.h>
#include <mm/pmm.h>
#include <stdint.h>
#include <string.h>
#include <boot/requests.h>
#include <arch/x86_64/asm.h>
/* Helper functions */
static inline void *phys_to_virt(uint64_t phys) {
	return (void*)(phys + hhdm_request.response->offset);
}

static inline uint64_t virt_to_phys(void *virt) {
	return (uint64_t)(virt - hhdm_request.response->offset);
}

static uint64_t* get_pml4_virt_addrs(void) {
	uint64_t cr3_val;
	asm volatile("mov %%cr3, %0" : "=r"(cr3_val));
	uint64_t pml4_phys = cr3_val & 0xFFFFFFFFFFFFF000; 
	return (uint64_t*)phys_to_virt(pml4_phys);
}

void vmm_map_page(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags)
{
	uint64_t pml4_idx = (virt_addr >> 39) & 0x1FF;
	uint64_t pdpt_idx = (virt_addr >> 30) & 0x1FF;
	uint64_t pd_idx = (virt_addr >> 21) & 0x1FF;
	uint64_t pt_idx = (virt_addr >> 12) & 0x1FF;

	uint64_t *pml4 = get_pml4_virt_addrs();


	uint64_t table_flags = PTE_PRESENT | PTE_WRITABLE | PTE_USER;

	if (!(pml4[pml4_idx] & PTE_PRESENT)) {
		uint64_t new_table_phys = (uint64_t)(uintptr_t)pmm_alloc(1);
		memset(phys_to_virt(new_table_phys), 0, PAGE_SIZE);
		pml4[pml4_idx] = new_table_phys | table_flags;
	}

	uint64_t *pdpt = (uint64_t*)phys_to_virt(pml4[pml4_idx] & PTE_ADDR_MASK);
	if (!(pdpt[pdpt_idx] & PTE_PRESENT)) {
		uint64_t new_table_phys = (uint64_t)(uintptr_t)pmm_alloc(1);
		memset(phys_to_virt(new_table_phys), 0, 4096);
		pdpt[pdpt_idx] = new_table_phys | table_flags;
	}

	uint64_t *pd = (uint64_t*)phys_to_virt(pdpt[pdpt_idx] & PTE_ADDR_MASK);
	if (!(pd[pd_idx] & PTE_PRESENT)) {
		uint64_t new_table_phys = (uint64_t)(uintptr_t)pmm_alloc(1);
		memset(phys_to_virt(new_table_phys), 0, 4096);
		pd[pd_idx] = new_table_phys | table_flags;
	}

	uint64_t *pt = (uint64_t*)phys_to_virt(pd[pd_idx] & PTE_ADDR_MASK);
	pt[pt_idx] = (phys_addr & PTE_ADDR_MASK) | flags | PTE_PRESENT;
	invlpg(virt_addr);
}

void vmm_unmap_page(uint64_t virt_addr)
{
	uint64_t pml4_idx = (virt_addr >> 39) & 0x1FF;
	uint64_t pdpt_idx = (virt_addr >> 30) & 0x1FF;
	uint64_t pd_idx = (virt_addr >> 21) & 0x1FF;
	uint64_t pt_idx = (virt_addr >> 12) & 0x1FF;

	uint64_t *pml4 = get_pml4_virt_addrs();

	if (!(pml4[pml4_idx] & PTE_PRESENT)) return;
	uint64_t *pdpt = (uint64_t*)phys_to_virt(pml4[pml4_idx] & PTE_ADDR_MASK);

	if (!(pdpt[pdpt_idx] & PTE_PRESENT)) return;
	uint64_t *pd = (uint64_t*)phys_to_virt(pdpt[pdpt_idx] & PTE_ADDR_MASK);

	if (!(pd[pd_idx] & PTE_PRESENT)) return;
	uint64_t *pt = (uint64_t*)phys_to_virt(pd[pd_idx] & PTE_ADDR_MASK);

	pt[pt_idx] = 0;
	
	invlpg(virt_addr);
}


void vmm_init(void)
{
	uint64_t hhdm_offset = hhdm_request.response->offset;
	uint64_t pml4_phys = (uint64_t)(uintptr_t)pmm_alloc(1);
	uint64_t *pml4_virt = (uint64_t*)(pml4_phys + hhdm_offset);

	uint64_t *boot_pml4 = get_pml4_virt_addrs();

	for (int i = 0; i < 256; i++) {
		pml4_virt[i] = 0;
	}

	for (int i = 256; i < 512; i++) {
		pml4_virt[i] = boot_pml4[i];
	}

	/* make cr3 point to pml4 */
	__asm__ volatile("mov %0, %%cr3" :: "r"(pml4_phys) : "memory");
}