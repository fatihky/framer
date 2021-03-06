/*
  Copyright (c) 2016 Fatih Kaya  All rights reserved.
  Copyright (c) 2016 Bent Cardan  All rights reserved.

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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "err.h"
#include "framer.h"

struct frm_frame *frm_frame_new()
{
  struct frm_frame *fr = calloc (1, sizeof (struct frm_frame));

  if (frm_slow (!fr))
    return NULL;

  frm_frame_init (fr);

  return fr;
}

void frm_frame_init (struct frm_frame *self)
{
  self->size = 0;
  self->buf = NULL;
  self->cursor = 0;
  self->refcnt = 1;
  self->type = FRAMER_FRAME_EMPTY;
  self->allocator = NULL;

  frm_list_item_init (&self->item);
}

int frm_frame_set_data (struct frm_frame *self, char *data, uint32_t size)
{
  frm_assert (self->buf == NULL);
  frm_assert (self->type == FRAMER_FRAME_EMPTY);

  self->buf = malloc (size);

  if (frm_slow (self->buf == NULL))
    return ENOMEM;

  memcpy (self->buf, data, size);

  self->size = size;
  self->type = FRAMER_FRAME_ALLOCATED;

  return 0;
}

int frm_frame_ref (struct frm_frame *self)
{
  return __sync_fetch_and_add (&self->refcnt, 1) + 1;
}

int frm_frame_get_refcnt (struct frm_frame *self)
{
  return __sync_fetch_and_add (&self->refcnt, 0);
}

int frm_frame_term (struct frm_frame *self)
{
  frm_assert (frm_frame_get_refcnt (self) >= 1);

  if (__sync_fetch_and_sub (&self->refcnt, 1) > 1)
    return frm_frame_get_refcnt (self);

  frm_list_item_term (&self->item);

  if (frm_fast (self->type == FRAMER_FRAME_EMBEDDED))
    frm_cbuf_unref (self->cbuf);

  if (frm_fast (self->type == FRAMER_FRAME_ALLOCATED))
    free (self->buf);

  if (self->allocator && self->allocator->destroy_fn)
    self->allocator->destroy_fn (self->allocator->data, self);

  return 0;
}

size_t frm_frame_totlen (struct frm_frame *self)
{
  return 4 + self->size;
}

void frm_frame_encode (char *dst, struct frm_frame *src)
{
  *(uint32_t *)dst = src->size;
  memcpy (dst + 4, src->buf, src->size);
}
