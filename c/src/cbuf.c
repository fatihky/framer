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
#include "fast.h"
#include "err.h"
#include "cbuf.h"

struct frm_cbuf *frm_cbuf_new (size_t size)
{
  struct frm_cbuf *cbuf = calloc (1, sizeof (struct frm_cbuf));

  if (frm_slow (cbuf == NULL))
    return NULL;

  if (size == 0)
    goto after_malloc;

  cbuf->buf = (char *)malloc (size);

  if (frm_slow (cbuf->buf == NULL)) {
    free (cbuf);

    return NULL;
  }

after_malloc:

  cbuf->size = size;

  frm_cbuf_ref (cbuf);

  return cbuf;
}

void frm_cbuf_ref (struct frm_cbuf *self)
{
  frm_assert (self->ref >= 0);

  self->ref++;
}

void frm_cbuf_unref (struct frm_cbuf *self)
{
  frm_assert (self->ref >= 1);

  self->ref--;

  if (frm_slow (self->ref == 0)) {
    if (frm_fast (self->size > 0))
      free (self->buf);

    free (self);
  }
}
