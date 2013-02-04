#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"



#if 0

Logic used:
===========
1. Current node forms a BST if:
  a. left subtree is BST or NULL
  b. right subtree is BST or NULL
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


pseudo code iteration 1:
========================
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
  o.max = (r.isNULL)?i.root->key:r.max


  // set o.isBST
  // if we are leaf node mark bst as true
  // if left is bst and right is bst and current node is in order
  // if left is NULL and right is BST
  // if right is NULL and left is BST
  if (l.isNULL || (l.isBST && i.root->key > l.max)) &&
     (r.isNULL || (r.isBST && i.root->key < r.min))
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


pseudo code optimisation (reducing stack usage):
===============================================
Iteration 1 uses approx 3(l,r,o)*6(struct elems) => 18 state variables
Also return stack values => copying entire struct on each return call.

1) iteration1 flow and variable dependancy:
o.isNULL          F(root)
>> l = left-recursion
>> r = right-recursion
o.isBST           F(root, l.isBST, l.max, r.isBST, r.min, l.isNULL, r.isNULL)
o.min             F(root, l.isNULL, l.min)
o.max             F(root, r.isNULL, r.max)
o.largestbstsize  F(root, o.isBST, l.largestbstsize, r.largestbstsize)
o.largestbst      F(root, o.isBST, l.largestbstsize, r.largestbstsize, l.largestbst, r.largestbst)

2) Rearranging statements to resue same variable for left and right
step1: rearrange statements to group "l" and "r"
o.isNULL          F(root)
>> l = left-recursion
o.isBST           F1(root, l.isBST, l.max, l.isNULL)
l_largestbstsize  = l.largestbstsize
l_largestbst      = l.largestbst    
o.min             F(root, l.isNULL, l.min)
>> r = right-recursion
o.isBST           F2(root, r.isBST, r.max, r.isNULL)
o.max             F(root, r.isNULL, r.max)
o.largestbstsize  F(root, o.isBST, l_largestbstsize, r.largestbstsize)
o.largestbst      F(root, o.isBST, l_largestbstsize, r.largestbstsize, l.largestbst, r.largestbst)

step2: replace l and r with same variable "lr"
o.isNULL          F(root)
>> lr = left-recursion
o.isBST           F1(root, lr.isBST, lr.max, lr.isNULL)
l_largestbstsize  = lr.largestbstsize
l_largestbst      = lr.largestbst    
o.min             F(root, lr.isNULL, lr.min)
>> lr = right-recursion
o.isBST           F2(root, lr.isBST, lr.max, lr.isNULL)
o.max             F(root, lr.isNULL, lr.max)
o.largestbstsize  F(root, o.isBST, l_largestbstsize, lr.largestbstsize)
o.largestbst      F(root, o.isBST, l_largestbstsize, lr.largestbstsize, l.largestbst, r.largestbst)

reduced 1*6 elems and added 2 new variables (l_xxx) => reduction of 4.
Total is 14.

3) clubbing isNULL and isBST 
Values of isNULL and isBST as used in the above function.
isNULL   isBST    isNULLorBST
 1      not-set      0
 0        0         -1
 0        1          1

o.isNULLorBST     F(root)
>> lr = left-recursion
o_isBST           F1(root, lr.isNULLorBST, lr.max)
l_largestbstsize  = lr.largestbstsize
l_largestbst      = lr.largestbst    
o.min             F(root, lr.isNULLorBST, lr.min)
>> lr = right-recursion
o_isBST           F2(root, lr.isNULLorBST, lr.max)
o.max             F(root, lr.isNULLorBST, lr.max)
o.largestbstsize  F(root, o_isBST, l_largestbstsize, lr.largestbstsize)
o.largestbst      F(root, o_isBST, l_largestbstsize, lr.largestbstsize, l.largestbst, r.largestbst)
o.isNULLorBST     F(o_isBST)

reduced 3*1elem and added o_isBST => reduction of 2.
Total is 12.

4) use global variable (or root recursion stack variable) to store the largestbst instead of passing up recursion chain purely via stack variables
g_largestbst, g_largestbstsize  => accumulated across recursions

o.isNULLorBST     F(root)
>> lr = left-recursion
o_isBST           F1(root, lr.isNULLorBST, lr.max)
l_largestbstsize  = lr.largestbstsize
o.min             F(root, lr.isNULLorBST, lr.min)
>> lr = right-recursion
r_largestbstsize  = lr.largestbstsize
o_isBST           F2(root, lr.isNULLorBST, lr.max)
o.max             F(root, lr.isNULLorBST, lr.max)
g_largestbstsize  F(o_isBST, l_largestbstsize, r_largestbstsize)
g_largestbst      F(root, o_isBST, l_largestbstsize, r_largestbstsize) << largestbst from l/r would already have been accumulated here
o.isNULLorBST     F(o_isBST)


Replace largestbstsize (which is the size of any BST subtree to the tree pointed by root) with bstsize(which is the size if the entire subtree is BST)
This is ok since we accumulate the largestbst in the g_largestbst
Now l/r.bstsize is the size of the immediate child subtree.

o.isNULLorBST     F(root)
>> lr = left-recursion
o_isBST           F1(root, lr.isNULLorBST, lr.max)
l_bstsize   = lr.bstsize
o.min             F(root, lr.isNULLorBST, lr.min)
>> lr = right-recursion
r_bstsize   = lr.bstsize
o_isBST           F2(root, lr.isNULLorBST, lr.max)
o.max             F(root, lr.isNULLorBST, lr.max)
o.bstsize   =     F(o_isBST, l_bstsize , r_bstsize )
g_largestbstsize  F(o_isBST, o.bstsize)
g_largestbst      F(root, o_isBST, o.bstsize) << largestbst from l/r would already have been accumulated here
o.isNULLorBST     F(o_isBST)


overload bstsize with isBSTorNULL info in the following way:
isBSTorNULL     bstsize
  NULL(0)         0
  BST(1)          >=1 => actual size of bst subtree
 !BST(-1)         -1

o.bstsize(0)     F(root)
>> lr = left-recursion
o_isBST           F1(root, lr.bstsize, lr.max)
l_bstsize   = lr.bstsize
o.min             F(root, lr.bstsize, lr.min)
>> lr = right-recursion
r_bstsize   = lr.bstsize
o_isBST           F2(root, lr.bstsize, lr.max)
o.max             F(root, lr.bstsize, lr.max)
o.bstsize   =     F(o_isBST, l_bstsize , r_bstsize)
g_largestbstsize  F(o_isBST, o.bstsize)
g_largestbst      F(root, o_isBST, o.bstsize) << largestbst from l/r would already have been accumulated here




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
  o.max = (r.isNULL)?i.root->key:r.max


  // set o.isBST 
  // if we are leaf node mark bst as true
  // if left is bst and right is bst and current node is in order
  // if left is NULL and right is BST 
  // if right is NULL and left is BST 
  if (l.isNULL || (l.isBST && i.root->key > l.max)) &&
     (r.isNULL || (r.isBST && i.root->key < r.min))
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
// Direct implementation of pseudocode
// needs large stack space per iteration 
// not all return variables are set at the time
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
  o.max = (r.isNULL)?root->key:r.max;


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


#if 0
//code iteration 2:
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
  o.max = (r.isNULL)?root->key:r.max;


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

#endif



/* 
 * Largest subtree of a given Binary Tree that 
 * includes all the descendants and is a Binary Search Tree
 * Bottom up approach - postorder
 * This is O(n)
 * solutionn form: http://leetcode.com/2010/11/largest-binary-search-tree-bst-in.html 
 */

int _largest_bst_in_bt_alldescendants_bottomup(bnode *root, int *min, int *max, int *largestsize, bnode **largestbst)
{
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

      random_swap(btroot, 10);

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
