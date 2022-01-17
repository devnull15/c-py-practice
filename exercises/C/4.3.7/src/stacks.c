#include "../include/stacks.h"
#include <stdarg.h>
#include <stdlib.h>

extern int g_arrays_fd_stdout;
extern int g_arrays_fd_stderr;
extern int g_arrays_fd_stdin;

// these have been provided for the student
static void local_printf(const char * format, ...) 
{
  va_list args; 
  va_start( args, format);
  vdprintf( g_arrays_fd_stdout, format, args );
  va_end( args );
}


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
STACK_p_t create_stack(int *items, const size_t numItems) {
  STACK_p_t s = calloc(1, sizeof(STACK_t));
  if(NULL == s) {
    fprintf(stderr, "!!! calloc error in create_stack\n")
    return NULL;
  }
  for(size_t i = 1; i <= numItems; i++) {
    push(s, items[numItems - i]);
  }

  return s;
}

/**
 * @brief add an item to the top of a stack
 * 
 * @param stack stack to push onto
 * @param item item to push onto stack
 * 
 * Items used:
 *  3.3.9.c
 */
void push(STACK_p_t stack, const int item) {
  if(NULL == stack) {
    fprintf(stderr, "!!! NULL stack in push\n")
      return NULL;
  }
  if(stack == NULL) { return; }
  NODE_p_t n = calloc(1, sizeof(NODE_t));
  n->data = item;
  n->next = stack->top;
  stack->top = n;
    
  return;
}


/**
 * @brief return the item at the top of the stack and remove it from the stack
 * 
 * @param stack stack to pop from
 * @return item popped from stack, or NULL if stack is empty
 * 
 * Items used:
 *  3.3.9.b
 */
NODE_p_t pop(STACK_p_t stack) {
  if(stack == NULL) { return NULL; }
  NODE_p_t n = stack->top;
  if(n  == NULL) { return NULL; }
  else { stack->top = n->next; }
  return n;
}

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
NODE_p_t pop_at(STACK_p_t stack, int index) {
  if(stack == NULL || stack->top == NULL) { return NULL; }
  NODE_p_t prev = NULL;
  NODE_p_t n = stack->top;
  while(index != 0 && n != NULL) {
    prev = n;
    n = n->next;
    index--;
  }
  if(index != 0) { return NULL; }
  if(prev != NULL) { prev->next = n->next; }
  else { stack->top = n->next; }
  
  return n;
}



/**
 * @brief return the item at the top of the stack without removing it from the stack
 * 
 * @param stack stack to pop from
 * @return item peeked from stack, or NULL if stack is empty
 * 
 * Items used:
 *  3.3.9.d
 */
NODE_p_t peek(STACK_p_t stack) {
  if(stack == NULL) { return NULL; }
  return stack->top;
}



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
NODE_p_t peek_at(STACK_p_t stack, int index) {
  if(stack == NULL || stack->top == NULL) { return NULL; }
  NODE_p_t n = stack->top;
  
  while(index != 0 && n != NULL) {
    n = n->next;
    index--;
  }
  if(index != 0) { return NULL; }

  return n;
}


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

void print_stack(STACK_p_t stack) {
  if(stack == NULL) { return; }
  if(stack->top == NULL) { local_printf("Stack()\n"); }
  else {
    local_printf("Stack(");
    NODE_p_t n = stack->top;
    while(n->next != NULL) {
      local_printf("%i, ", n->data);
      n = n->next;
    }
    local_printf("%i)\n", n->data);
  }
  
  return;
}


/**
 * @brief remove all items from the stack
 * 
 * @param list reference stack to clear
 * 
 * Items used:
 *  3.3.9.e
 */

void clear(STACK_p_t stack) {
  if(stack == NULL) { return; }
  NODE_p_t n = NULL;
  while(stack->top != NULL) {
    n = pop(stack);
    free(n);
  }
  return;
}


/**
 * @brief destroy the stack and set its pointer to NULL
 * 
 * @param list reference to list to destroy
 * 
 * Items used:
 *  3.3.9.f
 */

void destroy(STACK_p_t stack) {
  clear(stack);
  free(stack);
  stack = NULL;
  return;
}

