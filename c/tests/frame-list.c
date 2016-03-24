#include <assert.h>
#include "framer.h"

int main(int argc, char *argv[])
{
  struct iovec iovs[10];
  struct frm_out_frame_list *fl = frm_out_frame_list_new();
  struct frm_frame *frm1 = frm_frame_new();
  struct frm_frame *frm2 = frm_frame_new();
  struct frm_frame *frm3 = frm_frame_new();

  frm_frame_set_data (frm1, "foo", 3);
  frm_frame_set_data (frm2, "bar", 3);
  frm_frame_set_data (frm3, "test", 4);

  struct frm_out_frame_list_item *flitem1 = frm_out_frame_list_item_new();
  struct frm_out_frame_list_item *flitem2 = frm_out_frame_list_item_new();
  struct frm_out_frame_list_item *flitem3 = frm_out_frame_list_item_new();

  frm_out_frame_list_item_set_frame (flitem1, frm1);
  frm_out_frame_list_item_set_frame (flitem2, frm2);
  frm_out_frame_list_item_set_frame (flitem3, frm3);

  frm_out_frame_list_insert (fl, flitem1);
  frm_out_frame_list_insert (fl, flitem2);
  frm_out_frame_list_insert (fl, flitem3);

  ssize_t towrite;
  int retiovcnt = 0;

  towrite = frm_out_frame_list_get_iovs (fl, iovs, 10, &retiovcnt);

  assert (towrite == 22);
  assert (retiovcnt == 6);

  frm_out_frame_list_written (fl, 22);

  frm_out_frame_list_item_destroy (flitem1);
  frm_out_frame_list_item_destroy (flitem2);
  frm_out_frame_list_item_destroy (flitem3);

  frm_frame_destroy (frm1);
  frm_frame_destroy (frm2);
  frm_frame_destroy (frm3);

  frm_out_frame_list_destroy (fl);

  return 0;
}
