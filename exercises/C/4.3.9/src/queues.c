#include <queues.h>
#include <stdarg.h>
#include <assert.h>
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
 * @brief create a queue with items
 * 
 * @param items items to initialize queue with (for empty queue pass NULL)
 * @param numItems number of items in items
 * @return pointer to beginning of queue
 * 
 * Items used:
 *  3.3.5.a
 */

QUEUE_p_t create_queue(int *items, int numItems) {
  QUEUE_p_t q = calloc(1, sizeof(QUEUE_t));
  Q_NODE_p_t prev = NULL;
  Q_NODE_p_t n = NULL;
  for(int i = 0; i < numItems; i++) {
    prev = n;
    n = calloc(1, sizeof(Q_NODE_t));
    assert(NULL != n);
    q->tail = n;
    n->data = items[i];
    n->next = NULL;
    if(prev != NULL) { prev->next = n; }
    if(i == 0) { q->head = n; }
  }
  
  return q;
}

/**
 * @brief create a priority queue with items
 * 
 * @param items items to initialize queue with (for empty queue pass NULL)
 * @param numItems number of items in items
 * @return pointer to beginning of queue
 * 
 * Items used:
 *  3.3.5.a
 */

PQUEUE_p_t create_p_queue(PQ_ITEM_t items[], int numItems) {
  PQUEUE_p_t q = calloc(1, sizeof(PQUEUE_t));
  PQ_NODE_p_t prev = NULL;
  PQ_NODE_p_t n = NULL;
  for(int i = 0; i < numItems; i++) {
    p_enqueue(q,items[i]);
  }
  
  return q;
}

/**
 * @brief enqueue an item to the tail of a queue
 * 
 * @param queue queue to add item to
 * @param item item to add to queue
 * 
 * Items used:
 *  3.3.5.b
 */

void enqueue(QUEUE_p_t queue, int item) {
  if(queue == NULL) { return; }
  Q_NODE_p_t n = calloc(1,sizeof(Q_NODE_t));
  assert(NULL != n);
  if(queue->tail != NULL) { queue->tail->next = n; }
  queue->tail = n;
  n->data = item;
  if(queue->head == NULL) { queue->head = n; }
  return;
}

/**
 * @brief enqueue an item onto its appropriate location on the priority queue.
 *        This means to append the item such that is is behind the last item 
 *        of its same priority in the queue.
 * 
 * @param queue queue to add item to
 * @param item item to add to queue
 * 
 * Items used:
 *  3.3.5.b
 */

void p_enqueue(PQUEUE_p_t queue, PQ_ITEM_t item) {
  if(queue == NULL) { return; }

  PQ_NODE_p_t n = calloc(1,sizeof(PQ_NODE_t));
  n->item = item;

  fprintf(stderr, "penqueue\n");
  if(queue->head == NULL) {
    queue->head = n;
    n->next = NULL;
  }
  else {
    PQ_NODE_p_t curr = queue->head;
    PQ_NODE_p_t prev = NULL;

    while(curr != NULL && curr->item.priority <= n->item.priority) {
      prev = curr;
      curr = curr->next;
    }
    if(prev != NULL) { prev->next = n; }
    else { queue->head = n; }
    n->next = curr;
  }

  return;
}

/**
 * @brief dequeue an item from the head of a queue
 * 
 * @param queue queue to add item to
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.c
 */

int dequeue(QUEUE_p_t queue) {
  if(queue == NULL || queue->head == NULL) { return -1; }

  Q_NODE_p_t ret = queue->head;
  queue->head = queue->head->next;
  if(queue->head == NULL) { queue->tail = NULL; }

  int ret_i = ret->data;
  free(ret);
  ret = NULL;
  
  return ret_i;
}

/**
 * @brief dequeue the highest priority item from the head of a priority queue
 * 
 * @param queue queue to add item to
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.c
 */

int p_dequeue(PQUEUE_p_t queue) {
  if(queue == NULL || queue->head == NULL) { return -1; }
    
  PQ_NODE_p_t ret = queue->head;
  queue->head = queue->head->next;
  
  int ret_i = ret->item.data;
  free(ret);
  ret = NULL;
  
  return ret_i;
}

/**
 * @brief return the item from the head of a queue without removing the item from the queue
 * 
 * @param queue queue to peek
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.d
 */

int peek(QUEUE_p_t queue) {
  if(queue->head == NULL) { return -1; }
  return queue->head->data;
}

/**
 * @brief return the highest priority item from the head of a priority queue 
 *        without removing the item from the queue
 * 
 * @param priority queue queue to peek
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.d
 */

int p_peek(PQUEUE_p_t queue) {
  if(queue->head == NULL) { return -1; }
  return queue->head->item.data;
  
}

/**
 * @brief print each node in a queue
 * 
 * @param list reference to list to print
 * Example: Queue(1, 2, 3, 4, 5)
 *          Queue() for empty list
 * 
 * Items used:
 *  3.3.3.b
 */

void print_pqueue(PQUEUE_p_t queue) {
  if(queue == NULL) { return; }

  local_printf("Queue(");
  PQ_NODE_p_t n = queue->head;

  if(n != NULL) {
    while(n->next != NULL) {
      local_printf("%i, ", n->item.data);
      n = n->next;
    }
    local_printf("%i)\n", n->item.data);
  }
  else { local_printf(")\n"); }

  
  return;
}


void print_queue(QUEUE_p_t queue) {
  if(queue == NULL) { return; }

  local_printf("Queue(");
  Q_NODE_p_t n = queue->head;

  if(n != NULL) {
    while(n->next != NULL) {
      local_printf("%i, ", n->data);
      n = n->next;
    }
    local_printf("%i)\n", n->data);
  }
  else { local_printf(")\n"); }

  
  return;
}

/**
 * @brief remove all items from the queue
 * 
 * @param queue reference queue to clear
 * 
 * Items used:
 *  3.3.5.e
 */

void clear(QUEUE_p_t queue) { 
  if(queue == NULL || queue->head == NULL) { return; }

  int i = 0;
  while(i != -1) {
      i = dequeue(queue);
    }
 
  return;
} 

/**
 * @brief remove all items from the priority queue
 * 
 * @param queue reference priority queue to clear
 * 
 * Items used:
 *  3.3.5.e
 */

void p_clear(PQUEUE_p_t queue) {
  if(queue == NULL || queue->head == NULL) { return; }

  int i = 0;
  while(i != -1) {
      i = p_dequeue(queue);
    }
 
  return;
}

/**
 * @brief destroy the queue and set its pointer to NULL
 * 
 * @param queue reference to queue to destroy
 * 
 * Items used:
 *  3.3.5.f
 */

void destroy(QUEUE_p_t queue) {
  if(queue == NULL) { return; }
  if(queue->head != NULL) {
    int i = 0;
    while(i != -1) {
      i = dequeue(queue);
    }
  }
  
  free(queue);
  queue = NULL;
  
  return;
}

/**
 * @brief destroy the priority queue and set its pointer to NULL
 * 
 * @param queue reference to priority queue to destroy
 * 
 * Items used:
 *  3.3.5.f
 */

void p_destroy(PQUEUE_p_t queue) {
  fprintf(stderr,"destroy\n");
  if(queue == NULL) { return; }
  if(queue->head != NULL) {
    int i = 0;
    while(i != -1) {
      i = p_dequeue(queue);
    }
  }
  
  free(queue);
  queue = NULL;
  
  return;

  
  return;
}
