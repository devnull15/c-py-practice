#ifndef JQR_336_TREE_H
#define JQR_336_TREE_H
#include <stdbool.h>

/*
 * Tree
 *
 * This tree node holds a single signed integer for the sake of this exercise.
 * For real-world use it could point to another struct or a void pointer.
 * In addition to this 'data' this data structure contains pointers to both a
 * left and right tree node.
 */
typedef struct _Tree_node Tree_node;

struct _Tree_node
{
    int data;
    Tree_node *left;
    Tree_node *right;
};

/*
 * This "overarching" struct is intended to encapsulate the head node for the
 * binary tree data structure.
 */
typedef struct _tree
{
    Tree_node *head;
} Tree;

// Binary Tree Methods to implement

/* tree_new() creates a new tree
    Args:
        No arguments
    Return:
        Tree * (also equal to tree*) to a newly allocated tree.
*/
Tree *tree_new(void);

/* tree_add() adds to existing tree
    Args:
        Tree * tr : the Tree * to add the element to
        int data : the specific data element to add
    Return:
        bool: true on success, false on failure
*/
bool tree_add(Tree *tr, int data);

/* tree_find() searches a tree for the first occurance of a given value
    Args:
        Tree * tr : the Tree * to find the element in
        int data : the specific data element to find
    Return:
        Tree_node* of the found node. null if node not found.
*/
Tree_node *tree_find(Tree *tr, int data);

/* tree_rm_head() removes only the head of a tree
    Note: You should not orphan child nodes, you should retain
    all other nodes in the tree after removing the head. 
    Args:
        Tree * tr : the Tree * to remove the head from
    Return:
        Tree_node* of the removed head. null if tr is empty.
*/
Tree_node *tree_rm_head(Tree *tr);

/* tree_destroy() destroys a tree
    Args:
        Tree * tr : Tree *  to destroy
    Return:
        bool : true on success, false of failure
*/
bool tree_destroy(Tree *tr);

#endif
