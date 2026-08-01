#include <drivers/timers/hpet.h>
#include <mm/vmm.h>
#include <stdint.h>
#include <boot/requests.h>
static volatile uint64_t *hpet_base;
static uint32_t clk_period = 0; /* Femtosecs per tick */

/* Initializes the High Precision Event Timer */
void hpet_init(struct hpet_header *hpet)
{
	if (hpet->addr.addr_space_id != 0) return;

	/* MAP */
	uint64_t phys_addr = hpet->addr.addr;
	
	uint64_t aligned_phys = phys_addr & ~(PAGE_SIZE - 1);
	uint64_t offset = phys_addr & (PAGE_SIZE - 1);

	uint64_t hhdm_offset = hhdm_request.response->offset;
	uint64_t virt = aligned_phys + hhdm_offset;

	vmm_map_page(virt, aligned_phys, PTE_PRESENT | PTE_MMIO_FLAGS);

	hpet_base = (volatile uint64_t*)(virt + offset);

	/* Disable and Initialize */
	hpet_base[0x10 / 8] &= ~1ULL;

	clk_period = (uint32_t)(hpet_base[0x000 / 8] >> 32);

	hpet_base[0xF0 / 8] = 0;
	hpet_base[0x10 / 8] |= 1ULL;
}

/* sleep for microsecs */
void hpet_sleep_us(uint64_t us)
{
	if (!hpet_base || clk_period == 0) return;
	uint64_t total_fsec = us * 1000000000ULL;

	/* ticks to wait */
	uint64_t ticks_to_wait = total_fsec / clk_period;

	uint64_t start_tick = hpet_base[0xF0 / 8];

	while ((hpet_base[0xF0 / 8] - start_tick) < ticks_to_wait) {
		__asm__ volatile("pause");
	}
}

/* sleep for milisecs */
void hpet_sleep_ms(uint64_t ms)
{
	hpet_sleep_us(ms * 1000);
}

uint64_t hpet_get_ticks(void)
{
	if (!hpet_base) return 0;
	return hpet_base[0x0F0 / 8];
}