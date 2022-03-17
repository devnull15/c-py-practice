#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <test.h>

#ifdef LL
#include <ll.h>
#endif

#ifdef THREADPOOL
#include <threadpool.h>
#include <stdatomic.h>
#endif

#ifdef NETPOLL
#include <netpoll.h>
#include <sys/socket.h>
#include <signal.h>
#endif


#define MAX_MESSAGE 2048

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

int32_t* test_ll_make_data(uint32_t i) {
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
  void *data = NULL;


  
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
    ret = ll_insert(list, i, (void*)test_ll_make_data(i), f);
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
  ret = ll_set(list, 5, test_ll_make_data(2015));
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


  printf("*** Testing push_front ***\n");
  ret = push_front(list, test_ll_make_data(0) , f);
  if(0 > ret) {
    fprintf(stderr, "! push_front failed\n");
    goto RET;
  }
  printf("* list len = %i\n", ll_len(list));
  printf("*** ret=%i Passed. ***\n\n", ret);

  
  printf("*** Testing push_back ***\n");
  ret = push_back(list, test_ll_make_data(4443) , f);
  if(0 > ret) {
    fprintf(stderr, "! push_back failed\n");
    goto RET;
  }
  printf("* list len = %i\n", ll_len(list));
  printf("*** ret=%i Passed. ***\n\n", ret);

  
  printf("*** Testing pop_front ***\n");
  data = pop_front(list);
  if(NULL == data) {
    fprintf(stderr, "! pop_front failed\n");
    goto RET;
  }
  printf("* list len = %i\n", ll_len(list));
  printf("* data = %i\n", *(int32_t*)data);
  free(data);
  data = NULL;
  printf("*** ret=%i Passed. ***\n\n", ret);


  printf("*** Testing pop_back ***\n");
  data = pop_back(list);
  if(NULL == data) {
    fprintf(stderr, "! pop_back failed\n");
    goto RET;
  }
  printf("* list len = %i\n", ll_len(list));
  printf("* data = %i\n", *(int32_t*)data);
  free(data);
  data = NULL;
  printf("*** ret=%i Passed. ***\n\n", ret);

  
  printf("*** Testing ll_destroy ***\n");
  ret = ll_destroy(list);
  if(0 > ret) {
    fprintf(stderr, "ll_destroy failed\n");
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);



 RET:
  return ret;
}
#endif /* TEST_LL */

#ifdef THREADPOOL
static void increment_int(void *arg) {
  (*(atomic_uint*)arg)++;
  return;
}




int32_t test_threadpool() {
  int32_t ret = 0;
  threadpool *pool = NULL;
  uint32_t nthreads = 100;
  uint32_t njobs = 100000;
  atomic_uint counter = 0;

  printf("*** Testing thpool_init ***\n");
  pool = thpool_init(nthreads);
  if(NULL == pool) {
    fprintf(stderr, "! thpool_init failed\n");
    ret = -1;
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);

  printf("*** Testing thpool_add_job ***\n");
  for(uint i = 0; i < njobs; i++) {
    ret = thpool_add_job(pool, increment_int, &counter);
    if(0 != ret) {
      fprintf(stderr, "! thpool_add_job failed\n");
      goto RET;
    }
  }
  printf("** counter=%u **\n\n", counter);
  printf("*** ret=%i Passed. ***\n\n", ret);

  printf("*** Testing thpool_destroy ***\n");
  while(njobs != counter) {
    printf("** counter=%u **\n\n", counter);
    sleep(1);
  }
  printf("** counter=%u **\n\n", counter);
  ret = thpool_destroy(pool);
  if(0 != ret) {
    fprintf(stderr, "! thpool_destroy failed\n");
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);



 RET:
  return ret;
}
#endif /* TEST_THREADPOOL */

#ifdef NETPOLL

void test_rh(int sfd) {
  char buf[MAX_MESSAGE] = {0};

  tcp_read_handler(sfd, buf, 1);
  printf("%s", buf);
  tcp_write_handler(sfd, buf, 1);
  
}

void test_inthandler(int signo) {
  if(SIGINT == signo) {
    netpoll_keepalive = 0;
  }
}

int32_t test_netpoll() {
  int ret = 0;
  int sockfd = 0;
  uint16_t port = 22016;
  int ipDomain = AF_INET;
  int maxcon = 1;
  int maxpend = 16;
  int timeout = 6000;
  struct sigaction sigact = {0};

  sigact.sa_handler = test_inthandler;
  sigact.sa_flags = SA_RESTART;
  sigfillset(&sigact.sa_mask);
  
  ret = sigaction(SIGINT, &sigact, NULL);
  if(-1 == ret) {
    fprintf(stderr, "! signalaction failed\n");
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);
  
  
  printf("*** Testing tcp_socketsetup ***\n");
  sockfd = tcp_socketsetup(port, ipDomain, maxpend);
  if(-1 == sockfd) {
    fprintf(stderr, "! tcp_socketsetup failed\n");
    ret = -1;
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);

  printf("*** Testing tcp_netpoll ***\n");
  ret = tcp_netpoll(sockfd, test_rh, maxcon, timeout);
  if(-1 == sockfd) {
    fprintf(stderr, "! tcp_socketsetup failed\n");
    goto RET;
  }
  printf("*** ret=%i Passed. ***\n\n", ret);

 RET:
  return ret;
}
#endif /* TEST_NETPOLL */



int32_t main() {
  int32_t err = 0;

#ifdef LL
  err = test_ll();
  if(0 > err) {
    fprintf(stderr, "!!! test_ll failed\n");    
  }
#endif /* LL */

#ifdef THREADPOOL
  err = test_threadpool();
  if(0 > err) {
    fprintf(stderr, "!!! test_threadpool failed\n");    
  }
#endif /* THREADPOOL */

#ifdef NETPOLL  
  err = test_netpoll();
  if(0 > err) {
    fprintf(stderr, "!!! test_netpoll failed\n");    
  }
#endif /* NETPOLL */

  

  return err;
}
