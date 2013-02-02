#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"



/* this is a top down approach.
 * the resultant tree should have all its descendants in BST order
 */
bnode *largest_bst_in_bt_alldescendants_topdown(bnode *root)
{
   int countleft  = 0;
   int countright = 0;
   bnode *l, *r;

   if (root == NULL) return NULL;
   
   if (isbst(root)) {
      return root;
   }


   l = largest_bst_in_bt_alldescendants_topdown(root->left);
   r = largest_bst_in_bt_alldescendants_topdown(root->right);
   
   if ((l != NULL) && (r != NULL)) {
     countright = count(r);
     countleft  = count(l);
     return (countright > countleft)?r:l;
   }
   else if (l != NULL)
     return l;
   else if (r != NULL)
     return r;

   return NULL;
}

/*************************************
              main 
 ************************************/

#define maxsize  100
main ()
{
   int input_size, i, trial;
   bnode *btroot  = NULL;
   bnode *bstroot = NULL;

   srandom(&i);

   for (trial=0; trial < 3; trial ++) {

      input_size = (random() % (maxsize-maxsize/10)) + maxsize/10;
      btroot = create_random_bst(input_size);

      printf ("binary tree:\n");
      print_tree(btroot);

      random_swap(btroot, 2);

      printf ("binary search tree:\n");
      print_tree(largest_bst_in_bt_alldescendants_topdown(btroot));

      /* delete tree */
      delete_tree (btroot);
      btroot = NULL;
   }
}
