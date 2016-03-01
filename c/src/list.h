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

#ifndef FRAMER_LIST_INCLUDED
#define FRAMER_LIST_INCLUDED

struct frm_list_item {
  struct frm_list_item *next;
  struct frm_list_item *prev;
};

struct frm_list {
  struct frm_list_item *first;
  struct frm_list_item *last;
};

/*  Undefined value for initializing a list item which is not part of a list. */
#define FRAMER_LIST_NOTINLIST ((struct frm_list_item*) -1)

/*  Use for initializing a list item statically. */
#define FRAMER_LIST_ITEM_INITIALIZER {FRAMER_LIST_NOTINLIST, FRAMER_LIST_NOTINLIST}

/*  Initialise the list. */
void frm_list_init (struct frm_list *self);

/*  Terminates the list. Note that all items must be removed before the
    termination. */
void frm_list_term (struct frm_list *self);

/*  Returns 1 is list has zero items, 0 otherwise. */
int frm_list_empty (struct frm_list *self);

/*  Returns iterator to the first item in the list. */
struct frm_list_item *frm_list_begin (struct frm_list *self);

/*  Returns iterator to one past the last item in the list. */
struct frm_list_item *frm_list_end (struct frm_list *self);

/*  Returns iterator to an item prior to the one pointed to by 'it'. */
struct frm_list_item *frm_list_prev (struct frm_list *self,
                                   struct frm_list_item *it);

/*  Returns iterator to one past the item pointed to by 'it'. */
struct frm_list_item *frm_list_next (struct frm_list *self,
                                   struct frm_list_item *it);

/*  Adds the item to the list before the item pointed to by 'it'. Priot to
    insertion item should not be part of any list. */
void frm_list_insert (struct frm_list *self, struct frm_list_item *item,
                     struct frm_list_item *it);

/*  Removes the item from the list and returns pointer to the next item in the
    list. Item must be part of the list. */
struct frm_list_item *frm_list_erase (struct frm_list *self,
                                    struct frm_list_item *item);

/*  Initialize a list item. At this point it is not part of any list. */
void frm_list_item_init (struct frm_list_item *self);

/*  Terminates a list item. Item must not be part of any list before it's
    terminated. */
void frm_list_item_term (struct frm_list_item *self);

/*  Returns 1 is the item is part of a list, 0 otherwise. */
int frm_list_item_isinlist (struct frm_list_item *self);

#endif
