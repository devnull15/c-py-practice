#include "../include/LinkedList.h"


/**
 * @brief Creates a newly allocated linked list
 * 
 * The custom del and cmp functions are tailored to the data type or data structure
 * being stored in the linked list.  For simple data types, passing the standard free
 * function as the del parameter will usually suffice. These function pointers allow
 * the library user to store arbitrary data in the linked list.
 * 
 * Asserts that the cmp function is not NULL.
 * If NULL is passed as the del parameter, implementation must set free as the default.
 * 
 * @param del a function pointer to a custom free function tailored to
 * @param cmp a function pointer to a custom compare function
 * @return linked_list_t* a pointer to an allocated linked list object
 */
linked_list_t *linked_list_new(del_f del, cmp_f cmp) {
  linked_list_t *list = NULL;
  assert(cmp != NULL);
  if(del == NULL) { del = free; }
  list = calloc(1,sizeof(struct linked_list_t));
  list->delete_func = del;
  list->compare_func = cmp;
  list->head = NULL;
  return list;
}

/**
 * @brief Destroy the list and all nodes it contains, deallocating any memory
 *        that was allocated.
 * 
 * Asserts that list is not NULL, however *list == NULL is safe. 
 * List pointer is set to NULL after memory is deallocated to 
 * mitgate chance of Use-after-free bugs.
 * 
 * @param list a reference to a pointer to an allocated list.
 */
void linked_list_delete(linked_list_t **list) {
  assert(list != NULL);
  if(*list == NULL) { return; }
  
  ll_node_t *curr = (*list)->head;
  ll_node_t *next = NULL;
  
  while(curr != NULL) {
    next = curr->next;
    (*(*list)->delete_func)(curr->data);
    (*(*list)->delete_func)(curr);
    curr = next;
  }

  free(*list);
  *list = NULL;
  return;
}

/**
 * @brief Returns the number of nodes in the linked list
 * 
 * Asserts that the list pointer is not NULL.
 * 
 * @param list a pointer to an allocated linked list object
 * @return size_t the number of nodes in the linked list
 */
size_t linked_list_size(linked_list_t *list) {
  assert(list != NULL);
  size_t num_nodes = 0;
  ll_node_t *curr = list->head;
  while(curr != NULL) {
    num_nodes++;
    curr = curr->next;
  }
  return num_nodes;
}

/**
 * @brief Prepends a node before the given index in the linked list.
 *        The index starts at 0.
 * 
 * A new node will be allocated with the data passed as a parameter to the function.
 * 
 * If 0 is provided as an index, a node will be prepended as the new head node, 
 * and the existing head node will become the next node in the list. If 1 is
 * provided as an index, a node will be inserted between the existing head node
 * and it's next node, with that existing next node becoming the new node's next
 * node.
 * 
 * Asserts that the list pointer is not NULL.
 * Asserts that index <= list size. If index == size, the element will be appended.
 * 
 * @param list a pointer to an allocated linked list object
 * @param index the index of the reference node to which this new node will be
 *              prepended
 * @param data the data that will be placed in the newly allocated node
 */
void linked_list_insert_before(linked_list_t *list, size_t index, void *data) {
  assert(list != NULL);
  size_t size = linked_list_size(list);
  assert(index <= size);

  if(index == size) { linked_list_append(list,data); }
  
  ll_node_t *prev = NULL;
  ll_node_t *curr = list->head;
  while(curr != NULL && index != 0) {
    prev = curr;
    curr = curr->next;
    index--;
  }
  
  if(curr == NULL) { linked_list_append(list,data); }

  if(prev == NULL) {
    list->head = calloc(1, sizeof(ll_node_t));
    list->head->next = curr;
    list->head->data = data;
  }
  else {
    prev->next = calloc(1, sizeof(ll_node_t));
    prev->next->next = curr;
    prev->next->data = data;
  }
  return;
}

/**
 * @brief Appends a node to the tail of the linked list
 * 
 * A new node will be allocated with the data passed as a parameter to this function.
 * 
 * Asserts that the list pointer is not NULL.
 * 
 * @param list a pointer to an allocated linked list object
 * @param data the data that will be placed in the newly allocated node
 */
void linked_list_append(linked_list_t *list, void *data) {
  assert(list != NULL);
  if(list->head != NULL) {
    ll_node_t *curr = list->head;
    while(curr->next != NULL) { curr = curr->next; }
    curr->next = calloc(1, sizeof(ll_node_t));
    curr->next->data = data;
    curr->next->next = NULL;
  }
  else {
    list->head = calloc(1, sizeof(ll_node_t));
    list->head->data = data;
    list->head->next = NULL;
  }
   
  return;
}

/**
 * @brief Finds the index of the first node matching the value parameter 
 *        starting at index given by the start parameter.  Matches are
 *        defined by the custom comparison function that was passed to the cmp 
 *        parameter in `linked_list_new`.
 * 
 * A start index of 0 means search from the beginning of the list.
 * 
 * Asserts that the list pointer is not NULL.
 * 
 * @param list a pointer to an allocated linked list object
 * @param value a pointer to the value being matched after the given start index
 * @param start the 0-indexed index within the list where matching should begin
 * @return size_t the index in the list where the match was found, or (size_t) -1 if not found
 */
size_t linked_list_find_first_after(linked_list_t *list, void *value, size_t start) {
  assert(list != NULL);
  size_t i = 0;
  size_t ret = (size_t)-1;
  ll_node_t *curr = list->head;
  
  while(curr !=NULL && start != 0) {
    curr = curr->next;
    start--;
    i++;
  }

  while(curr !=NULL) {
    if((*list->compare_func)(value,curr->data) == 0) {
      ret = i;
      break;
    }
    curr = curr->next;
    i++;
  }
  
  return ret;
}

/**
 * @brief Returns the data pointer in the 0-indexed node at index starting from the head
 * 
 * Asserts that the list pointer is not NULL.
 * Asserts that index is less than list size (since index starts at 0).
 * 
 * @param list a pointer to an allocated list
 * @param index the index of the node being
 * @return void* a pointer to the data contained in the node at the given index
 */
void *linked_list_at(linked_list_t *list, size_t index) {
  assert(list != NULL);
  assert(index < linked_list_size(list));
  ll_node_t *curr = list->head;
  void *ret = NULL;
  while(curr != NULL && index != 0) {
    curr = curr->next;
    index--;
  }
  if(curr != NULL) { ret = curr->data; }
  return ret;
}

/**
 * @brief Delete a node in the linked list at a given index. Index starts at 0.
 * 
 * Asserts that the list pointer is not NULL.
 * Asserts that index is less than list size (since index starts at 0).
 * 
 * @param list a pointer to an allocated linked list object
 * @param index the index of the node to be deleted, where index=0 is the head node
 */
void linked_list_del_at(linked_list_t *list, size_t index) {
  assert(list != NULL);
  assert(index < linked_list_size(list));
  ll_node_t *prev = NULL;
  ll_node_t *curr = list->head;
  while(curr != NULL && index != 0) {
    prev = curr;
    curr = curr->next;
    index--;
  }
  if(prev == NULL) { list->head = curr->next; }
  else { prev->next = curr->next; }
  if(curr != NULL) {
    (*list->delete_func)(curr->data);
    (*list->delete_func)(curr);
  } 

  return;
}

/**
 * @brief Calls the function specified in the action parameter on every node in
 *        the linked list.
 * 
 * Asserts that list pointer is NOT NULL.
 * 
 * @param list a pointer to an allocated linked list object
 * @param action a function pointer to a user-specified function that will be called
 *               on each node in the list. An example function might print the data
 *               in a node.
 *               
 * @param action_ctx a pointer to the second parameter defined by the action function. 
 *                   (The first parameter to action is the data pointer in a list node.)
 *                   An action function that doesn't require the second parameter can
 *                   simply set this to NULL.
 */
void linked_list_for_each(linked_list_t *list, action_f action, void *action_ctx) {
  assert(list != NULL);
  ll_node_t *curr = list->head;
  while(curr != NULL) {
    (*action)(curr->data, action_ctx);
    curr = curr->next;
  }
  return;
}

