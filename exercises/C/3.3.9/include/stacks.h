#ifndef JQR_339_STACKS_H
#define JQR_339_STACKS_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <unistd.h>

/*
   JQR Items Covered: 

    condition: B
    standard: C
    sub_items:
    3.3.9.a: creating a stack with n number of items
    3.3.9.b: removing an item from the stack at the nth position
    3.3.9.c: adding an item on a stack
    3.3.9.d: peek an element of stack at the nth position without removing from stack
    3.3.9.e: removing all items from  the stack
    3.3.9.f: destroying a stack
    3.3.9.h: preventing a stack overrun
    3.3.9
    title: 'Demonstrate skill in creating and using a stack:'


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
 * @brief node for our stacks. A stack is a LIFO (Last in First Out) oriented data structure. 
 *  For the sake of learning and simplicity,
 *  this implementation will only accept integers, but note that you could
 *  use void pointers to represent generic data, but that adds another layer of complexity.
 *  The top of the stack has an index of 0, so a normal 'pop' function can be interpreted as
 *  using 'pop_at(stack,0)' in the below functions.
 */

typedef struct node
{
    int data;
    struct node *next;
} NODE_t, *NODE_p_t;

/**
 * @brief a struct to represent our stack
 */

typedef struct stack
{
    NODE_p_t top;
} STACK_t, *STACK_p_t;

/**
 * @brief create a stack with items
 *
 * Items should be added to the stack from right to left.
 * i.e. create_stack([1,2,3,4], 4) 
 * will first push 4 then 3 then 2 then 1
 * 
 * @param items items to initialize stack with (for empty stack pass NULL)
 * @param numItems number of items in items
 * @return pointer to beginning of linked list
 * 
 * Items used:
 *  3.3.9.a
 */
STACK_p_t create_stack(int *items, int numItems);

/**
 * @brief add an item to the top of a stack
 * 
 * @param stack stack to push onto
 * @param item item to push onto stack
 * 
 * Items used:
 *  3.3.9.c
 */
void push(STACK_p_t stack, int item);

/**
 * @brief return the item at the top of the stack and remove it from the stack
 * 
 * @param stack stack to pop from
 * @return item popped from stack, or NULL if stack is empty
 * 
 * Items used:
 *  3.3.9.b
 */
NODE_p_t pop(STACK_p_t stack);

/**
 * @brief return the item at the nth position of the stack and remove it from the stack
 * 
 * @param stack stack to pop from
 * @param index index of item to pop from stack
 * @return item popped from stack, or NULL if index is not in stack
 * 
 * Items used:
 *  3.3.9.b
 */
NODE_p_t pop_at(STACK_p_t stack, int index);

/**
 * @brief return the item at the top of the stack without removing it from the stack
 * 
 * @param stack stack to pop from
 * @return item peeked from stack, or NULL if stack is empty
 * 
 * Items used:
 *  3.3.9.d
 */
NODE_p_t peek(STACK_p_t stack);

/**
 * @brief return the item at the top of the stack without removing it from the stack
 * 
 * @param stack stack to peek from
 * @param index index of item to peek from stack
 * @return item popped from stack, or NULL if index is not in stack
 * 
 * Items used:
 *  3.3.9.d
 */
NODE_p_t peek_at(STACK_p_t stack, int index);

/**
 * @brief print each node in a stack
 * 
 * @param stack reference to stack to print
 * Example: Stack(1, 2, 3, 4, 5)
 *          Stack() for empty stack
 * 
 * Top of stack is left-most item
 * 
 * Items used:
 *  3.3.9.b
 */

void print_stack(STACK_p_t stack);

/**
 * @brief remove all items from the stack
 * 
 * @param list reference stack to clear
 * 
 * Items used:
 *  3.3.9.e
 */

void clear(STACK_p_t list);

/**
 * @brief destroy the stack and set its pointer to NULL
 * 
 * @param list reference to list to destroy
 * 
 * Items used:
 *  3.3.9.f
 */

void destroy(STACK_p_t stack);

#endif
