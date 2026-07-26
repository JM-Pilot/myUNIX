#include <stdint.h>
#include <boot/limine.h>
#include <drivers/video/framebuffer.h>
#include <boot/requests.h>

struct limine_framebuffer *framebuffer;
static uint32_t *video;

/* Initialize the framebuffer */
void framebuffer_init(void)
{
	framebuffer = framebuffer_request.response->framebuffers[0];
	video = (uint32_t*)(uintptr_t)framebuffer->address;
}

/* writes to framebuffer 
 * 'col' doesnt have alpha channel so you use RRGGBB
 */
void framebuffer_plot(uint32_t col, uint32_t x, uint32_t y)
{
	video[y * (framebuffer->pitch / 4) + x] = col;
}

/* fills the framebuffer with the color (col) */
void framebuffer_clear(uint32_t col)
{
	for (uint32_t y = 0; y < framebuffer->height; y++) {
		for (uint32_t x = 0; x < framebuffer->width; x++) {
			framebuffer_plot(col, x, y);
		}
	}
}

/* returns the value of the color in the framebuffer */
uint32_t framebuffer_get_col(uint32_t x, uint32_t y)
{
	return video[y * (framebuffer->pitch / 4) + x];
}
