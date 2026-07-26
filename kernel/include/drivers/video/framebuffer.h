#ifndef DRIVERS_VIDEO_FRAMEBUFFER_H
#define DRIVERS_VIDEO_FRAMEBUFFER_H

#include <stdint.h>
#include <boot/limine.h>

extern struct limine_framebuffer *framebuffer;

void framebuffer_init(void);
void framebuffer_plot(uint32_t col, uint32_t x, uint32_t y); 
void framebuffer_clear(uint32_t col);
uint32_t framebuffer_get_col(uint32_t x, uint32_t y);

#endif /* DRIVERS_VIDEO_FRAMEBUFFER_H */
