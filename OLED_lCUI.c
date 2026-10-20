#include "OLED_lCUI.h"
#include "OLED_Cdriver.h"
#include <stdint.h>
#include <string.h>

void
OLED_lCUI_progress_bar_init (struct OLED*                   oled,
                             struct OLED_lCUI_progress_bar* pg_b,
                             const uint16_t x1, const uint16_t y1,
                             const uint16_t x2, const uint16_t y2,
                             enum OLED_COLOR color)
{
	if (!oled || !pg_b || !OLED_range_isok (oled, x1, x2, y1, y2))
		return;
	const struct OLED_lCUI_progress_bar pb = { .progress = 0,
		                                   .color    = color,
		                                   .xs   = (x1 < x2 ? x1 : x2),
		                                   .xe   = (x1 > x2 ? x1 : x2),
		                                   .ys   = (y1 < y2 ? y1 : y2),
		                                   .ye   = (y1 > y2 ? y1 : y2),
		                                   .oled = oled };
	memcpy (pg_b, &pb, sizeof (struct OLED_lCUI_progress_bar));
	pg_b->px = pb.xs;

	OLED_draw_rect (oled, pb.xs, pb.ys, pb.xe, pb.ye, color);
}

void
OLED_lCUI_progress_bar_update (struct OLED_lCUI_progress_bar* pg_b,
                               uint8_t                        progress)
{
	if (!pg_b || progress > 100)
		return;
	pg_b->progress = progress;
}

void
OLED_lCUI_progress_bar_destroy (struct OLED_lCUI_progress_bar* pg_b)
{
	if (!pg_b)
		return;
	OLED_fill_rect (pg_b->oled, pg_b->xs, pg_b->ys, pg_b->xe, pg_b->ye, 0);
}

void
OLED_lCUI_progress_bar_cb (void* pg_b)
{
	if (!pg_b)
		return;

	struct OLED_lCUI_progress_bar b;
	memcpy (&b, pg_b, sizeof (struct OLED_lCUI_progress_bar));

	const uint16_t bar_len = b.xe - b.xs;
	if (b.progress == 100)
		OLED_lCUI_progress_bar_destroy (
		    (struct OLED_lCUI_progress_bar*)pg_b /* destroy the bar */
		);

	const uint16_t new_px = b.progress * bar_len / 100;
	OLED_fill_rect (b.oled, b.px, b.ys, new_px, b.ye, b.color);

	b.px += new_px;
	((struct OLED_lCUI_progress_bar*)pg_b)->px = b.px;
}
