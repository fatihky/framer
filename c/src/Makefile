SOURCES=cbuf.c err.c frame.c frame-allocator.c frame-list.c framer.c list.c
OBJS=$(patsubst %.c,%.o,$(SOURCES))
CFLAGS=-std=gnu99 -g

.PHONY: all

all: libframer.a

libframer.a: $(OBJS)
	ar -rcs libframer.a $(OBJS)

$(OBJS): framer.h

.PHONY: benchmark

benchmark: ../bench/parser_bench

../bench/parser_bench: ../bench/parser_bench.c libframer.a
	cc ../bench/parser_bench.c -o ../bench/parser_bench libframer.a -I. --std=gnu99

.PHONY: install

install: libframer.a
	rm -rf /usr/include/framer
	mkdir -p /usr/include/framer
	cp *.h /usr/include/framer
	cp libframer.a /usr/lib

.PHONY: clean

clean:
	rm -rf *.o libframer.a
	rm -rf ../bench/parser_bench
