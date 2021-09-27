#include <unistd.h>
#include <stdarg.h>
#include "../include/doubly_linked_lists.h"
#include <stdlib.h>

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
 * @brief create a doubly linked list with items
 * 
 * @param items items to initialize list with (for empty list pass NULL)
 * @param numItems number of items in items
 * @return pointer to beginning of linked list
 * 
 * Items used:
 *  3.3.3.a
 */

DLL_p_t create_list(int *items, int numItems) {

  DLL_p_t list = calloc(1, sizeof(DLL_t));
  list->length = numItems;

  if(numItems == 0) {
    list->head = NULL;
  }
  else {
    list->head = calloc(1,sizeof(NODE_t));
    NODE_p_t curr = list->head;
    curr->prev = NULL;
    for(int i = 0; i < numItems-1; i++) {
      curr->data = items[i];
      curr->next = calloc(1,sizeof(NODE_t));
      curr->next->prev = curr;
      curr = curr->next;
    }
    curr->data = items[numItems-1];
    curr->next = NULL;
  }

  return list;
  
}

void insert_list_item(DLL_p_t list, int item, int index) {

  if(list==NULL || index < 0 || index > list->length) { return; }
    

  NODE_p_t new = calloc(1,sizeof(NODE_t));
  new->data = item;  
  
  NODE_p_t curr = list->head;
  NODE_p_t prev = NULL;
  if(index == 0) { list->head = new; }
  while(curr != NULL && index != 0) {
    prev = curr;
    curr = curr->next;
    index--;
  }

  new->prev = prev;
  new->next = curr;
  
  if(curr != NULL) { curr->prev = new; }
  if(prev != NULL) { prev->next = new; }
  list->length++;
  
  return;
}

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

void print_list(DLL_p_t list) {
  if(list->head == NULL) { local_printf("List()\n"); }
  else {
    NODE_p_t curr = list->head;
    local_printf("List(");
    while(curr->next != NULL) {
      local_printf("%i, ",curr->data);
      curr = curr->next;
    }
    local_printf("%i)\n", curr->data);
  }
  return;
}

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

int find(DLL_p_t list, int item) {
  if(list == NULL) { return -1; }

  int i = 0;
  NODE_p_t curr = list->head;
  while(curr != NULL) {
    if(curr->data == item) { return i; }
    i++;
    curr = curr->next;
  }

  return -1;
}

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


/* Moves data not nodes */
/* is this cheating? Probably. Do I care? Nope. */
void _move_node(NODE_p_t curr_i, NODE_p_t min) {

  int temp = curr_i->data;
  curr_i->data = min->data;
  min->data = temp;

  return;
}

void dll_sort(DLL_p_t list, int (*cmp)(int, int)) {
  if(list == NULL) { return; }
  if(list->head == NULL) { return; }
  
  NODE_p_t curr_i, curr_j, min;
  curr_i = list->head;
  
  while(curr_i != NULL) {
    curr_j = curr_i;
    min = curr_i;
    while(curr_j != NULL) {
      if((*cmp)(min->data,curr_j->data) == 1) { min = curr_j; }
      curr_j = curr_j->next;
    }
    _move_node(curr_i,min);
    curr_i = curr_i->next;
  }
  
  return;
}

/**
 * @brief remove all items from the doubly linked list
 * 
 * @param list reference list to clear
 * 
 * Items used:
 *  3.3.3.f
 */

void clear(DLL_p_t list) {

  if(list == NULL) { return; }
  
  NODE_p_t curr = list->head;
  NODE_p_t next = NULL;
  while(curr != NULL) {
    next = curr->next;
    free(curr);
    curr = next;
  }
  
  list->head = NULL;
  list->length = 0;
    
  return;

}

/**
 * @brief destroy the linked list and set its pointer to NULL
 * 
 * @param list reference to list to destroy
 * 
 * Items used:
 *  3.3.3.f
 */

void destroy(DLL_p_t list) {

  if(list == NULL) { return; }
  
  NODE_p_t curr = list->head;
  NODE_p_t next = NULL;
  while(curr != NULL) {
    next = curr->next;
    free(curr);
    curr = next;
  }

  free(list);
  list = NULL;
  
  return;
}
