#include <stdint.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/psf.h>
#include <drivers/console/console.h>
#include <string.h>
/* initialize a console */
void console_init(struct console **con, uint8_t *font, int tabwidth, 
	uint32_t fg, uint32_t bg, bool cursor_on)
{
	(*con)->fg = fg;
	(*con)->bg = bg;

	(*con)->tabwidth = tabwidth;
	(*con)->psf_file_type = psf_check(font);
	if ((*con)->psf_file_type == IS_PSF1) {
		psf1_init(&(*con)->fontv1, font);
		(*con)->cellsx = framebuffer_main.framebuffer_width / 8;
		(*con)->cellsy = framebuffer_main.framebuffer_height / (*con)->fontv1->glyph_size;
	} else if ((*con)->psf_file_type == IS_PSF2) {
		psf2_init(&(*con)->fontv2, font);
		(*con)->cellsx = framebuffer_main.framebuffer_width / (*con)->fontv2->width;
		(*con)->cellsy = framebuffer_main.framebuffer_height / (*con)->fontv2->height;
	} else {
		(*con)->psf_file_type = IS_PSF2;
		psf2_init(&(*con)->fontv2, FONT_LAT2_TERMINUS16);
		(*con)->cellsx = framebuffer_main.framebuffer_width / (*con)->fontv2->width;
		(*con)->cellsy = framebuffer_main.framebuffer_height / (*con)->fontv2->height;
	}
	(*con)->cx = 0;
	(*con)->cy = 0;
	(*con)->cursor_on = cursor_on;
	(*con)->font = font;
}

/* scrolls up the framebuffer using the font height */
static void scroll_up(struct console *con)
{
	int font_height = (con->psf_file_type == IS_PSF2 ? con->fontv2->height : con->fontv1->glyph_size);
	uint8_t *addr = (uint8_t*)(uintptr_t)framebuffer_main.framebuffer_addr;
	uint32_t pitch = framebuffer_main.framebuffer_pitch;
	uint32_t height = framebuffer_main.framebuffer_height;
	/* move up */
	memmove(addr, addr + pitch * font_height, pitch * (height - font_height));
	/* clear last row */
	memset(addr + pitch * (height - font_height), 0, pitch * font_height);
}

static void draw_cursor(struct console *con)
{
	if (con->cursor_on == false) return;
	uint32_t crx = 0;
	uint32_t cry = 0;
	if (con->psf_file_type == IS_PSF2) {
		crx = con->cx * con->fontv2->width;
		cry = con->cy * con->fontv2->height;
	} else {
		crx = con->cx * 8;
		cry = con->cy * con->fontv1->glyph_size;
	}

	for (uint32_t y = 0; y < (con->psf_file_type == IS_PSF2 ? 
		con->fontv2->height : con->fontv1->glyph_size);
		y++) 
	{
		for (uint32_t x = 0; x < (con->psf_file_type == IS_PSF2 ? 
			con->fontv2->width : 8); 
			x++) 
		{
			uint32_t col = framebuffer_get_pixcol(crx + x, cry + y);
			col ^= 0xFFFFFFFF;
			framebuffer_put_px(col, crx + x, cry + y);
		}
	}
}
static void insert_newline(struct console *con)
{
	con->cx = 0;
	if ((uint32_t)++con->cy >= con->cellsy) {
		scroll_up(con);
		con->cy--;
	}
	draw_cursor(con);
}


/* places a char into screen */
void console_putc(struct console *con, char c)
{
	draw_cursor(con);
	switch (c) {
		case '\n':
			insert_newline(con);
			return;
		case '\r':
			con->cx = 0;
			draw_cursor(con);
			return;
		case '\b':
			if (con->cx == 0) return;
			con->cx--;
			draw_cursor(con);
			return;
		case '\t':
			con->cx = (con->cx + con->tabwidth) & ~(con->tabwidth - 1);
			if ((uint32_t)con->cx >= con->cellsx)
    				insert_newline(con);
			draw_cursor(con);
			return;
	}
	if (con->psf_file_type == IS_PSF2) {
		psf2_render_char(con->fontv2, con->font, c, 
			con->cx * con->fontv2->width, 
			con->cy * con->fontv2->height,
			con->fg, con->bg);
	} else {
		psf1_render_char(con->fontv1, con->font, c,
			con->cx * 8, 
			con->cy * con->fontv1->glyph_size,
			con->fg, con->bg);
	}
	if ((uint32_t)++con->cx >= con->cellsx)
		insert_newline(con);
	draw_cursor(con);
}

/* puts a string to screen */
void console_puts(struct console *con, const char *str)
{
	while (*str)
		console_putc(con, *str++);
}

/* resets the cursor and clears the screen with the bg */
void console_clear(struct console *con)
{
	con->cx = 0;
	con->cy = 0;

	framebuffer_clear(con->bg);
}


void console_disable_cursor(struct console *con)
{
	con->cursor_on = false;
}
void console_enable_cursor(struct console *con)
{
	con->cursor_on = true;
}
