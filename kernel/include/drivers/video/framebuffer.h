#include <boot/multiboot2.h>
#include <stdint.h>
#ifndef DRIVERS_VIDEO_FRAMEBUFFER_H
#define DRIVERS_VIDEO_FRAMEBUFFER_H

extern struct multiboot_tag_framebuffer_common framebuffer_main;
void framebuffer_init(struct multiboot_tag_framebuffer *tag);
void framebuffer_put_px(uint32_t col, uint32_t x, uint32_t y);
void framebuffer_clear(uint32_t col);
uint32_t framebuffer_get_pixcol(uint32_t x, uint32_t y);

#endif /* DRIVERS_VIDEO_FRAMEBUFFER_H */