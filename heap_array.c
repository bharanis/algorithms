#include <stdio.h>
#include <stdlib.h>

/* we are using an array to represent a heap.
 * The heap begins at index 1. i.e, root of the heap is a[1]
 * a[0] is used to store the lastnode (aka heap size)
 */


#define HEAP_INVALID        -1
#define MAX_HEAP_DEPTH      10
#define MAX_HEAP_IDX        (1<<MAX_HEAP_DEPTH)
int heaparray[1<<MAX_HEAP_DEPTH];
#define max_heap_idx_check(x,rv)  if(x > (1<<MAX_HEAP_DEPTH)) return rv;

#define max_val(x,y) (((x)>(y))?(x):(y))

#define lastnode(x)         ((x)[0])

#define leftval(a,x)        a[((x)*2)]
#define rightval(a,x)       a[((x)*2 + 1)]
#define parentval(a,x)      a[((int)(x)/2)]
#define left(x)             ((x)*2)
#define right(x)            ((x)*2 + 1)
#define parent(x)           ((int)(x)/2)


min_insert (int *heap, int key)
{
   int pos = lastnode(heap);
   int temp;

   if (pos >= MAX_HEAP_IDX) return;

   heap[lastnode(heap)] = key;
   lastnode(heap) ++;
   
   while ((pos > 1) && (parentval(heap, pos) > heap[pos])) {
     temp = heap[pos];
     heap[pos] = parentval(heap, pos);
     parentval(heap, pos) = temp;
     pos = parent(pos);
   }

   return 0;
}

max_insert (int *heap, int key)
{
   int pos = lastnode(heap);
   int temp;

   if (pos >= MAX_HEAP_IDX) return;

   heap[lastnode(heap)] = key;
   lastnode(heap) ++;
   
   while ((pos > 1) && (parentval(heap, pos) < heap[pos])) {
     temp = heap[pos];
     heap[pos] = parentval(heap, pos);
     parentval(heap, pos) = temp;
     pos = parent(pos);
   }

   return 0;
}

int min_delete (int *heap)
{
   int pos = 1;
   int selected;
   int retval = heap[1];
   int temp;

   lastnode(heap) --;
   heap[1] = heap[lastnode(heap)];
   
   while (left(pos) < lastnode(heap)) {
     selected = pos;
 
     if (leftval(heap, pos) < heap[selected]) {
        selected = left(pos);
     }
     if (rightval(heap, pos) < heap[selected]) {
        selected = right(pos);
     }
     if (selected != pos) {
        temp = heap[pos];
        heap[pos] = heap[selected];
        heap[selected] = temp;
        pos = selected;
     }
     else 
        break;
   }

   return retval;
}


int max_delete (int *heap)
{
   int pos = 1;
   int selected;
   int retval = heap[1];
   int temp;

   lastnode(heap) --;
   heap[1] = heap[lastnode(heap)];
   
   while (left(pos) < lastnode(heap)) {
     selected = pos;
 
     if (leftval(heap, pos) > heap[selected]) {
        selected = left(pos);
     }
     if (rightval(heap, pos) > heap[selected]) {
        selected = right(pos);
     }
     if (selected != pos) {
        temp = heap[pos];
        heap[pos] = heap[selected];
        heap[selected] = temp;
        pos = selected;
     }
     else 
        break;
   }

   return retval;
}


/*************************************
            array helpers 
 ************************************/

int populate_array (int *arr, int size)
{
   int i;
   /* randomize size between size/10 .. size */
   size = (random() % (size-size/10)) + size/10;

   /* populate with data range of 0 .. size*10 */
   for (i = 0; i < size; i ++) {
      arr[i] = random() % (size*10);
   }

   return size;
}

void print_array(char *header, int *a, int s)
{
  int i;

  printf("%s", header);
  for (i = 0; i < s; i++) 
     printf("%d ", a[i]);
}


/*************************************
              main 
 ************************************/

int input[50];

main ()
{
   int input_size, i, trial;

   srandom ((unsigned)&i);

   for (trial=0; trial < 3; trial ++) {
      memset (heaparray, HEAP_INVALID, sizeof(heaparray));
      input_size = populate_array (input, sizeof(input)/sizeof(input[0]));
      input[0] = 0;

      print_array ("\nunsorted:  ", input, input_size);

      heaparray[0] = 1;
      for (i=1; i<input_size; i++)
         max_insert (heaparray, input[i]);

      print_array ("\nmax-heap:  ", heaparray, input_size);

      printf ("\nmax-del:   %d ", heaparray[0]); 
      for (i=1; i<input_size; i++) 
        printf ("%d ", max_delete (heaparray)); 

      heaparray[0] = 1;
      for (i=1; i<input_size; i++)
         min_insert (heaparray, input[i]);

      print_array ("\nmin-heap:  ", heaparray, input_size);

      printf ("\nmin-del:   %d ", heaparray[0]); 
      for (i=1; i<input_size; i++) 
        printf ("%d ", min_delete (heaparray)); 
 
      
      printf ("\n");
   }
}
