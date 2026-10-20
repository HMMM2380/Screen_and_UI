#ifndef OLED_LCUI_H
#define OLED_LCUI_H

#include "OLED_Cdriver.h"
#include <stdint.h>

#define OLED_LCUI_MAX_ITEM_NUM (0x08)

struct OLED_lCUI_item
{
	void* item;
	void (*cb) (void*);
};

enum OLED_lCUI_cb_num
{
	OLED_lCUI_PROGRESSBAR = 0,
};

struct OLED_lCUI_progress_bar
{
	size_t          progress;
	enum OLED_COLOR color;

	uint16_t px;
	uint16_t xs;
	uint16_t ys;
	uint16_t xe;
	uint16_t ye;

	struct OLED* oled;
};

extern struct OLED_lCUI_item OLED_lCUI_all_items[OLED_LCUI_MAX_ITEM_NUM];

static inline void
OLED_lCUI_items_handler (struct OLED* oled)
{
	for (uint16_t idx = 0; idx < OLED_LCUI_MAX_ITEM_NUM; ++idx)
		{
			struct OLED_lCUI_item* item
			    = OLED_lCUI_all_items + idx;
			item->cb (item->item);
		}
	OLED_flush_all (oled);
}

#endif
