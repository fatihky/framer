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

#ifndef FRAMER_H
#define FRAMER_H

#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>
#include "list.h"
#include "cbuf.h"
#include "frame-list.h"

enum frm_frame_type {
  FRAMER_FRAME_EMPTY,
  FRAMER_FRAME_ALLOCATED, /*  frame data malloc'ed */
  FRAMER_FRAME_EMBEDDED   /*  frame data points to counted buffer's data */
};

struct frm_frame {
  uint32_t size;
  char *buf;

  /*  private */
  struct frm_list_item item;
  /*  read/write cursor */
  uint32_t cursor;
  /*  frame type */
  enum frm_frame_type type;
  /*  pointing buffer if frame is embedded */
  struct frm_cbuf *cbuf;
  /*  reference count */
  int refcnt;
};

struct frm_parser {
  struct frm_list in_frames;
  struct frm_frame curr_frame;
};

typedef struct frm_frame *(*frm_frame_allocator_fn)();

void frm_parser_init (struct frm_parser *self);
void frm_parser_term (struct frm_parser *self);
int frm_parser_parse (struct frm_parser *self, struct frm_cbuf *cbuf,
                                                            ssize_t nread);
int frm_parser_parse_cust (struct frm_parser *self, struct frm_cbuf *cbuf,
                            ssize_t nread, frm_frame_allocator_fn allocator);

/*  Allocate new frame */
struct frm_frame *frm_frame_new();

/*  Initialize frame.
    This function sets frame's reference count to 1. */
void frm_frame_init (struct frm_frame *self);

/*  Allocate and assing data to *empty* frame. */
int frm_frame_set_data (struct frm_frame *self, char *data, uint32_t size);

/*  Increase reference count of the frame.
    @returns new reference count. */
int frm_frame_ref (struct frm_frame *self);

/*  Get reference count of the frame. */
int frm_frame_get_refcnt (struct frm_frame *self);

/*  Deallocate all resources owned by frame.
    This function decreases frame's reference count by 1 and does nothing
    if frame has one or more references left.
    @returns 0 if frame's data terminated, or reference count of the frame. */
int frm_frame_term (struct frm_frame *self);

/*  Deallocate frame */
void frm_frame_destroy (struct frm_frame *self);

/*  Get total length(frame header + frame data) of frame */
size_t frm_frame_totlen (struct frm_frame *self);

/*  Encode frame to the buffer */
void frm_frame_encode (char *dst, struct frm_frame *src);

#endif
