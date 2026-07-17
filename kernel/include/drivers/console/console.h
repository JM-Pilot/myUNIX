#include <stdint.h>
#include <stdbool.h>
#include <drivers/video/psf.h>
#include <kfonts.h>
#ifndef DRIVERS_CONSOLE_H
#define DRIVERS_CONSOLE_H

struct console {
	int cx, cy;
	int tabwidth;
	uint32_t fg, bg;
	uint32_t cellsx, cellsy;
	struct psf2_header *fontv2;
	struct psf1_header *fontv1;
	enum psf_file_bool psf_file_type;
	uint8_t *font; /* use fonts defined in kfont.h */
	bool cursor_on;
};

void console_init(struct console **con, uint8_t *font, int tabwidth, 
	uint32_t fg, uint32_t bg, bool cursor_on);
void console_putc(struct console *con, char c);
void console_puts(struct console *con, const char *str);
void console_clear(struct console *con);
void console_disable_cursor(struct console *con);
void console_enable_cursor(struct console *con);


#endif /* DRIVERS_CONSOLE_H */