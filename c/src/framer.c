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

#include <stdlib.h>
#include "err.h"
#include "framer.h"

void frm_parser_init (struct frm_parser *self) {
  frm_list_init (&self->in_frames);
  frm_frame_init (&self->curr_frame);
}

void frm_parser_term (struct frm_parser *self) {
  frm_list_term (&self->in_frames);
  frm_frame_term (&self->curr_frame);
}

int frm_parser_parse (struct frm_parser *self, struct frm_cbuf *cbuf, ssize_t nread)
{
  char *ptr = cbuf->buf;
  size_t remaining = nread; // buf->len;
  size_t needed;
  size_t len;
  int can_embed;

  struct frm_frame *curr_frame = &self->curr_frame;

  for (;;) {
    if (frm_fast (curr_frame->cursor < 4)) {
      needed = 4 - curr_frame->cursor;
      len = remaining >= needed
            ? needed
            : remaining;

      if (frm_fast (len == 4))
        curr_frame->size = * (uint32_t *)ptr;
      else
        memcpy (&curr_frame->size + curr_frame->cursor, ptr, len);

      curr_frame->cursor += len;
      remaining -= len;
      ptr += len;
    }

    if (frm_fast (curr_frame->cursor >= 4 && remaining > 0)) {
      needed = curr_frame->size;

      if (curr_frame->cursor > 4)
        needed -= curr_frame->cursor - 4;

      len = remaining >= needed
            ? needed
            : remaining;
      can_embed = curr_frame->cursor == 4 && len == needed;

      if (frm_fast (can_embed)) {
        /*  just point to buffer */
        curr_frame->buf = ptr;
        curr_frame->cbuf = cbuf;
        curr_frame->type = FRAMER_FRAME_EMBEDDED;
        frm_cbuf_ref (cbuf);
      }
      else {
        if (frm_fast (curr_frame->buf == NULL)) {
          curr_frame->type = FRAMER_FRAME_ALLOCATED;
          curr_frame->buf = malloc (curr_frame->size);
        }

        if (frm_slow (curr_frame->buf == NULL))
          return ENOMEM;

        memcpy (curr_frame->buf + curr_frame->cursor - 4, ptr, len);
      }

      curr_frame->cursor += len;
      remaining -= len;
      ptr += len;

      if (frm_fast (curr_frame->cursor - 4 == curr_frame->size)) {
        struct frm_frame *fr = frm_frame_new();

        if (frm_slow (fr == NULL))
          return ENOMEM;

        fr->size = curr_frame->size;
        fr->buf = curr_frame->buf;
        fr->type = curr_frame->type;
        fr->cbuf = curr_frame->cbuf;

        frm_list_insert (&self->in_frames, &fr->item,
                        frm_list_end (&self->in_frames));

        frm_frame_init (&self->curr_frame);
      }
    }

    if (frm_slow (remaining == 0)) {
      frm_cbuf_unref (cbuf);
      break;
    }
  }

  return 0;
}
