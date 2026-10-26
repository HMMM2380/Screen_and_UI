#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "container_of.h"

struct list_head
{
	struct list_head *next;
	struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT (name)

static inline void
__list_add (struct list_head *new, struct list_head *prev,
            struct list_head *next)
{
	if (!new || !prev || !next)
		return;

	next->prev = new;
	prev->next = new;
	new->next  = next;
	new->prev  = prev;
}

static inline void
list_add (struct list_head *new, struct list_head *head)
{
	__list_add (new, head, head->next);
}

static inline void
list_add_tail (struct list_head *new, struct list_head *head)
{
	__list_add (new, head->prev, head);
}

static inline int
list_is_head (struct list_head *list, struct list_head *head)
{
	return list == head;
}

#define list_entry(pos, type, member) container_of (pos, type, member)

#define list_entry_first(pos, type, member)                                   \
	list_entry ((pos)->next, type, member)

#define list_next_entry(pos, member)                                          \
	list_entry ((pos)->member.next, typeof (*pos), member)

#define list_for_each(pos, head)                                              \
	for ((pos) = (head)->next; !list_is_head ((pos), (head));             \
	     (pos) = (pos)->next)

#define list_entry_is_head(pos, head, member) (&(pos)->member == (head))

#define list_for_each_entry(pos, head, member)                                \
	for ((pos) = list_entry_first (head, typeof (*pos), member);          \
	     !list_entry_is_head (pos, head, member);                         \
	     pos = list_next_entry (pos, member))

#ifdef __cplusplus
}
#endif

#endif
