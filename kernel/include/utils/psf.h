#ifndef UTILS_PSF_H
#define UTILS_PSF_H

#include <stdint.h>

extern uint8_t _binary_assets_fonts_ter_u16n_psf_start[];
#define DEFAULT_FONT _binary_assets_fonts_ter_u16n_psf_start

/* psf1 magic */
#define PSF1_MAGIC1 0x36
#define PSF1_MAGIC2 0x04

/* psf1 modes */
#define PSF1_MODE512 0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODESEQ 0x04

struct psf1_header {
	uint8_t magic[2];
	uint8_t mode;
	uint8_t glyph_size;
};

/* psf2 magic */
#define PSF2_MAGIC1 0x72
#define PSF2_MAGIC2 0xB5
#define PSF2_MAGIC3 0x4A
#define PSF2_MAGIC4 0x86

/* psf2 flag */
#define PSF2_HAS_UNICODE_TABLE 0x1

struct psf2_header {
	uint8_t magic[4];
	uint32_t version;
	uint32_t header_size;
	uint32_t flags;
	uint32_t length;
	uint32_t glyph_size;
	uint32_t height;
	uint32_t width;
};

/* psf types, we will use this below */
enum psf_type {
	PSF_TYPE_V1 = 0x1,
	PSF_TYPE_V2 = 0x2
};

/* psf_font to make our lives eazier */
struct psf_font {
	enum psf_type type;
	struct psf1_header *psf1;
	struct psf2_header *psf2;
	uint8_t *font_data;
};

void psf_init(struct psf_font *font, uint8_t *font_data);
void psf_putc(uint8_t c, struct psf_font *font, 
		uint32_t x, uint32_t y, 
		uint32_t fg, uint32_t bg);
#endif /* UTILS_PSF_H */
