CC=gcc
CFLAGS=-I.

EXECUTABLES := bst_array\
               bst_basic\
               bst_enhanced\
               heap_array

help:
	@echo "targets are:"
	@echo "all      - makes all"
	@echo "clean    - cleans all"
	@echo "filename - builds filename.c"

all: $(EXECUTABLES) 

bst_enhanced:     bst_enhanced.c
bst_basic:        bst_basic.c
bst_enhanced:     bst_enhanced.c
heap_array:       heap_array.c
	$(CC) $@.c -o $@ $(CFLAGS)

clean: 
	rm -fr *.o a.out* $(EXECUTABLES)
