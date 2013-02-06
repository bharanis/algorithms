#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"



#if 0
Largest BST in a BT.
The BST need not be a complete subtree with all descendants.
 
1. assuming root points to a bst, do a preorder search (similar to isbst())
2. when you hit a node not satisfying BST condition, start a new preorder search with that node as the root.
3. accumulate largest BST seen so far
4. the resultant BST may not be the complete subtree pointed by largestbst, hence at the end either copy OR 
   define inorder/preorder bst traversal
#endif



int _LBST(bnode *root, bnode **largestbst, int *largestbstsize, int min, int max)
{
    int size;

    if (root == NULL) return 0;

    if ((root->key > min) && (root->key < max)) {
       size = 1 + _LBST(root->left, largestbst, largestbstsize, min, root->key) + _LBST(root->right, largestbst, largestbstsize, root->key, max);
       if (size > *largestbstsize) {
          *largestbstsize = size;
          *largestbst     = root;
       }
    } else {
       _LBST(root->left, largestbst, largestbstsize, INT_MIN, INT_MAX);
       _LBST(root->right, largestbst, largestbstsize, INT_MIN, INT_MAX);
    }
}

bnode *LBST(bnode *root)
{
   bnode *largestbst;
   int largestbstsize;

   _LBST (root, &largestbst, &largestbstsize, INT_MIN, INT_MAX);

   return largestbst;
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
      btroot = create_random_bt(input_size);

      printf ("binary tree:\n");
      print_tree(btroot);

      random_swap(btroot, 10);

      printf ("binary search tree method 1:\n");
      print_tree(btroot);


      /* delete tree */
      delete_tree (btroot);
      btroot = NULL;
   }
}
