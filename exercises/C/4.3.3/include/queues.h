#ifndef JQR_335_QUEUES_H
#define JQR_335_QUEUES_H

#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <unistd.h>

/*
   JQR Items Covered: 

    condition: B
    standard: C
    sub_items:
    3.3.5.a: creating a queue with n number of items
    3.3.5.b: enqueue a new item (both Queue and Priority Queue)
    3.3.5.c: dequeue a new item (both Queue and Priority Queue)
    3.3.5.d: peek next item (without removing from queue)(both Queue and Priority Queue)
    3.3.5.e: removing all items from the queue
    3.3.5.f: destroying a queue
    title: 'Demonstrate skill in creating and using a queue/priority queue:'

 */

/**
 * @brief whenever writing to stdout, functions must write to this file descriptor to allow for test cases to monitor standard out.
 *   see: dprintf, vdprintf, write
 * 
 */
extern volatile int g_arrays_fd_stdout;

/**
 * @brief whenever writing to stderr, functions must write to this file descriptor to allow for test cases to monitor standard err.
 *   see: dprintf, vdprintf, write
 * 
 */
extern volatile int g_arrays_fd_stderr;

/**
 * @brief whenever reading from stdin, functions must read from this file descriptor to allow for test cases to provide input to stdin
 *   see: dprintf, vdprintf, write
 * 
 */
extern volatile int g_arrays_fd_stdin;

/**
 * @brief node for our queues. We will be using a linked list implementation for our queues.
 *  For the sake of learning and simplicity,
 *  this implementation will only accept integers, but note that you could
 *  use void pointers to represent generic data, but that adds another layer of complexity.
 *  We will be enqueueing items to the tail of the queue, and dequeueing items from the head of the queue.
 */

typedef struct q_node
{
    int data;
    struct q_node *next;
} Q_NODE_t, *Q_NODE_p_t;

/**
 * @brief a struct to represent our queue
 */

typedef struct queue
{
    Q_NODE_p_t head;
    Q_NODE_p_t tail;
} QUEUE_t, *QUEUE_p_t;

typedef struct pq_item
{
    int data;
    int priority;
} PQ_ITEM_t, *PQ_ITEM_p_t;

typedef struct pq_node
{
    struct pq_item item;
    struct pq_node *next;
} PQ_NODE_t, *PQ_NODE_p_t;

/**
 * @brief a struct to represent our priority queue. Priority is an int, and lower int equals higher priority
 */

typedef struct p_queue
{
    PQ_NODE_p_t head;
} PQUEUE_t, *PQUEUE_p_t;

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

QUEUE_p_t create_queue(int *items, int numItems);

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

PQUEUE_p_t create_p_queue(PQ_ITEM_t items[], int numItems);

/**
 * @brief enqueue an item to the tail of a queue
 * 
 * @param queue queue to add item to
 * @param item item to add to queue
 * 
 * Items used:
 *  3.3.5.b
 */

void enqueue(QUEUE_p_t queue, int item);

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

void p_enqueue(PQUEUE_p_t queue, PQ_ITEM_t item);

/**
 * @brief dequeue an item from the head of a queue
 * 
 * @param queue queue to add item to
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.c
 */

int dequeue(QUEUE_p_t queue);

/**
 * @brief dequeue the highest priority item from the head of a priority queue
 * 
 * @param queue queue to add item to
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.c
 */

int p_dequeue(PQUEUE_p_t queue);

/**
 * @brief return the item from the head of a queue without removing the item from the queue
 * 
 * @param queue queue to peek
 * @return the item that was at the head of the queue, or -1 if the queue was empty
 * 
 * Items used:
 *  3.3.5.d
 */

int peek(QUEUE_p_t queue);

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

int p_peek(PQUEUE_p_t queue);

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

void print_queue(QUEUE_p_t list);

/**
 * @brief remove all items from the queue
 * 
 * @param queue reference queue to clear
 * 
 * Items used:
 *  3.3.5.e
 */

void clear(QUEUE_p_t list);

/**
 * @brief remove all items from the priority queue
 * 
 * @param queue reference priority queue to clear
 * 
 * Items used:
 *  3.3.5.e
 */

void p_clear(PQUEUE_p_t list);

/**
 * @brief destroy the queue and set its pointer to NULL
 * 
 * @param queue reference to queue to destroy
 * 
 * Items used:
 *  3.3.5.f
 */

void destroy(QUEUE_p_t queue);

/**
 * @brief destroy the priority queue and set its pointer to NULL
 * 
 * @param queue reference to priority queue to destroy
 * 
 * Items used:
 *  3.3.5.f
 */

void p_destroy(PQUEUE_p_t queue);

#endif
