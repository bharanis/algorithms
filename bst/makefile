CC=gcc
CFLAGS=-g -I. -I../common/

COMMON := ../common

EXECUTABLES := bst_array\
               bst_basic\
               bst_enhanced\
               largest_bst_subtree_in_bt\
  

all: $(EXECUTABLES) 

help:
	@echo "targets are:"
	@echo "all      - makes all"
	@echo "clean    - cleans all"
	@echo "filename - builds filename.c"

largest_bst_subtree_in_bt:   largest_bst_subtree_in_bt.o $(COMMON)/binarytree_utils.o $(COMMON)/binarytree_utils.h
	$(CC) -o $@ $^

bst_enhanced:   bst_enhanced.o $(COMMON)/binarytree_utils.o $(COMMON)/binarytree_utils.h
	$(CC) -o $@ $^

bst_basic:      bst_basic.c
bst_array:      bst_array.c
heap_array:     heap_array.c
	$(CC) $@.c -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $? -o $@ $(CFLAGS)

clean: 
	rm -fr *.o a.out* *.dSYM $(EXECUTABLES)
