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

#include <sys/uio.h>

#ifndef FRM_FRAME_LIST_H
#define FRM_FRAME_LIST_H

struct frm_out_frame_list_item {
  struct frm_list_item item;
  struct frm_frame *frame;
  uint32_t cursor;
};

struct frm_out_frame_list {
  struct frm_list list;
  int out_index;
};

/*
  Out Frames List Item
*/

/*  allocate new object */
struct frm_out_frame_list_item *frm_out_frame_list_item_new();

/*  deallocate allocated object */
void frm_out_frame_list_item_destroy (struct frm_out_frame_list_item *self);

/*  initialize the object */
void frm_out_frame_list_item_init (struct frm_out_frame_list_item *self);

/*  set frame of the out list item.
    this function increases reference count of the frame. */
void frm_out_frame_list_item_set_frame (struct frm_out_frame_list_item *self,
  struct frm_frame *frame);

/*  de-init the object and frame of the item.
    this function called by this library inside `frm_out_frame_list_written`
    function. */
void frm_out_frame_list_item_term (struct frm_out_frame_list_item *self);

/*
  Out Frames List
*/

/*  allocate new object */
struct frm_out_frame_list *frm_out_frame_list_new();

/*  deallocate allocated object */
void frm_out_frame_list_destroy (struct frm_out_frame_list *self);

/*  initialize the object */
void frm_out_frame_list_init (struct frm_out_frame_list *self);

/*  de-init the object. */
void frm_out_frame_list_term (struct frm_out_frame_list *self);

/*  add new element to the list */
void frm_out_frame_list_insert (struct frm_out_frame_list *self,
  struct frm_out_frame_list_item *elem);

/*  fill output iovecs */
ssize_t frm_out_frame_list_get_iovs (struct frm_out_frame_list *self,
    struct iovec *iovs, int iovcnt, int *retiovcnt);

/*  set written bytes */
int frm_out_frame_list_written (struct frm_out_frame_list *self,
  ssize_t written);

#endif
