#ifndef DRIVERS_TIMERS_PIT_H
#define DRIVERS_TIMERS_PIT_H

#include <stdint.h>

/* we will use 0x40 */

#define PIT_CHANNEL 0x40 
#define PIT_COMMAND 0x43
#define PIT_FREQUENCY 1193182
#define PIT_HZ 250

void pit_set_hz(uint32_t hz);
uint32_t pit_get_ticks(void);
void pit_init(void);
void pit_wait(uint32_t ms);

#endif /* DRIVERS_TIMERS_PIT_H */
