#include <stddef.h>

#include "frame-allocator.h"

void frm_frame_allocator_init (struct frm_frame_allocator *self,
    frm_frame_allocator_fn alloc_fn, frm_frame_deallocator_fn destroy_fn) {

  self->alloc_fn = alloc_fn;
  self->destroy_fn = destroy_fn;
}

void frm_frame_allocator_term (struct frm_frame_allocator *self) {
  self->alloc_fn = NULL;
  self->destroy_fn = NULL;
}
