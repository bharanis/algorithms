CC=gcc
CFLAGS=-g -I.


all:   $(patsubst %.c,%.o,$(wildcard *.c))

help:
	@echo "targets are:"
	@echo "all      - makes all"
	@echo "clean    - cleans all"


%.o: %.c
	$(CC) -c $? -o $@ $(CFLAGS)

clean: 
	rm -fr *.o a.out* *.dSYM $(EXECUTABLES)
