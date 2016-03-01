#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "framer.h"

#define COUNT 50000000

/*  This function from redis. Please see https://github.com/antirez/redis for
    license information. */
static long long mstime()
{
  struct timeval tv;
  long long mst;

  gettimeofday(&tv, NULL);
  mst = ((long long)tv.tv_sec)*1000;
  mst += tv.tv_usec/1000;
  return mst;
}

int main(int argc, char *argv[])
{
  size_t sz;
  struct frm_frame fr;
  struct frm_cbuf *cbuf;
  struct frm_parser parser;
  frm_frame_init (&fr);
  frm_frame_set_data (&fr, "test", 4);
  sz = frm_frame_totlen (&fr);

  // encode
  cbuf = frm_cbuf_new (sz * COUNT);
  char *buf = cbuf->buf;

  for(int i = 0; i < COUNT; i++)
    frm_frame_encode (buf + (i * sz), &fr);

  // parse
  frm_parser_init (&parser);

  long long ms = mstime();

  frm_parser_parse (&parser, cbuf, COUNT * sz);

  ms = mstime() - ms;

  printf ("Thoughput: %lld frames/sec\n", (COUNT * 1000LL)/ ms);

  frm_frame_term (&fr);

  return 0;
}
