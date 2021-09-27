#include "../include/Cllist.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* circularly_llist_new() creates a new circularly linked list
    Args:
        No arguments
    Return:
        Circularly_llist * to a newly allocated circularly linked list.
*/
Circularly_llist *circularly_llist_new(void) {
  Circularly_llist *ret = calloc(1,sizeof(struct _Circularly_llist));
  return ret;
}

/* circularly_llist_add() adds to existing circularly linked list
    Args:
        Circularly_llist * list : the circularly_llist* to add the element to
        int data     : the specific data element to add
    Return:
        bool: true on success, false on failure
*/
bool circularly_llist_add(Circularly_llist *list, int data) {

  if(list == NULL) { return false; }

  Circularly_llist_node *curr = NULL;
  if(list->head == NULL || list->tail == NULL) {
    if((curr = calloc(1, sizeof(struct _Circularly_llist_node))) == NULL) { return false; }
    curr->data = data;
    curr->next = curr;
    list->head = curr;
    list->tail = curr;
  }

  else {
    if((list->tail->next = calloc(1, sizeof(struct _Circularly_llist_node))) == NULL) { return false; }
    list->tail = list->tail->next;
    list->tail->data = data;
    list->tail->next = list->head;
  }
    
  return true;
}


/* circularly_llist_find() searches a circularly linked list for the first occurance of a given value
    Args:
        Circularly_llist * list : the circularly_llist* to find the element in
        int data     : the specific data element to find
    Return:
        Circularly_llist_node* of the found node. null if node not found.
*/
Circularly_llist_node *circularly_llist_find(Circularly_llist *list, int data) {
  if(list == NULL || list->head == NULL) { return NULL; }

  Circularly_llist_node *curr = list->head;
  
  while(curr->next != list->head) {
    if(curr->data == data) { return curr; }
    curr = curr->next;
  }

  return NULL;
}

/* circularly_llist_rm_head() removes only the head of a circularly linked list
    Args:
        Circularly_llist * list : the circularly_llist* to remove the head from
    Return:
        Circularly_llist_node* of the removed head. null if list is empty.
*/
Circularly_llist_node *circularly_llist_rm_head(Circularly_llist *list) {
  if(list == NULL) { return NULL; }
  if(list->head == NULL) { return NULL; }
  Circularly_llist_node *ret = list->head;
  if(list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
  }
  else {
    list->head = list->head->next;
    list->tail->next = list->head;
  }

  return ret;
}

/* circularly_llist_rm_tail() removes only the tail of a circularly linked list
    Args:
        Circularly_llist * list : the Circularly_llist* to remove the tail from
    Return:
        Circularly_llist_node* of the removed tail. null if list is empty.
*/
Circularly_llist_node *circularly_llist_rm_tail(Circularly_llist *list) {
  if(list == NULL) { return NULL; }
  if(list->head == NULL) { return NULL; }
  
  Circularly_llist_node *curr = list->head;

  if(list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    return curr;
  }


  while(curr->next != list->tail) {
    curr = curr->next;
  }

  Circularly_llist_node *ret = curr->next;
  curr->next = list->head;
  list->tail = curr;
    
  if(list->head != NULL) { list->tail->next = list->head; }
  else { list->tail = NULL; }
  
  return ret;
}

/* circularly_llist_rm() removes from existing circularly linked list
    Args:
        Circularly_llist * list : the circularly_llist* to remove the element from
        int data     : the specific data element to remove
    Return:
        Circularly_llist_node* of the removed node. null if node not found.
*/
Circularly_llist_node *circularly_llist_rm(Circularly_llist *list, int data) {
  if(list == NULL) { return NULL; }
  if(list->head == NULL) { return NULL; }

 
  Circularly_llist_node *curr = list->head;
  Circularly_llist_node *prev = list->tail;
  Circularly_llist_node *ret = NULL;

  while(curr->next != list->tail) {
    if(curr->data == data) {
      ret = curr;
      if(curr == list->head) { list->head = curr->next; }
      if(curr == list->tail) { list->tail = prev; }
      prev->next = curr->next;
    }
    prev = curr;
    curr = curr->next;
  }

  return ret;
}

/* circularly_llist_destroy() destroys a circularly linked list
    Args:
        Circularly_llist *list : circularly_llist * to destroy
    Return:
        bool : true on success, false of failure
*/
bool circularly_llist_destroy(Circularly_llist *list) {
  
  if(list == NULL) { return true; }
  if(list->head == NULL) {
    free(list);
    list = NULL;
    return true;
  }

  Circularly_llist_node *curr = list->head;
  Circularly_llist_node *next = NULL;

  list->tail->next = NULL;
  while(curr != NULL) {
    next = curr->next;
    free(curr);
    curr = NULL;
    curr = next;
  }

  free(list);
  list = NULL;
  
  return true;
}

/* circularly_llist_is_circular() check if list is a circular linked list
    Args:
        Circularly_llist *list : circularly_llist * to check
    Return:
        bool : true on success(is circular), false of failure(is not circular)
*/
bool circularly_llist_is_circular(Circularly_llist *list){
  if(list == NULL) { return true; }
  if(list->head == NULL) { return true; }
  Circularly_llist_node *curr = list->head;
  while(curr != list->tail) {
    if(curr == NULL) { return false; }
    curr = curr->next;
  }
  if(curr->next == list->head) { return true; }
  else { return false; }
  
  return false;
}
