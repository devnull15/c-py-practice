#ifndef JQR_334_CIRCULARLY_LINKED_LISTS_H
#define JQR_334_CIRCULARLY_LINKED_LISTS_H
#include <stdbool.h>

/*
 * This node holds a single signed integer for the sake of this exercise.
 * For real-world use it could point to another struct or a void pointer.
 */
typedef struct _Circularly_llist_node Circularly_llist_node;

struct _Circularly_llist_node
{
    int data;
    Circularly_llist_node *next;
};

/*
 * This "overarching" struct is intended to encapsulate the nodes for both
 * the head and tail of the circularly linked list.
 */
typedef struct _Circularly_llist
{
    Circularly_llist_node *head;
    Circularly_llist_node *tail;
} Circularly_llist;

// Circularly Linked List Methods to implement

/* circularly_llist_new() creates a new circularly linked list
    Args:
        No arguments
    Return:
        Circularly_llist * to a newly allocated circularly linked list.
*/
Circularly_llist *circularly_llist_new(void);

/* circularly_llist_add() adds to existing circularly linked list
    Args:
        Circularly_llist * list : the circularly_llist* to add the element to
        int data     : the specific data element to add
    Return:
        bool: true on success, false on failure
*/
bool circularly_llist_add(Circularly_llist *list, int data);

/* circularly_llist_find() searches a circularly linked list for the first occurance of a given value
    Args:
        Circularly_llist * list : the circularly_llist* to find the element in
        int data     : the specific data element to find
    Return:
        Circularly_llist_node* of the found node. null if node not found.
*/
Circularly_llist_node *circularly_llist_find(Circularly_llist *list, int data);

/* circularly_llist_rm_head() removes only the head of a circularly linked list
    Args:
        Circularly_llist * list : the circularly_llist* to remove the head from
    Return:
        Circularly_llist_node* of the removed head. null if list is empty.
*/
Circularly_llist_node *circularly_llist_rm_head(Circularly_llist *list);

/* circularly_llist_rm_tail() removes only the tail of a circularly linked list
    Args:
        Circularly_llist * list : the Circularly_llist* to remove the tail from
    Return:
        Circularly_llist_node* of the removed tail. null if list is empty.
*/
Circularly_llist_node *circularly_llist_rm_tail(Circularly_llist *list);

/* circularly_llist_rm() removes from existing circularly linked list
    Args:
        Circularly_llist * list : the circularly_llist* to remove the element from
        int data     : the specific data element to remove
    Return:
        Circularly_llist_node* of the removed node. null if node not found.
*/
Circularly_llist_node *circularly_llist_rm(Circularly_llist *list, int data);

/* circularly_llist_destroy() destroys a circularly linked list
    Args:
        Circularly_llist *list : circularly_llist * to destroy
    Return:
        bool : true on success, false of failure
*/
bool circularly_llist_destroy(Circularly_llist *list);

/* circularly_llist_is_circular() check if list is a circular linked list
    Args:
        Circularly_llist *list : circularly_llist * to check
    Return:
        bool : true on success(is circular), false of failure(is not circular)
*/
bool circularly_llist_is_circular(Circularly_llist *list);

#endif
