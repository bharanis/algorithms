#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"

/* 
 * Largest subtree of a given Binary Tree that 
 * includes all the descendants and is a Binary Search Tree
 * Bottom up approach - postorder
 * The resultant tree should have all its descendants in BST order
 * Guess this is O(n)
 * solutionn form: http://leetcode.com/2010/11/largest-binary-search-tree-bst-in.html 
 */

int _largest_bst_in_bt_alldescendants_bottomup(bnode *root, int *min, int *max, int *largestsize, bnode **largestbst)
{
   bnode *lbst, *rbst, *bst;
   int curmin, curmax, lsize, rsize, size;
   int isBST = 1;
   
   if (root == NULL) return 0;

   lsize = _largest_bst_in_bt_alldescendants_bottomup(root->left,  min, max, largestsize, largestbst);
   curmin = (lsize == 0)?root->key:*min;
   if((lsize == -1) || ((lsize != 0) && (root->key <= *max)))
      isBST = 0;

   rsize = _largest_bst_in_bt_alldescendants_bottomup(root->right, min, max, largestsize, largestbst);
   curmax = (rsize == 0)?root->key:*max;
   if((rsize == -1) || ((rsize != 0) && (root->key >= *min)))
      isBST = 0;

   if (isBST) {
     size = lsize + rsize + 1;
     bst  = root;
     if (size > *largestsize) {
        *largestsize = size;
        *largestbst  = root;
     }
     *min = curmin;
     *max = curmax;
     return size;
   }

   return -1;
}

bnode *largest_bst_in_bt_alldescendants_bottomup(bnode *root)
{
   int min, max, size = INT_MIN;
   bnode *bst = NULL;
   _largest_bst_in_bt_alldescendants_bottomup(root, &min, &max, &size, &bst);
   return bst;
}


/* 
 * Largest subtree of a given Binary Tree that 
 * includes all the descendants and is a Binary Search Tree
 * Top down approach - preorder
 * The resultant tree should have all its descendants in BST order
 * Guess this is O(nlog(n))
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
      btroot = create_random_bt(input_size);

      printf ("binary tree:\n");
      print_tree(btroot);

      random_swap(btroot, 2);

      printf ("binary search tree method 1:\n");
      print_tree(largest_bst_in_bt_alldescendants_topdown(btroot));
      printf ("binary search tree method 2:\n");
      print_tree(largest_bst_in_bt_alldescendants_bottomup(btroot));

      /* delete tree */
      delete_tree (btroot);
      btroot = NULL;
   }
}
