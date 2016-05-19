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

#include <stdlib.h>
#include "framer.h"
#include "frame-list.h"
#include "cont.h"

/*  allocate new object */
struct frm_out_frame_list_item *frm_out_frame_list_item_new()
{
  struct frm_out_frame_list_item *self = malloc(
    sizeof (struct frm_out_frame_list_item));

  if (self == NULL)
    return NULL;

  frm_out_frame_list_item_init (self);

  return self;
}

/*  deallocate allocated object */
void frm_out_frame_list_item_destroy (struct frm_out_frame_list_item *self)
{
  frm_out_frame_list_item_term (self);
  free (self);
}

/*  initialize the object */
void frm_out_frame_list_item_init (struct frm_out_frame_list_item *self)
{
  frm_list_item_init (&self->item);
  self->frame = NULL;
  self->cursor = 0;
}

void frm_out_frame_list_item_set_frame (struct frm_out_frame_list_item *self,
  struct frm_frame *frame)
{
  self->frame = frame;
  frm_frame_ref (frame);
}

/*  de-init the object and frame of the item.
    this function called by this library inside `frm_out_frame_list_written`
    function. */
void frm_out_frame_list_item_term (struct frm_out_frame_list_item *self)
{
  frm_list_item_term (&self->item);

  if (self->frame)
    frm_frame_term (self->frame);
}


/*
  Out Frames List
*/

/*  allocate new object */

struct frm_out_frame_list *frm_out_frame_list_new()
{
  struct frm_out_frame_list *self = malloc(
    sizeof (struct frm_out_frame_list));

  if (self == NULL)
    return NULL;

  frm_out_frame_list_init (self);

  return self;
}

/*  deallocate allocated object */
void frm_out_frame_list_destroy (struct frm_out_frame_list *self)
{
  frm_out_frame_list_term (self);
  free (self);
}

/*  initialize the object */
void frm_out_frame_list_init (struct frm_out_frame_list *self)
{
  self->out_index = 0;
  frm_list_init (&self->list);
}

/*  de-init the object. */
void frm_out_frame_list_term (struct frm_out_frame_list *self)
{
  frm_list_term (&self->list);
}

/*  add new element to the list */
void frm_out_frame_list_insert (struct frm_out_frame_list *self,
  struct frm_out_frame_list_item *elem)
{
  frm_list_insert (&self->list, &elem->item, frm_list_end (&self->list));
}

/*  get output buffers */
ssize_t frm_out_frame_list_get_iovs (struct frm_out_frame_list *self,
    struct iovec *iovs, int iovcnt, int *retiovcnt)
{

  int totiovs = 0;
  ssize_t totsz = 0;
  struct frm_list_item *item;

  item = frm_list_begin (&self->list);

  if (item == NULL)
    return 0;

  for (int index = 0;
       totiovs < iovcnt && item != frm_list_end (&self->list);
       item = frm_list_next (&self->list, item), index++) {

    if (index < self->out_index)
      continue;

    struct frm_out_frame_list_item *listItem = frm_cont(item,
      struct frm_out_frame_list_item, item);
    struct frm_frame *frame = listItem->frame;

    if (listItem->cursor < 4) {
      iovs[totiovs].iov_base = (char *)&frame->size + listItem->cursor;
      iovs[totiovs].iov_len = 4 - listItem->cursor;
      totsz += iovs[totiovs].iov_len;
      totiovs++;
    }

    if (totiovs >= iovcnt)
      break;

    ssize_t len = frame->size - (listItem->cursor > 4 ? listItem->cursor - 4 : 0);
    ssize_t offset = frame->size > len ? frame->size - len : 0;
    iovs[totiovs].iov_base = (char *)frame->buf + offset;
    iovs[totiovs].iov_len = len;
    totsz += len;
    totiovs++;
  }

  *retiovcnt = totiovs;
  self->out_index += totiovs / 2; /*  FIXME: remove this property */
  return totsz;
}

void frm_out_frame_list_reset_out_index (struct frm_out_frame_list *self)
{
  self->out_index = 0;
}

/*  set written bytes */
int frm_out_frame_list_written (struct frm_out_frame_list *self,
  ssize_t written_)
{
  int removed = 0;
  ssize_t written = written_;
  struct frm_list_item *item;

  item = frm_list_begin (&self->list);
  while (item && written > 0) {
    /* out list item */
    struct frm_out_frame_list_item *olitem;
    ssize_t must_written;

    olitem = frm_cont (item, struct frm_out_frame_list_item, item);
    must_written = (olitem->frame->size + 4) - olitem->cursor;

    if (must_written <= written) {
      written -= must_written;
      /*  remove frame from the list */
      item = frm_list_erase (&self->list, item);
      /*  FIXME: destroy frame here */
      self->out_index--;
      /*  destroy out list item */
      frm_out_frame_list_item_destroy (olitem);
      removed++;
    }
    else { // to_write > written
      olitem->cursor += written;
      break;
    }

    item = frm_list_begin (&self->list);
  }

  return removed;
}
