#include <stdio.h>
#include <stdlib.h>

typedef struct node {
   struct node *left;
   struct node *right;
   int          key;
} bnode;

insert (bnode **root, int key)
{
   if (NULL == *root) {
     bnode *node;

     if (NULL == (node = (bnode*)malloc (sizeof (bnode)))) {
        return;
     }

     node->key    = key;
     node->right  = node->left = NULL;
     *root = node;
   }
   else if ((*root)->key > key) {
      insert (&(*root)->left, key);
   }
   else if ((*root)->key < key) {
      insert (&(*root)->right, key);
   }

   return;
}


bnode *search (bnode *root, int key)
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

bnode *largest(bnode *root)
{
  while (root->right != NULL) 
     root = root->right;
  return root;
}

bnode *smallest(bnode *root)
{
  while (root->left != NULL) 
     root = root->left;
  return root;
}


delete(bnode **root, int key)
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
      free (temp);
   } else if (NULL == (*root)->right) {
      temp = *root;
      *root = (*root)->left;
      free (temp);
   } else {
      /* swap out next smallest to current node */
      (*root)->key = (smallest((*root)->right))->key;
      /* delete next smallest node */
      delete (&(*root)->right, (*root)->key);
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

   return 1 + (depth(root->left) | depth(root->right));
}


void print_dotty (bnode *node)
{
   if (node->left)
     printf ("    %d -> %d;\n", node->key, node->left->key);

   if (node->right)
     printf ("    %d -> %d;\n", node->key, node->right->key);
}



#define delete_tree(r)      postorder(r, (void (*) (bnode *))free)

void printnode(bnode *n) 
{
   printf ("%d ", n->key);
}

bfs (bnode *root, void (*fn) (bnode *))
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
      fn(q[i]);
}

inorder(bnode *root, void (*fn) (bnode*))
{
    if (!root) return;
    inorder(root->left, fn);
    fn(root);
    inorder(root->right, fn);
}

postorder(bnode *root, void (*fn) (bnode*))
{
    if (!root) return;
    postorder(root->left, fn);
    postorder(root->right, fn);
    fn(root);
}

preorder(bnode *root, void (*fn) (bnode*))
{
    if (!root) return;
    fn(root);
    preorder(root->left, fn);
    preorder(root->right, fn);
}


print_tree(bnode *root)
{
   /* print bst inorder => sorted */
   printf ("inorder:   ");
   inorder(root, printnode);

   printf ("\npreorder:  ");
   preorder(root, printnode);

   printf ("\npostorder: ");
   postorder(root, printnode);

   printf ("\nbfs:       ");
   bfs(root, printnode);

   printf ("\n");
   printf ("digraph bst {\n");
   preorder (root, print_dotty);
   printf ("}\n");
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


/*************************************
              main 
 ************************************/

int input[25];
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
         insert (&bstroot, input[i]);

      print_tree(bstroot);

      for (i=0; i<input_size; i++) {
        delete (&bstroot, input[i]); 
        printf ("\ndeleted: %d\n", input[i]);
        print_tree(bstroot);
      }

      /* delete tree */
      delete_tree (bstroot);
      bstroot = NULL;
   }
}
