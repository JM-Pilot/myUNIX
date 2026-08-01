#include <mm/pmm.h>
#include <mm/vmm.h>
#include <stdint.h>
#include <stddef.h>
#include <utils/kprint.h>
#include <boot/requests.h>
#include <boot/limine.h>
#include <kernel/kernel.h>
#include <string.h>
static uint8_t *bitmap;
static size_t bitmap_size;
static size_t total_pages;

/* PMM Allocate
 * @returns NULL if no available
 */
void *pmm_alloc(size_t count)
{
	/* we want consecutive pages */
	if (count == 0)
		return NULL;
	size_t consec_free_pages = 0;
	size_t start_page_idx = 0;
	for (size_t i = 0; i < total_pages; i++) {
		/* skip used */
		if ((bitmap[i / 8] & (1 << (i % 8))) != 0) {
			consec_free_pages = 0;
			continue;
		}

		/* now alloc if not used */
		if (consec_free_pages == 0)
			start_page_idx = i;
		consec_free_pages++;
		if (consec_free_pages == count) {
			for (size_t p = 0; p < count; p++) {
				size_t cpage = start_page_idx + p;
				bitmap[cpage / 8] |= (1 << (cpage % 8));
			}
			return (void *)(start_page_idx * PAGE_SIZE);
		}
	}
	return NULL;
}

/* PMM Free
 * frees the addr
 */
void pmm_free(void *phys, size_t count)
{
	if (phys == NULL || count == 0)
		return;
	
	size_t base = (size_t)phys;
	/* align check */
	if (base % PAGE_SIZE != 0)
		return;
	
	pmm_free_page(base, count * PAGE_SIZE);
}
/* Free Page 
 * no need to compute base / PAGE_SIZE and length / PAGE_SIZE
 */
void pmm_free_page(size_t base, size_t length) 
{
	size_t start = base / PAGE_SIZE;
	size_t page_cnt = length / PAGE_SIZE;
	for (size_t p = 0; p < page_cnt; p++) {
		size_t cpage = start + p;
		bitmap[cpage / 8] &= ~(1 << (cpage % 8));
	}
}

/* Alloc Page 
 * no need to compute base / PAGE_SIZE and length / PAGE_SIZE
 */
void pmm_alloc_page(size_t base, size_t length)
{
	size_t start = base / PAGE_SIZE;
	size_t page_cnt = length / PAGE_SIZE;
	for (size_t p = 0; p < page_cnt; p++) {
		size_t cpage = start + p;
		bitmap[cpage / 8] |= (1 << (cpage % 8));
	}
}

/* Initialize the Physical Memory Manager */
void pmm_init(void)
{
	struct limine_hhdm_response *hhdm = hhdm_request.response;
	struct limine_memmap_response *mmap = mmap_request.response;

	/* highest address */
	uint64_t hi_addr = 0;
	for (size_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *mmap_entry = mmap->entries[i];
		uint64_t top = mmap_entry->base + mmap_entry->length;
		if (top > hi_addr)
			hi_addr = top;
	}

	kprintf("PMM HIGHEST ADDR: %#lx\n", hi_addr);
	total_pages = hi_addr / PAGE_SIZE;
	kprintf("PMM TOTAL PAGES: %lu\n", total_pages);
	bitmap_size = total_pages / 8;
	kprintf("PMM BITMAP SIZE: %lu\n", bitmap_size);

	/* find a good spot to place the bitmap */
	uint64_t bitmap_phys = 0;
	for (size_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *mmap_entry = mmap->entries[i];
		if (mmap_entry->type == LIMINE_MEMMAP_USABLE && mmap_entry->length >= bitmap_size) {
			bitmap_phys = mmap_entry->base;
			break;
		}
	}

	/* panic if we didnt find an addres */
	if (bitmap_phys == 0) {
		panic("PMM: FAILED TO FIND AN ADDRESS FOR BITMAP\n");
	}

	bitmap = (uint8_t*)(bitmap_phys + hhdm->offset);
	kprintf("PMM BITMAP ADDR: %#lx\n", bitmap);

	/* set all to used */
	memset(bitmap, 0xFF, bitmap_size);

	/* free available memory so we can use them */
	for (size_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *mmap_entry = mmap->entries[i];
		if (mmap_entry->type == LIMINE_MEMMAP_USABLE) {
			pmm_free_page(mmap_entry->base, mmap_entry->length);
		}
	}

	/* do not alloc in the bitmap region */
	pmm_alloc_page(bitmap_phys, bitmap_size);

	/* reserve null */
	bitmap[0] |= (1 << 0);
}