#ifndef OLED_CDRIVER
#define OLED_CDRIVER

#ifdef __cplusplus
extern "C"
{
#endif

#include "fonts.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct OLED
{
	uint8_t *_Gbuf;
	size_t   _Gbuf_size;
	size_t   x_len;
	size_t   y_len;

#ifdef Bin2Dec_BCD_16bit
	BCD_16bit_t bcd_16bit;
#endif

	int (*Send) (void *Gbuf, size_t size);
};

enum OLED_COLOR
{
	OLED_COLOR_MAIN = (uint8_t)0x01,
	OLED_COLOR_NULL = (uint8_t)0x00,
};

static inline const int
OLED_range_isok (struct OLED *oled, const uint16_t x1, const uint16_t x2,
                 const uint16_t y1, const uint16_t y2)
{
	if (!oled || x1 >= oled->x_len || x2 >= oled->x_len
	    || y1 >= oled->y_len || y2 >= oled->y_len)
		return 0;
	else
		return 1;
}

static inline const int
OLED_init (struct OLED *oled, uint8_t *init_cmd, size_t icmd_len, size_t x_len,
           size_t y_len, uint8_t *Gbuf, int (*send_func) (void *, size_t))
{
	const size_t gbuf_size = x_len * (y_len >> 3);

	memset (oled, 0, sizeof (struct OLED));
	memset (Gbuf, 0, gbuf_size);

	oled->_Gbuf      = Gbuf;
	oled->_Gbuf_size = gbuf_size;
	oled->x_len      = x_len;
	oled->y_len      = y_len;
	oled->Send       = send_func;

	if (!init_cmd || !icmd_len)
		return 0;
	return oled->Send (init_cmd, icmd_len);
}

static inline const int
OLED_flush_all (struct OLED *oled)
{
	return oled->Send (oled->_Gbuf, oled->_Gbuf_size);
}

static inline void
OLED_clear_all (struct OLED *oled)
{
	memset (oled->_Gbuf, 0, oled->_Gbuf_size);
}

static inline const int
OLED_pixel (struct OLED *oled, const uint16_t x, const uint16_t y,
            enum OLED_COLOR color)
{
	if (!OLED_range_isok (oled, x, 0, y, 0))
		return 0;

	const uint16_t bit = 0x01 << (y & 0x07U);
	if (color)
		/* y / 8 <=> y >> 3 */
		(oled->_Gbuf)[(y >> 3) * oled->x_len + x] |= bit;
	else
		/* y / 8 <=> y >> 3 */
		(oled->_Gbuf)[(y >> 3) * oled->x_len + x] &= ~bit;

	return 1;
}

static inline const int
OLED_draw_vline (struct OLED *oled, const uint16_t x, const uint16_t y1,
                 const uint16_t y2, enum OLED_COLOR color)
{
	if (!OLED_range_isok (oled, x, 0, y1, y2))
		return 0;

	const uint16_t ys    = y1 < y2 ? y1 : y2;
	const uint16_t ye    = y1 > y2 ? y1 : y2;
	const uint16_t spage = ys >> 3;
	const uint16_t epage = ye >> 3;

	if (0 == (epage - spage))
		{
			const uint8_t bits
			    = (uint8_t)((uint16_t)(0xFF << (ys & 0x07))
			                & (uint16_t)(0xFF
			                             >> (0x07 - (ye & 0x07))));

			if (color)
				(oled->_Gbuf)[spage * oled->x_len + x] |= bits;
			else
				(oled->_Gbuf)[spage * oled->x_len + x]
				    &= ~bits;
		}
	else
		{
			const uint8_t sbits = (uint8_t)(0xFF << (ys & 0x07));
			const uint8_t ebits
			    = (uint8_t)(0xFF >> (0x07 - (ye & 0x07)));

			if (color)
				{
					(oled->_Gbuf)[spage * oled->x_len + x]
					    |= sbits;
					(oled->_Gbuf)[epage * oled->x_len + x]
					    |= ebits;
					for (uint16_t i = spage + 1; i < epage;
					     ++i)
						(oled->_Gbuf)[i * oled->x_len
						              + x] = 0xFF;
				}
			else
				{
					(oled->_Gbuf)[spage * oled->x_len + x]
					    &= ~sbits;
					(oled->_Gbuf)[epage * oled->x_len + x]
					    &= ~ebits;

					for (uint16_t i = spage + 1; i < epage;
					     ++i)
						(oled->_Gbuf)[i * oled->x_len
						              + x] = 0x00;
				}
		}

	return 1;
}

static inline const int
OLED_draw_hline (struct OLED *oled, const uint16_t x1, const uint16_t x2,
                 const uint16_t y, enum OLED_COLOR color)
{
	if (!OLED_range_isok (oled, x1, x2, y, 0))
		return 0;

	const uint16_t xs = x1 < x2 ? x1 : x2;
	const uint16_t xe = x1 > x2 ? x1 : x2;

	const uint16_t bit = 0x01 << (y & 0x07U);
	/* y / 8 <=> y >> 3 */
	const uint16_t page = y >> 3;

	if (color)
		for (uint16_t i = xs; i <= xe; ++i)
			(oled->_Gbuf)[page * oled->x_len + i] |= bit;
	else
		for (uint16_t i = xs; i <= xe; ++i)
			(oled->_Gbuf)[page * oled->x_len + i] &= ~bit;

	return 1;
}

static inline const int
OLED_draw_rect (struct OLED *oled, const uint16_t x1, const uint16_t y1,
                const uint16_t x2, const uint16_t y2, enum OLED_COLOR color)
{
	if (!OLED_range_isok (oled, x1, x2, y1, y2))
		return 0;

	OLED_draw_hline (oled, x1, x2, y1, color);
	OLED_draw_hline (oled, x1, x2, y2, color);
	OLED_draw_vline (oled, x1, y1, y2, color);
	OLED_draw_vline (oled, x2, y1, y2, color);

	return 1;
}

static inline const int
OLED_fill_rect (struct OLED *oled, const uint16_t x1, const uint16_t y1,
                const uint16_t x2, const uint16_t y2, enum OLED_COLOR color)
{
	if (!OLED_range_isok (oled, x1, x2, y1, y2))
		return 0;

	const uint16_t xe = x1 > x2 ? x1 : x2;
	for (uint16_t x = (x1 < x2 ? x1 : x2); x <= xe; ++x)
		OLED_draw_vline (oled, x, y1, y2, color);

	return 1;
}

static inline const char
OLED_putc (struct OLED *oled, char c, const uint16_t x, const uint16_t y,
           const uint8_t font_size)
{
	if (!OLED_range_isok (oled, x, 0, y, 0))
		return -1;

	if (c == '\0' || c == '\n')
		return c;

	const uint8_t *glyph;
	uint8_t        pages = 1;
	uint8_t        cols  = 6;
	switch (font_size)
		{
		case 6:
		default:
			glyph = &char_6x8[0][0] + (uint8_t)(c - ' ') * 6;
			break;
		case 8:
			pages = 2;
			cols  = 8;
			glyph = &char_8x16[0][0];
			break;
		case 16:
			pages = 4;
			cols  = 16;
			glyph = &char_16x32[0][0];
			break;
		}

	uint16_t page_base = (y >> 3) * oled->x_len;
	uint16_t xs        = x;
	if (x >= (oled->x_len - cols))
		{
			page_base += pages * 8;
			xs = 0;
		}
	if (page_base >= (oled->y_len >> 3))
		page_base = 0;

	for (uint16_t i = 0; i < pages; ++i)
		{
			uint16_t page_idx = page_base + i * pages * 8;
			for (uint16_t j = 0; j < cols; ++j)
				oled->_Gbuf[page_idx + j + x]
				    = glyph[j + i * cols];
		}

	return c;
}

static inline const char *
OLED_puts (struct OLED *oled, const char *s, const uint16_t x,
           const uint16_t y, const uint8_t font_size)
{
	if (!OLED_range_isok (oled, x, 0, y, 0))
		return 0;

	const uint8_t *font;
	uint8_t        pages = 1;
	uint8_t        cols  = 6;
	switch (font_size)
		{
		case 6:
		default:
			font = &char_6x8[0][0];
			break;
		case 8:
			pages = 2;
			cols  = 8;
			font  = &char_8x16[0][0];
			break;
		case 16:
			pages = 4;
			cols  = 16;
			font  = &char_16x32[0][0];
			break;
		}

	uint16_t xs = x;
	uint16_t ys = y;
	while ('\0' != *s)
		{
			if (xs >= (oled->x_len - cols))
				{
					ys += pages * 8;
					if (ys >= oled->y_len)
						return s;
					xs = 0;
				}
			if (*s == '\n')
				{
					ys += pages * 8;
					++s;
					if (ys >= oled->y_len || *s == '\0')
						return s;
					xs = 0;
				}

			const uint16_t cidx      = (*s - ' ') * cols * pages;
			const uint16_t page_base = (ys >> 3) * oled->x_len;

			for (uint16_t i = 0; i < pages; ++i)
				{
					uint16_t page_idx
					    = page_base + i * oled->x_len;
					for (uint16_t j = 0; j < cols; ++j)
						oled->_Gbuf[page_idx + j + xs]
						    = font[cidx + j
						           + i * cols];
				}

			++s;
			xs += cols;
		}

	return s;
}

#ifdef __cplusplus
}
#endif

#endif
