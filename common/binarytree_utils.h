#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define max_val(x,y) (((x)>(y))?(x):(y))

typedef struct node {
   /* bare minimum binary tree node */
   struct node *left;
   struct node *right;
   int          key;

   /* ws print helper data */
   int          x;
   int          y;
   int          modifier;
   int          height;
} bnode;

#define delete_tree(r)   postorder(r, (void (*) (bnode *, void*))free, NULL)

bnode *bst_insert (bnode **root, int key);
bnode *bst_search (bnode *root, int key);
bnode *rightmost(bnode *root);
bnode *bst_largest(bnode *root);
bnode *bst_smallest(bnode *root);
void   bst_delete(bnode **root, int key);
int    count (bnode *root);
int    depth (bnode *root);
void   print_ws(bnode *root);
void   print_dotty (bnode *node, void *ctx);
void   printnode(bnode *n, void *ctx); 
void   bfs(bnode *root, void (*fn) (bnode *, void *ctx), void *ctx);
void   inorder(bnode *root, void (*fn) (bnode*, void*), void *ctx);
void   postorder(bnode *root, void (*fn) (bnode*, void*), void *ctx);
void   preorder(bnode *root, void (*fn) (bnode*, void*), void *ctx);
void   print_paths(bnode *root);
void   mirror(bnode *root);
int    random_swap(bnode *root, int swaps);
int    isbst(bnode *root);
void   print_tree(bnode *root);
void   populate_array (int *arr, int size);
void   print_array(char *header, int *a, int s);
bnode *random_insert(bnode **root, int key);
bnode *create_random_bst(int size);
bnode *create_random_bt(int size);
