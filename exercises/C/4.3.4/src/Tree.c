#include <stdlib.h>
#include <stddef.h>
#include "../include/Tree.h"
#include <stdio.h>

#define LEFT 0
#define RIGHT 1
 
/* tree_new() creates a new tree
    Args:
        No arguments
    Return:
        Tree * (also equal to tree*) to a newly allocated tree.
*/
Tree *tree_new(void) {
  Tree *ret = calloc(1,sizeof(Tree));
  if(NULL == ret) {
    fprintf(stderr, "!!! Error in tree_new calloc\n");
    return NULL;
  }
  
  return ret;
}

/* tree_add() adds to existing tree
    Args:
        Tree * tr : the Tree * to add the element to
        int data : the specific data element to add
    Return:
        bool: true on success, false on failure
*/
bool tree_add(Tree *tr, int data) {
  if(tr == NULL) { return false; }

  Tree_node *curr = tr->head;
  Tree_node *prev = NULL;
  int dir = -1;
  
  while(curr != NULL) {
    if(data <= curr->data) {
      prev = curr;
      curr = curr->left;
      dir = LEFT;
    }
    else {
      prev = curr;
      curr = curr->right;
      dir = RIGHT;
    }
  }
  
  if(dir == LEFT) {
    prev->left = calloc(1,sizeof(Tree_node));
    if(NULL == prev->left) {
      fprintf(stderr, "!!! Error in tree_add LEFT calloc\n");
      return NULL;
    }
    curr = prev->left;
  }
  else if(dir == RIGHT) {
    prev->right = calloc(1,sizeof(Tree_node));
    if(NULL == prev->right) {
      fprintf(stderr, "!!! Error in tree_add RIGHT calloc\n");
      return NULL;
    }
    curr = prev->right;
  }
  else {
    tr->head = calloc(1,sizeof(Tree_node));
    if(NULL == tr->head) {
      fprintf(stderr, "!!! Error in tree_add HEAD calloc\n");
      return NULL;
    }
    curr = tr->head;
  }
  
  curr->data = data;
  
  return true;
}

Tree_node * _tree_find_helper(Tree_node *n, int data) {
  if(n == NULL) { return NULL; }
  if(n->data == data) { return n; }

  Tree_node *l = NULL;
  Tree_node *r = NULL;

  
  l = _tree_find_helper(n->left,data);
  r = _tree_find_helper(n->right,data);

  if(l != NULL) { return l; }
  else { return r; }

}

/* tree_find() searches a tree for the first occurance of a given value
    Args:
        Tree * tr : the Tree * to find the element in
        int data : the specific data element to find
    Return:
        Tree_node* of the found node. null if node not found.
*/
Tree_node *tree_find(Tree *tr, int data) {
  if(tr == NULL) { return NULL; }
  return _tree_find_helper(tr->head, data);
}

/* tree_rm_head() removes only the head of a tree
    Note: You should not orphan child nodes, you should retain
    all other nodes in the tree after removing the head. 
    Args:
        Tree * tr : the Tree * to remove the head from
    Return:
        Tree_node* of the removed head. null if tr is empty.
*/


Tree_node *_merge_trees(Tree_node *pop, Tree *push) {
  if(pop == NULL || push == NULL) { return NULL; }
  
  
  if(pop->left == NULL && pop->right == NULL) {
    if(!tree_add(push, pop->data)) { fprintf(stderr, "tree_add Error!\n"); } 
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
    
    if(!tree_add(push, pop->data)) { fprintf(stderr, "tree_add Error!\n"); } 
    free(pop);
    pop = NULL;
    
    return NULL;
    
  }
  
  return push->head;
}

Tree_node *tree_rm_head(Tree *tr) {
  fprintf(stderr, "rm_head\n");
  if(tr == NULL || tr->head == NULL) { return NULL; }
  Tree_node *ret = tr->head;
  Tree_node *pop = tr->head->left;
  tr->head = tr->head->right;
  _merge_trees(pop,tr);
  
  
  return ret;
}

/* tree_destroy() destroys a tree
    Args:
        Tree * tr : Tree *  to destroy
    Return:
        bool : true on success, false of failure
*/

void _destroy_nodes(Tree_node *n) {
  if(n == NULL) { return; }
  _destroy_nodes(n->left);
  _destroy_nodes(n->right);
  free(n);
  n = NULL;
  return; 
}

bool tree_destroy(Tree *tr) {
  if(tr == NULL) { return false; }
  _destroy_nodes(tr->head);
  free(tr);
  tr = NULL;
  return true;
}
