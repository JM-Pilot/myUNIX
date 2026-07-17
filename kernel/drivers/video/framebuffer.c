#include <stdint.h>
#include <boot/multiboot2.h>
#include <drivers/video/framebuffer.h>

struct multiboot_tag_framebuffer_common framebuffer_main;
static uint32_t *video_ptr;
void framebuffer_init(struct multiboot_tag_framebuffer *tag)
{
	framebuffer_main = tag->common;
	video_ptr = (uint32_t*)(uintptr_t)framebuffer_main.framebuffer_addr;
}
void framebuffer_put_px(uint32_t col, uint32_t x, uint32_t y)
{
	if (x >= framebuffer_main.framebuffer_width) return;
	if (y >= framebuffer_main.framebuffer_height) return;
	video_ptr[y * (framebuffer_main.framebuffer_pitch / 4) + x] = col;
}
void framebuffer_clear(uint32_t col)
{
	for (uint32_t y = 0; y < framebuffer_main.framebuffer_height; y++)
		for (uint32_t x = 0; x < framebuffer_main.framebuffer_width; x++)
			framebuffer_put_px(col, x, y);
}

uint32_t framebuffer_get_pixcol(uint32_t x, uint32_t y)
{
	return video_ptr[y * (framebuffer_main.framebuffer_pitch / 4) + x];
}