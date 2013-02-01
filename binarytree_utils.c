#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "binarytree_utils.h"

bnode *_bst_insert (bnode **root, int key, int d)
{
   if (NULL == *root) {
     bnode *node;

     if (NULL == (node = (bnode*)malloc (sizeof (bnode)))) {
        return NULL;
     }

     node->key    = key;
     node->height = d;
     node->right  = node->left = NULL;
     *root = node;
     return node;
   }
   else if ((*root)->key > key) {
      return _bst_insert (&(*root)->left, key, ++d);
   }
   else if ((*root)->key < key) {
      return _bst_insert (&(*root)->right, key, ++d);
   }

   return NULL;
}

bnode *bst_insert (bnode **root, int key)
{
   return _bst_insert (root, key, 0);
}


bnode *bst_search (bnode *root, int key)
{
   while (root != NULL) {
     if (root->key == key) 
        break;
     else if (root->key > key) 
        root = root->left;
     else 
        root = root->right;
   }
  
   return root;
}

bnode *rightmost(bnode *root)
{
  while (root->right != NULL) 
     root = root->right;
  return root;
}

bnode *bst_largest(bnode *root)
{
  return rightmost(root);
}



bnode *bst_smallest(bnode *root)
{
  while (root->left != NULL) 
     root = root->left;
  return root;
}

void decrement_height (bnode *root, void *ctx)
{
   root->height --;
}

void bst_delete(bnode **root, int key)
{
   bnode *temp;

   while (*root != NULL) {
     if ((*root)->key == key) 
        break;
     else if ((*root)->key > key) 
        root = &(*root)->left;
     else 
        root = &(*root)->right;
   }
  
   if (NULL == *root) 
      return;

   if ((NULL == (*root)->left) && (NULL == (*root)->right)) {
      free (*root);
      *root = NULL;
   } else if (NULL == (*root)->left) {      
      temp = *root;
      *root = (*root)->right;
      inorder (*root, decrement_height, NULL);
      free (temp);
   } else if (NULL == (*root)->right) {
      temp = *root;
      *root = (*root)->left;
      inorder (*root, decrement_height, NULL);
      free (temp);
   } else {
      /* swap out next smallest to current node */
      (*root)->key = (bst_smallest((*root)->right))->key;
      /* bst_delete next smallest node */
      bst_delete (&(*root)->right, (*root)->key);
   }
}


int count (bnode *root)
{
   if (root == NULL) 
     return 0;

   return 1 + count(root->left) + count(root->right);
}


int depth (bnode *root)
{
   if (root == NULL) 
     return 0;

   return 1 + max_val(depth(root->left), depth(root->right));
}



/*
  http://poincare.matf.bg.ac.rs/~tijana/geometrija/seminarski/tree_drawing.pdf
  based on "WETHERELL AND SHANNON: TIDY DRAWING OF TREES"
*/
struct ws_tidy_ctx {
    int *modifier;
    int *nextpos;
};


#define max(a,b)  ((a)>(b)?(a):(b))


ws_tidy_phase1 (bnode *root, void *c)
{
   struct ws_tidy_ctx *ctx = (struct ws_tidy_ctx *) c;
   int place;
   int is_leaf;
   int h = root->height;
   

   is_leaf = (NULL == root->left) && (NULL == root->right);
   if (is_leaf) {
      place = ctx->nextpos[h];
   }
   else if (NULL == root->left) {
      place = root->right->x - 1;
   }
   else if (NULL == root->right) {
      place = root->left->x + 1;
   }
   else {
      place = (root->left->x + root->right->x)/2;
   }

   ctx->modifier[h] = max(ctx->modifier[h], ctx->nextpos[h]-place); 
   if (is_leaf) {
      root->x = place;
   } 
   else {
      root->x = place + ctx->modifier[h];
   }

   ctx->nextpos[h] = root->x + 2; 
   root->modifier = ctx->modifier[h]; 
}

ws_tidy_phase2 (bnode *root, int modifier_sum, struct ws_tidy_ctx  *ctx)
{
   if (!root) return;

   root->x      = root->x + modifier_sum; 
   root->y      = 2*root->height + 1; 
   modifier_sum = modifier_sum + root->modifier; 

   ws_tidy_phase2 (root->left, modifier_sum, ctx);
   ws_tidy_phase2 (root->right, modifier_sum, ctx);
}

ws_tidy_nodereset(bnode *root, void *c)
{
   root->x = 0;
   root->y = 0;
   root->modifier = 0;
}

ws_tidy(bnode *root)
{
    int max_height = depth(root);
    struct ws_tidy_ctx  ctx;
    int i;

    inorder (root, ws_tidy_nodereset, NULL);

    ctx.modifier = (int*) malloc (max_height * sizeof(int)); 
    ctx.nextpos = (int*) malloc (max_height * sizeof(int)); 
    
    for (i=0; i < max_height; i++) {
       ctx.modifier[i] = 0;
       ctx.nextpos[i] = 1;
    }

    postorder (root, ws_tidy_phase1, (void*)&ctx);
    ws_tidy_phase2(root, 0, &ctx);

    free (ctx.modifier);
    free(ctx.nextpos);
}

struct printnode_ws_ctx {
      int lastx;
      int lasty;
      char *connector_line1;
      int last_connector_x;
      char *connector_line2;
};


printnode_ws_ctx_init (struct printnode_ws_ctx *ctx) 
{
  memset (ctx, 0, sizeof(*ctx));
}

void printnode_ws_ctx_cleanup(struct printnode_ws_ctx *ctx) 
{
   if (NULL != ctx->connector_line1) free(ctx->connector_line1);
   if (NULL != ctx->connector_line2) free(ctx->connector_line2);
}

void printnode_ws(bnode *n, struct printnode_ws_ctx *ctx) 
{
#define ns 4
   int deltax;
 
   if (NULL == ctx->connector_line1) {
      int maxx = (rightmost(n)->x);
      ctx->connector_line1 = malloc (ns * 2 * maxx);
      ctx->connector_line2 = malloc (ns * 2 * maxx);
   }

   if (ctx->lasty != n->y) {
      printf ("\n%s", ctx->connector_line1);
      printf ("\n%s", ctx->connector_line2);
      printf ("\n");
      ctx->lasty = n->y;
      ctx->lastx = 0;
      ctx->last_connector_x = 0;
      ctx->connector_line1[0] = '\0';
      ctx->connector_line2[0] = '\0';
   }

   deltax = n->x - ctx->lastx - 1;
   printf ("%*s%*d", deltax*ns, " ", ns, n->key);
   sprintf (ctx->connector_line1 + strlen(ctx->connector_line1), "%*s%s", deltax*ns + ns/2 + 1, " ", (n->left||n->right)?"|":" ");
   ctx->lastx = n->x;

#if 1
#define hdash    '-'
#define vdash    '|'
#define lterm    '+'
#define rterm    '+'
#define center   '+'
#define lcenter  '+'
#define rcenter  '+'
#else
#define hdash    196
#define vdash    '|'
#define lterm    218
#define rterm    191
#define center   193
#define lcenter  217
#define rcenter  192
#endif

   if (n->left) {
      char *s = ctx->connector_line2 + strlen(ctx->connector_line2);
      int leftspc = (n->left->x - ctx->last_connector_x - 1)*ns + ns/2 + 1;
      int leftdash  = (n->x - n->left->x - 1)*ns + ns/2 + 1;

      memset(&s[0],' ',leftspc); 
      s[leftspc] = lterm;
      memset(&s[leftspc+1],hdash,leftdash); 
      s[leftspc+1+leftdash] = lcenter;
      s[leftspc+1+leftdash+1]= '\0';
      ctx->last_connector_x = n->x;

      if (n->right) {
        int rightdash  = (n->right->x - n->x - 1)*ns + ns/2 + 1;
        s[leftspc+1+leftdash] = center;
        memset(&s[leftspc+leftdash+2],hdash,rightdash); 
        s[leftspc+leftdash+rightdash+2]= rterm;
        s[leftspc+leftdash+rightdash+3]= '\0';
        ctx->last_connector_x = n->right->x;
      }
   }
   else if (n->right) {
      char *s = ctx->connector_line2 + strlen(ctx->connector_line2);
      int leftspc = (n->x - ctx->last_connector_x - 1)*ns + ns/2 + 1;
      int rightdash  = (n->right->x - n->x - 1)*ns + ns/2 + 1;

      memset(&s[0],' ',leftspc); 
      s[leftspc] = rcenter;
      memset(&s[leftspc+1],hdash,rightdash); 
      s[leftspc+1+rightdash] = rterm;
      s[leftspc+1+rightdash+1] = '\0';

      ctx->last_connector_x = n->right->x;
   }

}


void print_ws(bnode *root)
{
   struct printnode_ws_ctx ctx;

   ws_tidy (root);
   printnode_ws_ctx_init (&ctx);
   bfs(root, printnode_ws, &ctx);
   printnode_ws_ctx_cleanup (&ctx);
}




void print_dotty (bnode *node, void *ctx)
{
   if (node->left)
     printf ("    %d -> %d;\n", node->key, node->left->key);

   if (node->right)
     printf ("    %d -> %d;\n", node->key, node->right->key);
}


void printnode(bnode *n, void *ctx) 
{
   printf ("%d ", n->key);
}

void bfs (bnode *root, void (*fn) (bnode *, void *ctx), void *ctx)
{
   int c = count(root);
   bnode **q = malloc (sizeof (bnode *) * c  );
   int qidx = 0, pidx = 0, i;

   q[qidx++]   = root;
   while (pidx < c) {
     if (NULL != q[pidx]->left) {
       q[qidx++] = q[pidx]->left;
     }
     
     if (NULL != q[pidx]->right) {
       q[qidx++] = q[pidx]->right;
     }
     pidx ++;
   }

   for (i=0; i<c; i++)
      fn(q[i], ctx);

   free(q);
}

void inorder(bnode *root, void (*fn) (bnode*, void*), void *ctx)
{
    if (!root) return;
    inorder(root->left, fn, ctx);
    fn(root, ctx);
    inorder(root->right, fn, ctx);
}

void postorder(bnode *root, void (*fn) (bnode*, void*), void *ctx)
{
    if (!root) return;
    postorder(root->left, fn, ctx);
    postorder(root->right, fn, ctx);
    fn(root, ctx);
}

void preorder(bnode *root, void (*fn) (bnode*, void*), void *ctx)
{
    if (!root) return;
    fn(root, ctx);
    preorder(root->left, fn, ctx);
    preorder(root->right, fn, ctx);
}



void print_path(bnode *root, int *path, int len)
{
   int i;
   if (NULL == root) return;

   path[len++] = root->key;

   if ((NULL == root->left) && (NULL == root->right)) {
     printf("%d", path[0]);
     for (i = 1; i < len; i++) 
        printf("->%d", path[i]);
     printf ("\n");
   } else {
     print_path(root->left, path, len);
     print_path(root->right, path, len);
   }
}

void print_paths(bnode *root)
{
   int max_height = depth(root);
   int *path = (int*) malloc (sizeof(int)*(max_height + 1));

   print_path (root, path, 0);

   free (path);
}

void _mirror(bnode *root, void* ctx)
{
  bnode *temp;
  
  temp = root->left;
  root->left = root->right;
  root->right = temp;
}

void mirror(bnode *root)
{
   postorder(root, _mirror, NULL);
}


int _random_swap(bnode *root, bnode *swapnode, int swaps)
{
   int temp;
   if (swaps == 0) return 0;
   if (root == NULL) return swaps;

   if ((random() % 10) > 5) {
      if (NULL == swapnode) {
         swapnode = root;
      } else {
         swaps--;
         temp = root->key;
         root->key = swapnode->key;
         swapnode->key = temp;
         swapnode = NULL;
      }
   }

   return _random_swap (root->left, swapnode, swaps);
   return _random_swap (root->right, swapnode, swaps);
}

int random_swap(bnode *root, int swaps)
{
  return _random_swap(root, NULL, swaps);
}

int _isbst(bnode *root, int min, int max)
{
   if (!root) return 1;

   if ((root->key > max) || (root->key < min)) {
      printf ("%d is out of order\n", root->key);
      return 0;
   }

   return _isbst(root->left, min, root->key) && _isbst(root->right, root->key, max);
}

int isbst(bnode *root)
{
   return _isbst(root, INT_MIN, INT_MAX);
}


void print_tree(bnode *root)
{
#if 0
   /* print bst inorder => sorted */
   printf ("inorder:   ");
   inorder(root, printnode, NULL);

   printf ("\npreorder:  ");
   preorder(root, printnode, NULL);

   printf ("\npostorder: ");
   postorder(root, printnode, NULL);

   printf ("\nbfs:       ");
   bfs(root, printnode, NULL);

   printf ("\n");
   printf ("digraph bst {\n");
   preorder (root, print_dotty, NULL);
   printf ("}\n");
#endif
   print_ws(root);
   printf ("\n");
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
