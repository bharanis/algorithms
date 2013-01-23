#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int i, int j)
{
  int t = arr[i];
  arr[i] = arr[j];
  arr[j] = t;
}

int partition (int *arr, int left, int right, int pivotidx)
{
   int pivot = arr[pivotidx];
   int storeidx = left;
   int i;

   swap (arr, pivotidx, right);
   for (i = left; i <= right-1; i++) {
      if (arr[i] < pivot) {
        swap(arr, i, storeidx);
        storeidx ++;
      }
   }

   swap (arr, right, storeidx);
   return storeidx;
}

myqsort (int *arr, int left, int right)
{
   int pivotidx = (left + right) / 2;

   pivotidx = partition (arr, left, right, pivotidx);

   if (left < pivotidx)
      myqsort (arr, left, pivotidx - 1);
   if (pivotidx < right)
      myqsort (arr, pivotidx + 1, right);
}


void print_array(char *header, int *a, int s)
{
  int i;

  printf("%s\n", header);
  for (i = 0; i < s; i++) 
     printf("%d ", a[i]);
  printf("\n");
}


#define INPUT_SIZE  50
int input[INPUT_SIZE];

main ()
{
   int input_size, i, trial;

   for (trial=0; trial < 5; trial ++) {
      /* generate and input size from 10 .. 100 */
      input_size = (random() % (INPUT_SIZE-10)) + 10;

      for (i = 0; i < input_size; i ++) {
         input[i] = random() % (INPUT_SIZE*10);
      }

      print_array ("unsorted: ", input, input_size);
      myqsort(input, 0, input_size-1); 
      print_array ("sorted: ", input, input_size);
   }
}
