//ref: http://www.cs.columbia.edu/~aya/W3101-01/lectures/linked-list.pdf

#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>

typedef void (*node_free)(void *p);

typedef struct _node node;
struct _node {
  void *data;
  node *next;
  node_free f;
};

typedef struct _ll {
  node *head;
} ll;

ll* ll_init (void); /* initialize an empty list */
int ll_insert(ll*, uint32_t i, void *val, node_free f); /* insert val at n */
int32_t ll_len(ll*); /* returns list length */
int32_t ll_destroy(ll*); /* deletes list */
int32_t ll_set(ll*, uint32_t i, void *val); /* modifies item at n to val*/
node* ll_get(ll*, uint32_t i); /* returns value at n*/
int32_t ll_rm(ll*, uint32_t i); /* removes node n from list */
void ll_print(ll*); /* pretty prints the list */


#endif /* _LIST_H */
