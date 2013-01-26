#include <stdio.h>
#include <stdlib.h>

#define BTREE_INVALID        -1
#define MAX_BTREE_DEPTH      10
#define MAX_BTREE_IDX        (1<<MAX_BTREE_DEPTH)
int bstarray[1<<MAX_BTREE_DEPTH];
#define max_btree_idx_check(x,rv)  if(x > (1<<MAX_BTREE_DEPTH)) return rv;

#define leftval(a,x)        a[((x)*2)]
#define rightval(a,x)       a[((x)*2 + 1)]
#define parentval(a,x)      a[((int)(x)/2)]
#define left(x)             ((x)*2)
#define right(x)            ((x)*2 + 1)
#define parent(x)           ((int)(x)/2)



int insert (int *array, int key)
{
   int root = 1;

   while (array[root] != BTREE_INVALID) {
     if (array[root] > key) {
        root = left(root);
     } else if (array[root] < key) {
        root = right(root);
     } else {
       printf ("insert %d duplicate\n", key);
       return 0;
     }
     max_btree_idx_check(root, -1);
   }
   
   array[root] = key;
   return 0;
}

int search (int *array, int key)
{
   int root = 1;

   while (array[root] != BTREE_INVALID) {
     if (array[root] > key) {
        root = left(root);
     } else if (array[root] < key) {
        root = right(root);
     } else {
        return key;
     }
     max_btree_idx_check(root, BTREE_INVALID);
   }
   
   return BTREE_INVALID;
}


int largest(int *array, int root)
{
   max_btree_idx_check(right(root), root);

   while (rightval(array, root) != BTREE_INVALID) {
     root = right(root);
     max_btree_idx_check(root, parent(root));
   }
 
   return (root);
}


int smallest(int *array, int root)
{
   max_btree_idx_check(left(root), root);

   while (leftval(array, root) != BTREE_INVALID) {
     root = left(root);
     max_btree_idx_check(root, parent(root));
   }
 
   return (root);
}


#if 0
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
#endif

int count (int *array, int root)
{
   max_btree_idx_check(root, 0);

   if (array[root] == BTREE_INVALID) 
     return 0;

   return 1 + count(array, left(root)) + count(array, right(root));
}

int depth (int *array, int root)
{
   max_btree_idx_check(root, 0);

   if (array[root] == BTREE_INVALID) 
     return 0;

   return 1 + (count(array, left(root)) | count(array, right(root)));
}


void print_dotty (int *array, int root)
{
   if ((left(root) < MAX_BTREE_IDX) && (leftval(array, root)!=BTREE_INVALID))
     printf ("    %d -> %d;\n", array[root], leftval(array,root));

   if ((right(root) < MAX_BTREE_IDX) && (rightval(array, root)!=BTREE_INVALID))
     printf ("    %d -> %d;\n", array[root], rightval(array,root));
}


void printnode(int *array, int root) 
{
   printf ("%d ", array[root]);
}


bfs (int *array, int root, void (*fn) (int *, int))
{
   int c = count(array, root);
   int *q = malloc (sizeof(int)*c);
   int qidx = 0, pidx = 0, i;

   q[qidx++] = root;
   while (pidx < c) {
     if ((left(q[pidx]) < MAX_BTREE_IDX) && (leftval(array, q[pidx])!=BTREE_INVALID)) {
       q[qidx++] = left(q[pidx]);
     }
     
     if ((right(q[pidx]) < MAX_BTREE_IDX) && (rightval(array, q[pidx])!=BTREE_INVALID)) {
       q[qidx++] = right(q[pidx]);
     }
     pidx ++;
   }

   for (i=0; i<c; i++)
      fn(array, q[i]);

   free(q);
}

inorder(int *array, int root, void (*fn) (int *, int))
{
   max_btree_idx_check(root, 0);
   if (array[root] == BTREE_INVALID) 
     return 0;

   inorder(array, left(root), fn);
   fn(array, root);
   inorder(array, right(root), fn);
}

postorder(int *array, int root, void (*fn) (int *, int))
{
   max_btree_idx_check(root, 0);
   if (array[root] == BTREE_INVALID) 
     return 0;

   postorder(array, left(root), fn);
   postorder(array, right(root), fn);
   fn(array, root);
}

preorder(int *array, int root, void (*fn) (int *, int))
{
   max_btree_idx_check(root, 0);
   if (array[root] == BTREE_INVALID) 
     return 0;

   fn(array, root);
   preorder(array, left(root), fn);
   preorder(array, right(root), fn);
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

print_tree(int *array)
{
   /* print bst inorder => sorted */
   printf ("inorder:   ");
   inorder(array, 1, printnode);

   printf ("\npreorder:  ");
   preorder(array, 1, printnode);

   printf ("\npostorder: ");
   postorder(array, 1, printnode);

   printf ("\nbfs:       ");
   bfs(array, 1, printnode);

   printf ("\n");
   printf ("digraph bst {\n");
   preorder(array, 1, print_dotty);
   printf ("}\n");
}

int input[50];

main ()
{
   int input_size, i, trial;

   srandom ((unsigned)&i);

   for (trial=0; trial < 3; trial ++) {
      memset (bstarray, BTREE_INVALID, sizeof(bstarray));
      input_size = populate_array (input, sizeof(input)/sizeof(input[0]));

      print_array ("\nunsorted:  ", input, input_size);
      printf ("\n");

      /* convert array to bst */
      for (i=0; i<input_size; i++)
         if (insert (bstarray, input[i]))
            printf ("insert %d failed\n", input[i]);

      print_tree(bstarray);

#if 0
      for (i=0; i<input_size; i++) {
        delete (bstarray, input[i]); 
        printf ("\ndeleted: %d\n", input[i]);
        print_tree(bstarray);
      }
#endif
   }
}
