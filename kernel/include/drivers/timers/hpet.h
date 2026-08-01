#ifndef DRIVERS_TIMERS_HPET_H
#define DRIVERS_TIMERS_HPET_H

#include <acpi/acpi.h>

struct hpet_addr_struct {
	uint8_t addr_space_id;
	uint8_t reg_bit_width;
	uint8_t reg_bit_offset;
	uint8_t reserved;
	uint64_t addr;
}__attribute__((packed));

struct hpet_header {
	struct sdt_header sdt;
	uint32_t event_timer_blk_id;
	struct hpet_addr_struct addr;
	uint8_t hpet_num;
	uint16_t min_ticks;
	uint8_t page_protection;
}__attribute__((packed));

void hpet_init(struct hpet_header *hpet);
void hpet_sleep_us(uint64_t us);
void hpet_sleep_ms(uint64_t ms);
uint64_t hpet_get_ticks(void);

#endif /* DRIVERS_TIMERS_HPET_H */