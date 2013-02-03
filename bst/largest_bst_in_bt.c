#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"



#if 0

Logic used:
===========
1. Current node forms a BST if:
  a. left subtree is BST
  b. right subtree is BST
  c. root->key > (left subtree).max value
  d. root->key < (right subtree).min value
2. Leaf node is treated as a BST of size 1
   Leaf node is identified by 2 NULL subtrees.
3. If current node is a BST, pass it up
   If not pass up the largest BST between left and right subtree

Data required from each subtree (recursion level):
 1. isBST           << is the subtree a BST
 2. max             << max value of the subtree
 3. min             << min value of the subtree
 4. largestbstsize  << size of largest BST contained in the subtree
 5. largestbst      << pointer to largest BST contained in the subtree
 6. isNULL          << is the subtree a NULL subtree
Data passed to each subtree (recursion level):
 1. root   << pointer to subtree


pseudo code:
============
i. => input params
o. => o/p return value < passed up to caller
r. => right
l. => left

(o.isBST, o.min, o.max, o.largestbstsize, o.largestbst, o.isNULL) returned from LBST() taking (i.root)
{
  if NULL == i.root {
    o.isNULL = true
    o.largestbstsize = 0
    return;
  } else 
    o.isNULL = false
  

  l.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->left)
  r.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->right)

  // Extract Min and Max values
  // if left subtree exists, inherit smallest node, else current node is smallest
  o.min = (l.isNULL)?i.root->key:l.min
  // if right subtree exists, inherit largest node, else current node is largest
  o.max = (l.isNULL)?i.root->key:r.max


  // set o.isBST 
  // if we are leaf node mark bst as true
  if (l.isNULL && r.isNULL)
     o.isBST = true
  // if left is bst and right is bst and current node is in order
  else if(l.isBST && r.isBST && i.root->key > l.max && i.root->key < r.min)
    o.isBST = true;
  else 
    o.isBST = false

  if (o.isBST) {
     // we are the largest subtree @ this level
     o.largestbstsize = l.largestbstsize + r.largestbstsize + 1;
     o.largestbst = i.root;
  } else {
     // pass up the largest subtree so far
     o.largestbstsize = max(l.largestbstsize, r.largestbstsize)
     o.largestbst     = (l.largestbstsize > r.largestbstsize)?l.largestbst:r.largestbst;
  }
}

#endif

//code iteration 1:
//=================
struct lbst_op {
  int    isBST;
  int    isNULL;
  int    min;
  int    max;
  int    largestbstsize;
  bnode *largestbst;
};

#define true  1
#define false 0

struct lbst_op LBST(bnode *root)
{
  struct lbst_op o, l, r;

  if (NULL == root) {
    o.isNULL = true;
    o.largestbstsize = 0;
    return o;
  }
  else {
   o.isNULL = false;
  }

  l = LBST(root->left);
  r = LBST(root->right);

  // Extract Min and Max values
  // if left subtree exists, inherit smallest node, else current node is smallest
  o.min = (l.isNULL)?root->key:l.min;
  // if right subtree exists, inherit largest node, else current node is largest
  o.max = (l.isNULL)?root->key:r.max;


  // set o.isBST 
  // if we are leaf node mark bst as true
  // if left is bst and right is bst and current node is in order
  if((l.isNULL || (l.isBST && (root->key > l.max))) && 
          (r.isNULL || (r.isBST && (root->key < r.min))))
    o.isBST = true;
  else 
    o.isBST = false;

  if (o.isBST) {
     // we are the largest subtree @ this level
     o.largestbstsize = l.largestbstsize + r.largestbstsize + 1;
     o.largestbst = root;
  } else {
     // pass up the largest subtree so far
     o.largestbstsize = (l.largestbstsize > r.largestbstsize)?l.largestbstsize:r.largestbstsize;
     o.largestbst     = (l.largestbstsize > r.largestbstsize)?l.largestbst:r.largestbst;
  }

  return o;
}




/* 
 * Largest subtree of a given Binary Tree that 
 * includes all the descendants and is a Binary Search Tree
 * Bottom up approach - postorder
 * This is O(n)
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
   struct lbst_op o;

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
      printf ("binary search tree method 3:\n");
      o = LBST(btroot);
      print_tree(o.largestbst);

      /* delete tree */
      delete_tree (btroot);
      btroot = NULL;
   }
}
