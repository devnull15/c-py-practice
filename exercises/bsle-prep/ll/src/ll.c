#include <ll.h>
#include <stdlib.h>
#include <stdio.h>

ll *ll_init() {
  ll *list = calloc(1, sizeof(ll));
  if(NULL == list) {
    fprintf(stderr, "! ll_init error\n");
  }

  return list;
}

int32_t ll_len(ll *list) {
  int32_t ret = 0;

  if(NULL == list) {
    fprintf(stderr, "! NULL list given to ll_len\n");
    ret = -1;
    goto RET;
  }

  node *n = list->head;

  while(NULL != n) {
    ret++;
    n = n->next;
  }

 RET: 
  return ret;
}

int32_t ll_insert(ll *list, uint32_t i, void *val, node_free f) {
  int32_t ret = 0;
  
  if(NULL == list || NULL == val || NULL == f) {
    fprintf(stderr, "! can't have NULL arguements in ll_insert\n");
    ret = -1;
    goto RET;
  }

  node *n = list->head;
  node *prev = NULL;
  node *insert = calloc(1, sizeof(node));
  if(NULL == insert) {
    fprintf(stderr, "! ll_insert calloc error\n");
    ret = -1;
    goto RET;
  }

  insert->data = val;
  insert->f = f;
  
  while(NULL != n && 0 < i) {
    i--;
    prev = n;
    n = n->next;
  }

  if(NULL != prev) {
    prev->next = insert;
  }
  else {
    list->head = insert;
  }

  insert->next = n;
  
 RET: 
  return ret;
}

int32_t ll_destroy(ll *list) {
  int32_t ret = 0;
  node *n = NULL;
  node *nx = NULL;
  
  if(NULL == list) {
    fprintf(stderr, "! can't destroy NULL list\n");
    ret = -1;
    goto RET;
  }

  n = list->head;

  while(NULL != n) {
    nx = n->next;
    (*(n->f))((void*)n);
    n = NULL;
    n = nx;
  }
  
  free(list);
  list = NULL;
  
 RET:
  return ret;
}

void  ll_print(ll* list) {
  node *n = NULL;

  if(NULL == list) {
    fprintf(stderr, "! NULL list in ll_print\n");
    goto RET;
  }

  printf("[ ");
  n = list->head;

  while(NULL != n) {
    printf("(<%p> data:%p f:%lx) -> ", (void*)n, (void*)n->data, (uint64_t)n->f);
    n = n->next;
  }

  printf("NULL ]\n");

 RET:
  return;
}


int32_t ll_set(ll* list, uint32_t i, void *val) {
  int32_t ret = 0;
  node *n = NULL;
  node_free f = NULL;
  
  if(NULL == list) {
    fprintf(stderr, "! NULL list in ll_set\n");
    ret = -1;
    goto RET;
  }

  n = ll_get(list, i);
  if(NULL == n) {
    fprintf(stderr, "! NULL node in ll_set\n");
    ret = -1;
    goto RET;
  }
  f = n->f;
  
  ret = ll_rm(list, i);
  if(0 > ret) {
    fprintf(stderr, "ll_set failed to remove node\n");
    goto RET;
  }

  ret = ll_insert(list, i, val, f);
  if(0 > ret) {
    fprintf(stderr, "ll_set failed to insert node\n");
    goto RET;
  }


 RET:
  return ret;
}

node *ll_get(ll* list, uint32_t i) {
  node *ret = NULL;
  
  if(NULL == list) {
    fprintf(stderr, "! NULL list in ll_get\n");
    goto RET;
  }
  
  ret = list->head;
  
  while(NULL != ret && 0 < i) {
    ret = ret->next;
    i--;
  }

  if(NULL == ret) {
    fprintf(stderr, "! index out of bounds in ll_get\n");
    goto RET;
  }

 RET:
  return ret;
  
}


int32_t ll_rm(ll *list, uint32_t i) {
  int32_t ret = 0;
  node *n = NULL;
  node *prev = NULL;
  node *nx = NULL;
  
  if(NULL == list) {
    fprintf(stderr, "! NULL list in ll_rm\n");
    ret = -1;
    goto RET;
  }

  n = list->head;

  while(NULL != n && 0 < i) {
    prev = n;
    n = n->next;
    i--;
  }

  if(NULL == n) {
    fprintf(stderr, "! index out of bounds in ll_rm\n");
    ret = -1;
    goto RET;
  }

  nx = n->next;
  (*(n->f))((void*)n);
  n = NULL;

  if(NULL != prev) {
    prev->next = nx;
  }
  else {
    list->head = nx;
  }

 RET:
  return ret;
  
}

