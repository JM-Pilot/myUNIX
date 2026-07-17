#include <stdint.h>
#include <stdbool.h>
#ifndef DRIVERS_VIDEO_PSF_H
#define DRIVERS_VIDEO_PSF_H

enum psf_file_bool {
	IS_PSF1 = 0,
	IS_PSF2 = 1,
	IS_NOT_PSF = 3,
};

enum psf_file_bool psf_check(uint8_t *data);

#define PSF1_MODE512 0x1
#define PSF1_MODEHASTAB 0x2
#define PSF1_MODESEQ 0x4

#define PSF1_MAGIC1 0x36
#define PSF1_MAGIC2 0x04

struct psf1_header {
	uint8_t magic[2];
	uint8_t mode;
	uint8_t glyph_size;
};

bool psf1_init(struct psf1_header **header, uint8_t *data);
void psf1_render_char(struct psf1_header* header,  
	uint8_t *data, uint8_t c,
	uint32_t x, uint32_t y,
	uint32_t fg, uint32_t bg);
#define PSF2_HAS_UNICODE_TABLE 0x1
#define PSF2_MAGIC1 0x72
#define PSF2_MAGIC2 0xB5
#define PSF2_MAGIC3 0x4A
#define PSF2_MAGIC4 0x86

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

bool psf2_init(struct psf2_header **header, uint8_t *data);
void psf2_render_char(struct psf2_header *header, 
	uint8_t *data, uint8_t c,
	uint32_t x, uint32_t y, 
	uint32_t fg, uint32_t bg);

#endif /* DRIVERS_VIDEO_PSF_H */