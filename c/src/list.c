/*
  Copyright (c) 2012 Martin Sustrik  All rights reserved.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/

#include <stddef.h>

#include "list.h"
#include "err.h"
#include "attr.h"

void frm_list_init (struct frm_list *self)
{
  self->first = NULL;
  self->last = NULL;
}

void frm_list_term (struct frm_list *self)
{
  frm_assert (self->first == NULL);
  frm_assert (self->last == NULL);
}

int frm_list_empty (struct frm_list *self)
{
  return self->first ? 0 : 1;
}

struct frm_list_item *frm_list_begin (struct frm_list *self)
{
  return self->first;
}

struct frm_list_item *frm_list_end (NN_UNUSED struct frm_list *self)
{
  return NULL;
}

struct frm_list_item *frm_list_prev (struct frm_list *self,
                                   struct frm_list_item *it)
{
  if (!it)
    return self->last;
  frm_assert (it->prev != FRAMER_LIST_NOTINLIST);
  return it->prev;
}

struct frm_list_item *frm_list_next (NN_UNUSED struct frm_list *self,
                                   struct frm_list_item *it)
{
  frm_assert (it->next != FRAMER_LIST_NOTINLIST);
  return it->next;
}

void frm_list_insert (struct frm_list *self, struct frm_list_item *item,
                     struct frm_list_item *it)
{
  frm_assert (!frm_list_item_isinlist (item));

  item->prev = it ? it->prev : self->last;
  item->next = it;
  if (item->prev)
    item->prev->next = item;
  if (item->next)
    item->next->prev = item;
  if (!self->first || self->first == it)
    self->first = item;
  if (!it)
    self->last = item;
}

struct frm_list_item *frm_list_erase (struct frm_list *self,
                                    struct frm_list_item *item)
{
  struct frm_list_item *next;

  frm_assert (frm_list_item_isinlist (item));

  if (item->prev)
    item->prev->next = item->next;
  else
    self->first = item->next;
  if (item->next)
    item->next->prev = item->prev;
  else
    self->last = item->prev;

  next = item->next;

  item->prev = FRAMER_LIST_NOTINLIST;
  item->next = FRAMER_LIST_NOTINLIST;

  return next;
}

void frm_list_item_init (struct frm_list_item *self)
{
  self->prev = FRAMER_LIST_NOTINLIST;
  self->next = FRAMER_LIST_NOTINLIST;
}

void frm_list_item_term (struct frm_list_item *self)
{
  frm_assert (!frm_list_item_isinlist (self));
}

int frm_list_item_isinlist (struct frm_list_item *self)
{
  return self->prev == FRAMER_LIST_NOTINLIST ? 0 : 1;
}

