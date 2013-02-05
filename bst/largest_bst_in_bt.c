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
Iteration 1 uses approx 3(l,r,o)*6(struct elems) => 18 state variables per stack level.
Also return stack values => copying entire struct on each return call.

1) iteration1 flow
  if root==NULL {
     o.isNULL=true;
     o.largestbstsize = 0;
     return;
  }

  l.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->left)
  r.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->right)

  o.min = (l.isNULL)?i.root->key:l.min
  o.max = (r.isNULL)?i.root->key:r.max

  o.isBST = ((l.isNULL || (l.isBST && i.root->key > l.max)) && (r.isNULL || (r.isBST && i.root->key < r.min))) ? true:false;

  if (o.isBST) {
     // we are the largest subtree @ this level
     o.largestbstsize = l.largestbstsize + r.largestbstsize + 1;
     o.largestbst = i.root;
  } else {
     // pass up the largest subtree so far
     o.largestbstsize = max(l.largestbstsize, r.largestbstsize)
     o.largestbst     = (l.largestbstsize > r.largestbstsize)?l.largestbst:r.largestbst;
  }

2) Rearranging statements to resue same variable for left and right
step1: rearrange statements to group "l" and "r"
       note 
         o.isBST = (x && y)?:true:false;
       is equivalent to
         o.isBST = !(x && y)?:false:true;
       is equivalent to
         o.isBST = (!x || !y)?:false:true;
       is equivalent to
         o.isBST = true;
         if !x o.isBST = false;
         if !y o.isBST = false;

  o.isBST = true;
  if root==NULL {
     o.isNULL=true;
     o.largestbstsize = 0;
     return;
  }

  l.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->left)
  if !(l.isNULL || (l.isBST && i.root->key > l.max))
     o.isBST = false
  o.min = (l.isNULL)?i.root->key:l.min

  r.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->right)
  if !(r.isNULL || (r.isBST && i.root->key < r.min))
     o.isBST = false
  o.max = (r.isNULL)?i.root->key:r.max

  if (o.isBST) {
     // we are the largest subtree @ this level
     o.largestbstsize = l.largestbstsize + r.largestbstsize + 1;
     o.largestbst = i.root;
  } else {
     // pass up the largest subtree so far
     o.largestbstsize = max(l.largestbstsize, r.largestbstsize)
     o.largestbst     = (l.largestbstsize > r.largestbstsize)?l.largestbst:r.largestbst;
  }


step2: replace l and r with same variable "lr"

  if root==NULL {
     o.isNULL=true;
     o.largestbstsize = 0;
     return;
  }
  o.isBST = true;

  lr.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->left)
  if !(lr.isNULL || (lr.isBST && i.root->key > lr.max))
     o.isBST = false
  o.min = (l.isNULL)?i.root->key:l.min

  lr.(isBST, isNULL, max, min, largestbst, largestbstsize) <= LBST(i.root->right)
  if !(lr.isNULL || (lr.isBST && i.root->key < lr.min))
     o.isBST = false
  o.max = (r.isNULL)?i.root->key:lr.max

  if (o.isBST) {
     // we are the largest subtree @ this level
     o.largestbstsize = l.largestbstsize + r.largestbstsize + 1;
     o.largestbst = i.root;
  } else {
     // pass up the largest subtree so far
     o.largestbstsize = max(l.largestbstsize, r.largestbstsize)
     o.largestbst     = (l.largestbstsize > r.largestbstsize)?l.largestbst:r.largestbst;
  }


reduced 1*6 elems and added 2 new variables (l_xxx) => reduction of 4.
Total is 14.

3)use global variable (or top of recursion stack variable) to store the largestbst 
  instead of passing up recursion chain purely via stack variables.
  g_largestbst, g_largestbstsize  => accumulated across recursions
  so we dont need to store largestbst.
  and we can replace largestbstsize with just size (of the current tree if BST)

  if root==NULL {
     o.isNULL=true;
     o.size = 0;
     return;
  }
  o.isBST = true;

  lr.(isBST, isNULL, max, min, size) <= LBST(i.root->left)
  lsize = lr.size;
  if !(lr.isNULL || (lr.isBST && i.root->key > lr.max))
     o.isBST = false
  o.min = (l.isNULL)?i.root->key:l.min

  lr.(isBST, isNULL, max, min, size) <= LBST(i.root->right)
  rsize = lr.size;
  if !(lr.isNULL || (lr.isBST && i.root->key < lr.min))
     o.isBST = false
  o.max = (r.isNULL)?i.root->key:lr.max

  if (o.isBST) {
     // we are the largest subtree @ this level
     o.size = lsize + rsize + 1;
     if (g_largestbstsize < o.size) {
        g_largestbstsize = o.size;  
        g_largestbst = root;
     }
  }


4) overload size with isBST and isNULL info in the following way:
   size == 0  => isNULL && isBST
   size == -1 => !isBST
   size >= 1  => isBST
  we dont have to return isBST.
  Please Note
   !(lr.isNULL || (lr.isBST && i.root->key < lr.min))
  is equivalent to
   (!lr.isNULL && (!lr.isBST || (i.root->key > lr.min)))

  if root==NULL {
     o.size = 0;
     return;
  }
  isBST = true;

  lr.(max, min, size) <= LBST(i.root->left)
  lsize = lr.size;
  if !((lr.size == 0) || ((lr.size>=0) && i.root->key > lr.max))
     isBST = false
  o.min = (lr.size == 0)?i.root->key:l.min

  lr.(max, min, size) <= LBST(i.root->right)
  rsize = lr.size;
  if !((lr.size == 0) || ((lr.size>=0) && i.root->key < lr.min))
     isBST = false
  o.max = (lr.size == 0)?i.root->key:lr.max

  if (isBST) {
     // we are the largest subtree @ this level
     o.size = lsize + rsize + 1;
     if (g_largestbstsize < o.size) {
        g_largestbstsize = o.size;  
        g_largestbst = root;
     }
  }
  else 
    o.size = -1;

5) If @ this level we are not a BST, the entire recursion stack above us will 
   not be a BST (think subtree is !BST => tree is not BST)
   Unless at some level of unwinding, we start building recursion again (walking down another subtree)
   We cache the min from left and max from right into local variables before overwriting the global min, max
   with a recursion call.
   Since we use global variables (or top of recursion stack variable), remove max/min from return context
   This way we replace min/max from each side with min/max @ just our level.
   Note
    !((lsize == 0) || ((lsize>=0) && i.root->key > g_max))
   is equivalent to
    (!(lsize == 0) && (!(lsize>=0) || (i.root->key <= g_max)))
   is equivalent to
    ((lsize != 0) && ((lsize == -1) || (i.root->key <= g_max)))
   is equivalent to
    ((lsize != 0) && (lsize == -1)) || ((lsize != 0) && (i.root->key <= g_max))
   is equivalent to
    (lsize == -1) || ((lsize != 0) && (i.root->key <= g_max))
   
  if root==NULL {
     return 0;
  }
  isBST = true;

  lsize <= LBST(i.root->left)
  
  if !((lsize == 0) || ((lsize>=0) && i.root->key > g_max))
     isBST = false
  curmin = (lsize == 0)?i.root->key:g_min

  rsize <= LBST(i.root->right)
  if !((rsize == 0) || ((rsize>=0) && i.root->key < g_min))
     isBST = false
  curmax = (rsize == 0)?i.root->key:g_max

  if (isBST) {
     // we are the largest subtree @ this level
     size = lsize + rsize + 1;
     if (g_largestbstsize < o.size) {
        g_largestbstsize = o.size;  
        g_largestbst = root;
     }
     g_min = curmin
     g_max = curmax
     return size;
  }
  else 
    return -1;


stack space per level:
isBST + size + curmin + curmax + lsize + rsize => 6


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

struct lbst_op LBST1(bnode *root)
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

  l = LBST1(root->left);
  r = LBST1(root->right);

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


//code iteration 2:
//=================
// instead of global variable, we are using top of recursion stack variable 

int _LBST2(bnode *root, int *min, int *max, bnode **largestbst, int *largestbstsize)
{
  int lsize, rsize, isBST, curmin, curmax, size;

  if (root==NULL) 
     return 0;
  isBST = true;

  lsize = _LBST2(root->left, min, max, largestbst, largestbstsize);
  if (!((lsize == 0) || ((lsize>=0) && (root->key > *max))))
     isBST = false;
  curmin = (lsize == 0)?root->key:*min;

  rsize = _LBST2(root->right, min, max, largestbst, largestbstsize);
  if (!((rsize == 0) || ((rsize>=0) && (root->key < *min))))
     isBST = false;
  curmax = (rsize == 0)?root->key:*max;

  if (isBST) {
     // we are the largest subtree @ this level
     size = lsize + rsize + 1;
     if (*largestbstsize < size) {
        *largestbstsize = size;  
        *largestbst = root;
     }

     *min = curmin;
     *max = curmax;
     return size;
  }
  else 
    return -1;
}

bnode *LBST2(bnode *root)
{
   bnode *largestbst;
   int min,max,largestbstsize=INT_MIN;

   _LBST2(root, &min, &max, &largestbst, &largestbstsize);
   return largestbst;
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
      o = LBST1(btroot);
      print_tree(o.largestbst);
      printf ("binary search tree method 4:\n");
      print_tree(LBST2(btroot));




      /* delete tree */
      delete_tree (btroot);
      btroot = NULL;
   }
}
