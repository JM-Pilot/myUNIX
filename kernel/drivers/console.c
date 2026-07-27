#include <stdint.h>
#include <string.h>
#include <drivers/console.h>
#include <drivers/video/framebuffer.h>
#include <utils/psf.h>

void console_init(struct console *con, 
	uint8_t *font,
	uint32_t fg, uint32_t bg, 
	uint8_t tab_width)
{
	con->colbg = bg;
	con->colfg = fg;
	con->cursx = 0;
	con->cursy = 0;
	con->tab_width = tab_width;
	psf_init(&con->font, font);
	con->maxcx = framebuffer->width / (con->font.type == PSF_TYPE_V1 ? 
					8 : con->font.psf2->width);
	con->maxcy = framebuffer->height / (con->font.type == PSF_TYPE_V1 ? 
					con->font.psf1->glyph_size : con->font.psf2->height);
}

/* scrolls up the framebuffer using the font height and clear the last row
 * helper function for _newline
 */
static void _scroll_up(struct console *con)
{
	int font_height = (con->font.type == PSF1_MAGIC2 ? con->font.psf2->height : con->font.psf1->glyph_size);
	uint8_t *addr = (uint8_t*)(uintptr_t)framebuffer->address;
	uint32_t pitch = framebuffer->pitch;
	uint32_t height = framebuffer->height;
	/* move up */
	memmove(addr, addr + pitch * font_height, pitch * (height - font_height));
	/* clear last row */
	memset(addr + pitch * (height - font_height), 0, pitch * font_height);
}
/* helper function to _putc */
static void _newline(struct console *con)
{
	con->cursx = 0;
	if (++(con->cursy) >= con->maxcy) {
		_scroll_up(con);
		con->cursy--;
	}
}
/* helper function for console_write */
static void _putc(struct console *con, char c)
{
	struct psf_font font = con->font;
	switch (c)
	{
		case '\n':
			_newline(con);
			return;
		case '\r':
			con->cursx = 0;
			return;
		case '\t':
			con->cursx += con->tab_width - (con->cursx % con->tab_width);
			return;
		case '\b':
			if (con->cursx == 0) return;
			con->cursx--;
			return;
		break;
	}
	psf_putc(c, &font, 
		con->cursx * (font.type == PSF_TYPE_V1 ? 8 : font.psf2->width), 
		con->cursy * (font.type == PSF_TYPE_V1 ? font.psf1->glyph_size : font.psf2->height), 
		con->colfg, con->colbg);
	
	if (++(con->cursx) >= con->maxcx)
		_newline(con);
}
/* writes to console */
void console_write(struct console *con, const char *str)
{
	while (*str)
		_putc(con, *str++);
}