#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"


/*************************************
              main 
 ************************************/

int input[100];
main ()
{
   int input_size, i, trial;

   srandom ((unsigned)&i);
   bnode *bstroot = NULL;

   for (trial=0; trial < 3; trial ++) {
      input_size = populate_array (input, sizeof(input)/sizeof(input[0]));

      print_array ("\nunsorted:  ", input, input_size);
      printf ("\n");

      /* convert array to bst */
      for (i=0; i<input_size; i++)
         bst_insert (&bstroot, input[i]);


      printf ("binary search tree:\n");
      print_tree(bstroot);

      printf ("paths:\n");
      print_paths(bstroot);

      if (2 != random_swap(bstroot, 2)) {
         printf ("BST distrubed\n");
         print_tree(bstroot);
      }
      printf ("isbst returned %s\n", isbst(bstroot)?"true":"false");

      mirror(bstroot);
      printf ("mirror:\n");
      print_tree(bstroot);

#if 0
      for (i=0; i<input_size; i++) {
        delete (&bstroot, input[i]); 
        printf ("\n\ndeleted: %d\n", input[i]);
        print_tree(bstroot);
      }
#endif

      /* delete tree */
      delete_tree (bstroot);
      bstroot = NULL;
   }
}
