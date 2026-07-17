#include <drivers/video/psf.h>
#include <drivers/video/framebuffer.h>
#include <stdint.h>
#include <stdbool.h>

/* check if the file is a PSF
 * returns IS_PSF2 if is PSFv2
 * returns IS_PSF1 if its PSFv1
 * returns IS_NOT_PSF if its not a PSF file
 */

enum psf_file_bool psf_check(uint8_t *data)
{
	/* psf v1 */
	if (data[0] == PSF1_MAGIC1 && data[1] == PSF1_MAGIC2)
		return IS_PSF1;
	/* psf v2 */
	else if (data[0] == PSF2_MAGIC1 && 
		data[1] == PSF2_MAGIC2 &&
		data[2] == PSF2_MAGIC3 &&
		data[3] == PSF2_MAGIC4)
		return IS_PSF2;
	return IS_NOT_PSF;
}

/* initialize a psf1 header
 * returns false if `data` is not psf1
 * returns true if successful
 */
bool psf1_init(struct psf1_header **header, uint8_t *data)
{
	if (psf_check(data) != IS_PSF1)
		return false;

	*header = (struct psf1_header*)data;
	
	if (psf_check((*header)->magic) != IS_PSF1)
		return false;
	return true;
}


void psf1_render_char(struct psf1_header* header,  
	uint8_t *data, uint8_t c,
	uint32_t x, uint32_t y,
	uint32_t fg, uint32_t bg)
{
	uint8_t *glyph = (uint8_t*)data + sizeof(struct psf1_header) + (c * header->glyph_size);

	for (int gy = 0; gy < header->glyph_size; gy++) {
		uint8_t byte = glyph[gy];
		for (int gx = 0; gx < 8; gx++) {
			if (byte & (0x80 >> gx))
				framebuffer_put_px(fg, x + gx, y + gy);
			else
				framebuffer_put_px(bg, x + gx, y + gy);
		}
	}	 
}

/* initialize a psf2 header
 * returns false if `data` is not psfv2
 * returns true if successful
 */
bool psf2_init(struct psf2_header **header, uint8_t *data)
{
	if (psf_check(data) != IS_PSF2)
		return false;
	*header = (struct psf2_header *)data;
	if (psf_check((*header)->magic) != IS_PSF2)
		return false;
	return true;
}

void psf2_render_char(struct psf2_header *header, 
	uint8_t *data, uint8_t c,
	uint32_t x, uint32_t y, 
	uint32_t fg, uint32_t bg)
{
	uint8_t *glyph = (uint8_t*)data + header->header_size + (c * header->glyph_size);

	uint32_t bpg_line = (header->width + 7) / 8;
	for (uint32_t gy = 0; gy < header->height; gy++) {
		uint8_t *cur_byte = glyph + (bpg_line * gy);
		uint8_t mask = 1 << 7;
		for (uint32_t gx = 0; gx < header->width; gx++) {
			if (*cur_byte & mask)
				framebuffer_put_px(fg, x + gx, y + gy);
			else
				framebuffer_put_px(bg, x + gx, y + gy);
			mask >>= 1;
			if (mask == 0) {
				mask = 1 << 7;
				cur_byte += 1;
			}	
		}
	}
}