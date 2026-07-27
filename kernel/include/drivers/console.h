#ifndef DRIVERS_CONSOLE_H
#define DRIVERS_CONSOLE_H

#include <stdint.h>
#include <stdbool.h>
#include <utils/psf.h>

/* Console Header
 * @warning other console can take over the screen
 */
struct console {
	uint32_t colfg, colbg;
	uint32_t cursx;
	uint32_t cursy;
	uint32_t maxcx;
	uint32_t maxcy;
	uint8_t tab_width;
	struct psf_font font;
};


/* initializes console (con) 
 * @param font - is the psf data, default DEFAULT_FONT
 * @param fg,bg - colors of background
 * @param tab_width - tab width  
 */
void console_init(struct console *con,
	uint8_t *font,
	uint32_t fg, uint32_t bg, 
	uint8_t tab_width);

/* writes to console */
void console_write(struct console *con, const char *str);

#endif /* DRIVERS_CONSOLE_H */