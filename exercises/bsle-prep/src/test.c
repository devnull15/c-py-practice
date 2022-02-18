#include <stdio.h>
#include <stdlib.h>
#include <test.h>

#ifdef LL
#include <ll.h>
#endif


#ifdef LL
void test_ll_node_free(void *p) {
  node *n = (node*)p;
  free(n->data);
  n->data = NULL;
  n->next = NULL;
  n->f = NULL;
  free(n);
  return;
}

int32_t* ll_make_data(uint32_t i) {
  int32_t *data = NULL;
  
  data = calloc(1, sizeof(uint32_t));
  if(NULL == data) {
    fprintf(stderr, "! calloc error in test_ll\n");
  }

  *data = i;

  return data;
}

int32_t test_ll() {
  int32_t ret = 0;
  ll *list = NULL;
  node_free f = test_ll_node_free;
  node *n = NULL;


  
  printf("*** Testing ll_init ***\n");
  list = ll_init();
  if(NULL == list) {
    fprintf(stderr, "! ll_init failed\n");
    ret = -1;
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);

  
  printf("*** Testing ll_len ***\n");
  ret = ll_len(list);
  if(0 > ret) {
    fprintf(stderr, "! ll_len failed\n");
    goto RET;
  }
  printf("* list len = %i\n", ret);
  printf("*** ret=%i Passed. ***\n\n", ret);


  printf("*** Testing ll_insert ***\n");
  for(int i = 0; i < 10; i++) {
    ret = ll_insert(list, i, (void*)ll_make_data(i), f);
    if(0 > ret) {
      fprintf(stderr, "! ll_insert failed\n");
      goto RET;
    }
  }
  ll_print(list);
  printf("*** ret=%i Passed. ***\n\n", ret);


  ret = 0;
  printf("*** Testing ll_get ***\n");
  for(int i = 0; i < 10; i++) {
    n = ll_get(list, i);
    printf("** ll_get: %i == %i?\n", i, *(int32_t*)n->data);
    if(NULL == n || i != *(int32_t*)(n->data)) {
      fprintf(stderr, "ll_get failed\n");
      ret = -1;
      goto RET;
    }
  }
  printf("*** ret=%i Passed. ***\n\n", ret);


  printf("*** Testing ll_set ***\n");
  ret = ll_set(list, 5, ll_make_data(2015));
  if(0 > ret) {
    fprintf(stderr, "ll_set failed\n");
    goto RET;
  }
  n = ll_get(list, 5);
  printf("** ll_get: %i\n", *(int32_t*)n->data);
  

  printf("*** ret=%i Passed. ***\n\n", ret);

  
  printf("*** Testing ll_rm ***\n");
  ret = ll_rm(list, 0);
  if(0 > ret) {
    fprintf(stderr, "ll_rm 0 failed\n");
    goto RET;
  }
  ret = ll_rm(list, 5);
  if(0 > ret) {
    fprintf(stderr, "ll_rm 5 failed\n");
    goto RET;
  }
  ret = ll_rm(list, 8);
  if(0 <= ret) {
    fprintf(stderr, "ll_rm 8 failed\n");
    goto RET;
  }
  ret = ll_rm(list, 7);
  if(0 > ret) {
    fprintf(stderr, "ll_rm 7 failed\n");
    goto RET;
  }
  ll_print(list);
  printf("*** ret=%i Passed. ***\n\n", ret);


  printf("*** Testing ll_len ***\n");
  ret = ll_len(list);
  if(7 != ret) {
    fprintf(stderr, "! ll_len failed\n");
    goto RET;
  }
  printf("* list len = %i\n", ret);
  printf("*** ret=%i Passed. ***\n\n", ret);

  
  printf("*** Testing ll_destory ***\n");
  ret = ll_destroy(list);
  if(0 > ret) {
    fprintf(stderr, "ll_destroy failed\n");
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);



 RET:
  return ret;
}
#endif

int32_t main() {
  int32_t err = 0;

#ifdef LL
  err = test_ll();
  if(0 > err) {
    fprintf(stderr, "!!! test_ll failed\n");    
  }
#endif

  return err;
}
