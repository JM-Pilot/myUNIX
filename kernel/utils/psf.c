#include <utils/psf.h>
#include <drivers/video/framebuffer.h>
#include <stdint.h>

void psf_init(struct psf_font *font, uint8_t *font_data)
{

	/* check if its either psf1 or psf2 */
	if (font_data[0] == PSF1_MAGIC1 && 
		font_data[1] == PSF1_MAGIC2) 
	{
		font->type = PSF_TYPE_V1;
		font->psf1 = (struct psf1_header*)font_data;
		font->font_data = font_data;
	} 
	else if (font_data[0] == PSF2_MAGIC1 &&
		font_data[1] == PSF2_MAGIC2 &&
		font_data[2] == PSF2_MAGIC3 &&
		font_data[3] == PSF2_MAGIC4) 
	{
		font->type = PSF_TYPE_V2;
		font->psf2 = (struct psf2_header*)font_data;
		font->font_data = font_data;
	}
	else {
		font->type = PSF_TYPE_V1;
		font->psf1 = (struct psf1_header*)DEFAULT_FONT;
		font->font_data = DEFAULT_FONT;
	}
}

void psf_putc(uint8_t c, struct psf_font *font, 
		uint32_t x, uint32_t y, 
		uint32_t fg, uint32_t bg)
{
	if (font->type == PSF_TYPE_V1) {
		uint8_t *glyph = (uint8_t*)font->font_data + sizeof(struct psf1_header) + (c * font->psf1->glyph_size);

		for (int gy = 0; gy < font->psf1->glyph_size; gy++) {
			uint8_t byte = glyph[gy];
			for (int gx = 0; gx < 8; gx++) {
				if (byte & (0x80 >> gx))
					framebuffer_plot(fg, x + gx, y + gy);
				else
					framebuffer_plot(bg, x + gx, y + gy);
			}
		}	
	}
	else if (font->type == PSF_TYPE_V2) {
		uint8_t *glyph = (uint8_t*)font->font_data + font->psf2->header_size + (c * font->psf2->glyph_size);

		uint32_t bpg_line = (font->psf2->width + 7) / 8;
		for (uint32_t gy = 0; gy < font->psf2->height; gy++) {
			uint8_t *cur_byte = glyph + (bpg_line * gy);
			uint8_t mask = 1 << 7;
			for (uint32_t gx = 0; gx < font->psf2->width; gx++) {
				if (*cur_byte & mask)
					framebuffer_plot(fg, x + gx, y + gy);
				else
					framebuffer_plot(bg, x + gx, y + gy);
				mask >>= 1;
				if (mask == 0) {
					mask = 1 << 7;
					cur_byte += 1;
				}	
			}
		}
	}
}
