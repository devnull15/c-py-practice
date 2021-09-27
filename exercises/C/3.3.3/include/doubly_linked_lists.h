#ifndef JQR_333_DOUBLY_LINKED_LISTS_H
#define JQR_333_DOUBLY_LINKED_LISTS_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <unistd.h>

/*
   JQR Items Covered: 

    condition: B
    standard: C
    sub_items:
        3.3.3.a: creating a doubly linked list with n number of items
        3.3.3.b: navigating through the doubly linked list
        3.3.3.c: finding the first occurrence of an item in a doubly linked list
        3.3.3.d: sorting the doubly linked list alphanumerically using a function pointer
        3.3.3.e: inserting an item into a specific location in a doubly linked list
        3.3.3.f: removing all items from the doubly linked list
        3.3.3.g: destroying a doubly linked list
    title: 'Demonstrate skill in creating and using a doubly linked list:'
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
 * @brief node for our doubly linked lists. A list is generally described as 
 *  a reference to the head of the list. For the sake of learning and simplicity,
 *  this implementation will only accept integers, but note that you could
 *  use void pointers to represent generic data, but that adds another layer of complexity
 */

typedef struct l_node
{
    int data;
    struct l_node *next;
    struct l_node *prev;
} NODE_t, *NODE_p_t;

/**
 * @brief a struct to represent our list
 */

typedef struct dll
{
    NODE_p_t head;
    int length;
} DLL_t, *DLL_p_t;

/**
 * @brief create a doubly linked list with items
 * 
 * @param items items to initialize list with (for empty list pass NULL)
 * @param numItems number of items in items
 * @return pointer to beginning of linked list
 * 
 * Items used:
 *  3.3.3.a
 */

DLL_p_t create_list(int *items, int numItems);

/**
 * @brief insert an item to list at given index (0-indexed)
 * 
 * @param list reference to list to insert node into
 * @param  iutem item to insert
 * @param index index to insert node
 * 
 * Items used:
 *  3.3.3.e
 */

void insert_list_item(DLL_p_t list, int item, int index);

/**
 * @brief print each node in a list given the printing function
 * 
 * @param list reference to list to print
 * Example: List(1, 2, 3, 4, 5)
 *          List() for empty list
 * 
 * Items used:
 *  3.3.3.b
 */

void print_list(DLL_p_t list);

/**
 * @brief find the first occurrence of an item in a doubly linked list
 * 
 * @param list reference list to search
 * @param  item item to find
 * @return index of first occurence of item in list, -1 if item not in list
 * 
 * Items used:
 *  3.3.3.c
 */

int find(DLL_p_t list, int item);

/**
 * @brief sort a list according to a comparison function
 * 
 * @param list reference to list to search
 * @param  cmp pointer to comparison function.
 *  cmp returns
 *   -1 if left-hand-side goes before right-hand-side
 *    0 if the values are equal
 *    1 if left-hand-side goes after right-hand-side
 * 
 * Items used:
 *  3.3.3.d
 */

void dll_sort(DLL_p_t list, int (*cmp)(int, int));

/**
 * @brief remove all items from the doubly linked list
 * 
 * @param list reference list to clear
 * 
 * Items used:
 *  3.3.3.f
 */

void clear(DLL_p_t list);

/**
 * @brief destroy the linked list and set its pointer to NULL
 * 
 * @param list reference to list to destroy
 * 
 * Items used:
 *  3.3.3.f
 */

void destroy(DLL_p_t list);

#endif
