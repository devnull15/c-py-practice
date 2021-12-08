#include "../include/bst.h"
#include <stdarg.h>
#include <stdlib.h>

#define LEFT 1
#define RIGHT 0


volatile int g_arrays_fd_stdout = STDOUT_FILENO;
volatile int g_arrays_fd_stderr = STDERR_FILENO;
volatile int g_arrays_fd_stdin  = STDIN_FILENO;


// these have been provided for the student
static void local_printf(const char * format, ...) 
{
  va_list args; 
  va_start( args, format);
  vdprintf( g_arrays_fd_stdout, format, args );
  va_end( args );
}


/**
 * @brief create a new BST with the given elements. Assumes the items are added to the BST from 
 *        left to right
 * 
 * @param arr array of items to add (NULL if empty BST)
 * @param len length of array (0 if empty BST)
 * @return a pointer to the BST
 * 
 * JQR Items used:
 *  - 3.3.7.a
 */

BST_p_t create_bst(int *arr, int len) {

  BST_p_t bst = calloc(1, sizeof(BST_t));

  if(arr != NULL && len != 0) {
    for(int i = 0; i < len; i++) {
      insert(bst,arr[i]);
    }
  }

  
  return bst;
}

/**
 * @brief insert an item into the appropriate location in the BST
 * 
 * @param bst BST to insert item into
 * @param item item to insert
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.h
 */
void insert(BST_p_t bst, int item) {

  NODE_p_t curr = bst->root;
  NODE_p_t prev = NULL;
  NODE_p_t add = calloc(1,sizeof(NODE_t));
  add->data = item;
  int dir = -1;
  
  while(curr != NULL) {
    if(item < curr->data) {
      prev = curr;
      curr = curr->left;
      dir = LEFT;
    }
    else if(item > curr->data) {
      prev = curr;
      curr = curr->right;
      dir = RIGHT;
    }
    else { return;  } //no duplicates
  }
  
  if(dir == LEFT) { prev->left = add; }
  else if(dir == RIGHT) { prev->right = add; }
  else { bst->root = add; }

  //print_bst(bst);


  return;
}

/**
 * @brief find the first node in the tree with a given item
 * 
 * @param bst BST to search
 * @param item item to insert
 * @return pointer to the node with the item or NULL if item not in BST
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.c
 */

/* NODE_p_t _tree_find_helper(NODE_p_t n, int data) { */
/*   if(n == NULL) { return NULL; } */
/*   if(n->data == data) { return n; } */

/*   NODE_p_t l = NULL; */
/*   NODE_p_t r = NULL; */

  
/*   l = _tree_find_helper(n->left,data); */
/*   r = _tree_find_helper(n->right,data); */

/*   if(l != NULL) { return l; } */
/*   else { return r; } */
/* } */

NODE_p_t _tree_find_helper(NODE_p_t n, int data) {
  if(n == NULL) { return NULL; }

  NODE_p_t ret = NULL;

  if(data < n->data) { ret = _tree_find_helper(n->left,data); }
  else if(data > n->data) { ret = _tree_find_helper(n->right,data); }
  else if(data == n->data) { return n; }
  else {
    fprintf(stderr, "Error in _tree_find_helper\n");
    exit(1);
  }

  return ret;
}


NODE_p_t find(BST_p_t bst, int item) {
  if(bst == NULL) { return NULL; }
  return _tree_find_helper(bst->root, item);
}

/**
 * @brief remove the given item from the BST
 * 
 * @param bst BST to remove item from
 * @param item item to remove
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.d
 */

NODE_p_t _merge_trees(NODE_p_t pop, BST_p_t push) {
  if(pop == NULL || push == NULL) { return NULL; }
  
  
  if(pop->left == NULL && pop->right == NULL) {
    insert(push, pop->data);
    free(pop);
    pop = NULL;
    return NULL;
  }
  else {
    if(_merge_trees(pop->left,push) != NULL) {
      fprintf(stderr, "Something went wrong left\n");
      exit(1);
    }
    if(_merge_trees(pop->right,push) != NULL) {
      fprintf(stderr, "Something went wrong right\n");
      exit(1);
    }
    
    insert(push, pop->data);
    free(pop);
    pop = NULL;
    return NULL;
    
  }
  
  return push->root;
}



int _which_side(NODE_p_t n, int data) {
  if(data < n->data && n->left != NULL && n->left->data == data) { return LEFT; }
  else if(data > n->data && n->right != NULL && n->right->data == data) { return RIGHT; }
  else { return -1; }
}

//returns parent of node to be deleted
NODE_p_t _tree_remove_helper(NODE_p_t n, int data) {
  if(n == NULL) { return NULL; }
  if(n->data == data) {
    fprintf(stderr, "Error in _tree_remove_helper\n");
    return NULL;
  }

  if(_which_side(n,data) != -1) { return n; }

  NODE_p_t ret = NULL;
  if(data < n->data) { ret = _tree_remove_helper(n->left,data); }
  else if(data > n->data) { ret = _tree_remove_helper(n->right,data); }
  else {
    fprintf(stderr, "Error in _tree_find_helper\n");
    exit(1);
  }
  
  return ret;
}

void remove_node(BST_p_t bst, int item) {
  if(bst == NULL || bst->root == NULL) { return; }

  NODE_p_t rm_p = NULL;
  NODE_p_t rm = NULL;
  
  //check to see if root node is to be rm'd, otherwise use helper.
  if(bst->root->data != item) {
    rm_p = _tree_remove_helper(bst->root, item);
    if(_which_side(rm_p,item) == LEFT) {
      rm = rm_p->left;
      rm_p->left = NULL;
    }
    else if(_which_side(rm_p,item) == RIGHT) {
      rm = rm_p->right;
      rm_p->right = NULL;
    }
    else {
      fprintf(stderr, "Something went wrong remove_node\n");
      exit(1);
    }
  }
  else {
    rm = bst->root;
    bst->root = NULL;
  }
  
  NODE_p_t left = rm->left;
  NODE_p_t right = rm->right;
  free(rm);
  rm = NULL;
  _merge_trees(left, bst);
  _merge_trees(right, bst);

  return;
}

/**
 * @brief remove all items from BST 
 * 
 * @param bst BST to clear
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.e
 */

NODE_p_t _clear_helper(NODE_p_t n) {

  if(n == NULL) { return NULL; }
  
  if(  _clear_helper(n->left) == NULL && _clear_helper(n->right) == NULL) {
    free(n);
    n = NULL;
    return NULL;
  }
  else {
    fprintf(stderr, "_clear_helper something went wrong\n");
    exit(1);
  }

  return NULL;
}

void clear(BST_p_t bst) {
  _clear_helper(bst->root);

  return;
}

/**
 * @brief destroy a BST and free its associated memory, then set its pointer to NULL
 * 
 * @param bst BST to destroy
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.g
 */
void destroy(BST_p_t bst) {
  if(bst == NULL) { return; }
  if(bst->root != NULL) { clear(bst); }
  free(bst);
  bst = NULL;
  
  return;
}

/**
 * @brief print a BST in an inorder traversal (i.e. from smallest to largest)
 * 
 * @param bst BST to print
 * 
 * JQR Items used:
 *  - 3.3.7.b
 * 
 * Example:
 *          3
 *        /  \
 *       1    4
 *        \    \
 *         2    5
 * 
 *  BST(1, 2, 3, 4, 5, )
 * ^ MUST MATCH THIS FORMAT TO PASS TEST
 * Remember to end with a newline character
 */

void _print_helper(NODE_p_t n) {
  if(n == NULL) { return; }

  

  _print_helper(n->left);
  local_printf("%i, ", n->data);
  _print_helper(n->right);
  
  return;
}


void print_bst(BST_p_t bst) {
  local_printf("BST(");
  _print_helper(bst->root);
  local_printf(")\n");
  return;
}
