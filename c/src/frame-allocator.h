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

#ifndef FRM_FRAME_ALLOCATOR_H
#define FRM_FRAME_ALLOCATOR_H

struct frm_frame;

typedef struct frm_frame *(*frm_frame_allocator_fn)(void *data);
typedef struct frm_frame *(*frm_frame_deallocator_fn)(void *data,
  struct frm_frame *frame);

struct frm_frame_allocator {
  /*  frame allocator function */
  frm_frame_allocator_fn alloc_fn;
  /*  deallocator function */
  frm_frame_deallocator_fn destroy_fn;
  /*  user data */
  void *data;
};

void frm_frame_allocator_init (struct frm_frame_allocator *self,
    frm_frame_allocator_fn alloc_fn, frm_frame_deallocator_fn destroy_fn);
void frm_frame_allocator_term (struct frm_frame_allocator *self);

#endif
