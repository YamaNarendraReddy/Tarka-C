CC=gcc
CFLAGS=-g -Wall

all: chibicc/chibicc tarka_runtime.o

chibicc/chibicc:
	$(MAKE) -C chibicc

tarka_runtime.o: tarka_runtime.c
	$(CC) $(CFLAGS) -c tarka_runtime.c -o tarka_runtime.o

clean:
	$(MAKE) -C chibicc clean
	rm -f *.o *_bin test_tarka a.out

.PHONY: all clean
