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

ll* ll_init (void);
int32_t ll_insert(ll*, uint32_t i, void *val, node_free f);
int32_t ll_len(ll*);
int32_t ll_destroy(ll*);
int32_t ll_set(ll*, uint32_t i, void *val);
node* ll_get(ll*, uint32_t i);
int32_t ll_rm(ll*, uint32_t i);
void ll_print(ll*);
int32_t push_front(ll*, void *val, node_free f);
int32_t push_back(ll*, void *val, node_free f);
void* pop_front(ll*);
void* pop_back(ll*);


#endif /* _LIST_H */
