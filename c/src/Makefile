SOURCES=cbuf.c err.c frame.c framer.c list.c
OBJS=$(patsubst %.c,%.o,$(SOURCES))

.PHONY: all

all: libframer.a

libframer.a: $(OBJS)
	ar -rcs libframer.a $(OBJS)

$(OBJS): framer.h

.PHONY: benchmark

benchmark: ../bench/parser_bench

../bench/parser_bench: ../bench/parser_bench.c libframer.a
	cc ../bench/parser_bench.c -o ../bench/parser_bench libframer.a -I. --std=gnu99

.PHONY: clean

clean:
	rm -rf *.o libframer.a
	rm -rf ../bench/parser_bench
