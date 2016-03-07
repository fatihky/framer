/*
  Copyright (c) 2016 Fatih Kaya  All rights reserved.

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

/*  Initialize frame */
void frm_frame_init (struct frm_frame *self);

/*  Allocate and assing data to *empty* frame */
int frm_frame_set_data (struct frm_frame *self, char *data, uint32_t size);

/*  Deallocate all resources owned by frame */
void frm_frame_term (struct frm_frame *self);

/*  Deallocate frame */
void frm_frame_destroy (struct frm_frame *self);

/*  Get total length(frame header + frame data) of frame */
size_t frm_frame_totlen (struct frm_frame *self);

/*  Encode frame to the buffer */
void frm_frame_encode (char *dst, struct frm_frame *src);

#endif
