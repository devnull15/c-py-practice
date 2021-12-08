#ifndef JQR_337_BST_H
#define JQR_337_BST_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <unistd.h>

/*
   JQR Items Covered: 

    condition: B
standard: C
sub_items:
  3.3.7.a: creating a binary search tree with n numbers of items
  3.3.7.b: navigating through a binary search tree
  3.3.7.c: locating an item in a binary tree
  3.3.7.d: Removing selected items from the binary search tree
  3.3.7.e: removing all items from the binary search tree
  3.3.7.g: destroying a binary search tree
  3.3.7.h: add an item to a binary tree
title: Demonstrate skill in creating and using a binary search tree


 */

/**
 * @brief whenever writing to stdout, functions must write to this file descriptor to allow for test cases to monitor standard out.
 *   see: dprintf, vdprintf, write
 * 
 */
extern volatile int g_arrays_fd_stdout;

/**
 * @brief whenever writing to stderr, functions must write to this file descriptor to allow for test cases to monitor standard err.
 *   see: dprintf, vdprintf, write
 * 
 */
extern volatile int g_arrays_fd_stderr;

/**
 * @brief whenever reading from stdin, functions must read from this file descriptor to allow for test cases to provide input to stdin
 *   see: dprintf, vdprintf, write
 * 
 */
extern volatile int g_arrays_fd_stdin;

/**
 * @brief node for our Binary Search Trees (BST). We will be using a linked list implementation for our bst.
 *  For the sake of learning and simplicity,
 *  this implementation will only accept integers, but note that you could
 *  use void pointers to represent generic data, but that adds another layer of complexity.
 * For our BSTs, we will use the following definition:
 * Binary Search Tree, is a node-based binary tree data structure which has the following properties:
    The left subtree of a node contains only nodes with keys lesser than the node’s key.
    The right subtree of a node contains only nodes with keys greater than the node’s key.
    The left and right subtree each must also be a binary search tree.
    There must be no duplicate nodes.
 */

typedef struct node
{
    int data;
    struct node *left;
    struct node *right;
} NODE_t, *NODE_p_t;

/**
 * @brief a struct to represent our binary search tree
 */

typedef struct bst
{
    struct node *root;
} BST_t, *BST_p_t;

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

BST_p_t create_bst(int *arr, int len);

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
void insert(BST_p_t bst, int item);

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
NODE_p_t find(BST_p_t bst, int item);

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
void remove_node(BST_p_t bst, int item);

/**
 * @brief remove all items from BST 
 * 
 * @param bst BST to clear
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.e
 */
void clear(BST_p_t bst);

/**
 * @brief destroy a BST and free its associated memory, then set its pointer to NULL
 * 
 * @param bst BST to destroy
 * 
 * JQR Items used:
 *  - 3.3.7.b
 *  - 3.3.7.g
 */
void destroy(BST_p_t bst);

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
void print_bst(BST_p_t);

#endif
