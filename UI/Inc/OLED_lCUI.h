#ifndef OLED_LCUI_H
#define OLED_LCUI_H

#include "OLED_Cdriver.h"
#include "list.h"
#include <stdint.h>

struct OLED_lCUI_item
{
	void *item;
	void (*cb) (void *);
	struct list_head list;
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

	struct OLED *oled;
};

static inline void
OLED_lCUI_item_add_next (struct OLED_lCUI_item *new,
                         struct list_head *items_list)
{
	list_add (&new->list, items_list);
}

static inline void
OLED_lCUI_item_add_tail (struct OLED_lCUI_item *new,
                         struct list_head *items_list)
{
	list_add_tail (&new->list, items_list);
}

static inline void
OLED_lCUI_items_handler (struct OLED *oled, struct list_head *items_list)
{

	struct OLED_lCUI_item *i;
	list_for_each_entry (i, items_list, list)
	{
		if (i->cb && i->item)
			i->cb (i->item);
	}

	OLED_flush_all (oled);
}

#endif
