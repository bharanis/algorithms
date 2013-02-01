CC=gcc
CFLAGS=-g -I. -I../common/

COMMON := ../common

EXECUTABLES = quicksort

all: $(EXECUTABLES) 

help:
	@echo "targets are:"
	@echo "all      - makes all"
	@echo "clean    - cleans all"
	@echo "filename - builds filename.c"

quicksort:     quicksort.c
	$(CC) $@.c -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $? -o $@ $(CFLAGS)

clean: 
	rm -fr *.o a.out* *.dSYM $(EXECUTABLES)
