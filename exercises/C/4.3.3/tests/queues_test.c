#ifndef JQR_335_QUEUES_TEST_H
#define JQR_335_QUEUES_TEST_H

#include "../include/queues.h"
#include <stdlib.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>

/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

#define TEST_STDOUT_CAPTURE_FILE "/tmp/CUnit_JQR_335_QUEUES_TEST_stdout.log"

extern volatile int g_arrays_fd_stdout;
static FILE *fp_stdout_file = NULL;
static int cur_stdout_fd;

// Tells if queue contents match array contents
static int testQueueContents(QUEUE_p_t queue, int *arr, int len)
{
    Q_NODE_p_t current = queue->head;
    for (int index = 0; index < len; index++)
    {
        if ((current == NULL) || (current->data != arr[index]))
            return 0;
        current = current->next;
    }
    return 1;
}

static int testPQueueContents(PQUEUE_p_t queue, PQ_ITEM_t *arr, int len)
{
    PQ_NODE_p_t current = queue->head;
    for (int index = 0; index < len; index++)
    {
        if ((current == NULL) ||
            (current->item.data != arr[index].data) ||
            (current->item.priority != arr[index].priority))
            return 0;
        current = current->next;
    }
    return 1;
}

void jqr_test_create_queue(void)
{
    // Check for empty queue
    QUEUE_p_t emptyQueue = create_queue(NULL, 0);
    CU_ASSERT_PTR_NULL_FATAL(emptyQueue->head);
    CU_ASSERT_PTR_NULL_FATAL(emptyQueue->tail);
    destroy(emptyQueue);

    // Check for populated queue
    int test[] = {1, 2, 3, 4, 5};
    QUEUE_p_t popQueue = create_queue(test, 5);
    CU_ASSERT_PTR_NOT_NULL_FATAL(popQueue->head);
    CU_ASSERT_PTR_NOT_NULL_FATAL(popQueue->tail);
    CU_ASSERT_EQUAL_FATAL(testQueueContents(popQueue, test, 5), 1);
    destroy(popQueue);
}

void jqr_test_create_p_queue(void)
{
    // Check for empty queue
    PQUEUE_p_t emptyQueue = create_p_queue(NULL, 0);
    CU_ASSERT_PTR_NULL_FATAL(emptyQueue->head);
    p_destroy(emptyQueue);

    // Check for populated queue
    PQ_ITEM_t test[] = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    PQUEUE_p_t popQueue = create_p_queue(test, 5);
    CU_ASSERT_PTR_NOT_NULL_FATAL(popQueue->head);
    CU_ASSERT_EQUAL_FATAL(testPQueueContents(popQueue, test, 5), 1);
    p_destroy(popQueue);
}

void jqr_test_enqueue(void)
{
    int test[] = {1, 2, 3};
    QUEUE_p_t queue = create_queue(NULL, 0);
    enqueue(queue, 1);
    enqueue(queue, 2);
    enqueue(queue, 3);
    CU_ASSERT_EQUAL_FATAL(testQueueContents(queue, test, 3), 1);
    destroy(queue);
}

void jqr_test_p_enqueue(void)
{
    PQ_ITEM_t test[] = {{1, 1}, {3, 1}, {2, 2}};
    PQ_ITEM_t test2[] = {{0, 0}, {1, 1}, {3, 1}, {2, 2}};
    PQUEUE_p_t queue = create_p_queue(NULL, 0);
    PQ_ITEM_t zero = {0, 0};
    PQ_ITEM_t one = {1, 1};
    PQ_ITEM_t two = {2, 2};
    PQ_ITEM_t three = {3, 1};
    p_enqueue(queue, one);
    p_enqueue(queue, two);
    p_enqueue(queue, three);
    CU_ASSERT_EQUAL_FATAL(testPQueueContents(queue, test, 3), 1);
    p_enqueue(queue, zero);
    CU_ASSERT_EQUAL_FATAL(testPQueueContents(queue, test2, 4), 1);
    p_destroy(queue);
}

void jqr_test_dequeue(void)
{
    int test[] = {1, 2, 3};
    QUEUE_p_t queue = create_queue(test, 3);
    CU_ASSERT_EQUAL_FATAL(dequeue(queue), 1);
    CU_ASSERT_EQUAL_FATAL(dequeue(queue), 2);
    CU_ASSERT_EQUAL_FATAL(dequeue(queue), 3);
    CU_ASSERT_EQUAL_FATAL(dequeue(queue), -1);
    CU_ASSERT_PTR_NULL(queue->head);
    CU_ASSERT_PTR_NULL(queue->tail);
    destroy(queue);
}

void jqr_test_p_dequeue(void)
{
    PQ_ITEM_t test[] = {{1, 1}, {2, 2}, {3, 1}};
    PQUEUE_p_t queue = create_p_queue(test, 3);
    CU_ASSERT_EQUAL_FATAL(p_dequeue(queue), 1);
    CU_ASSERT_EQUAL_FATAL(p_dequeue(queue), 3);
    CU_ASSERT_EQUAL_FATAL(p_dequeue(queue), 2);
    CU_ASSERT_EQUAL_FATAL(p_dequeue(queue), -1);
    CU_ASSERT_PTR_NULL(queue->head);
    p_destroy(queue);
}

void jqr_test_peek(void)
{
    QUEUE_p_t emptyQueue = create_queue(NULL, 0);
    CU_ASSERT_EQUAL(peek(emptyQueue), -1);
    destroy(emptyQueue);

    int test[] = {1};
    QUEUE_p_t queue = create_queue(test, 1);
    CU_ASSERT_EQUAL(peek(queue), 1);
    CU_ASSERT_PTR_NOT_NULL(queue->head);
    CU_ASSERT_PTR_NOT_NULL(queue->tail);
    destroy(queue);
}

void jqr_test_p_peek(void)
{
    PQUEUE_p_t emptyQueue = create_p_queue(NULL, 0);
    CU_ASSERT_EQUAL(p_peek(emptyQueue), -1);
    p_destroy(emptyQueue);

    PQ_ITEM_t test[] = {{2, 1}};
    PQUEUE_p_t queue = create_p_queue(test, 1);
    CU_ASSERT_EQUAL(p_peek(queue), 2);
    CU_ASSERT_PTR_NOT_NULL(queue->head);
    p_destroy(queue);
}

static void _redirect_stdout()
{
    cur_stdout_fd = g_arrays_fd_stdout;
    fp_stdout_file = fopen(TEST_STDOUT_CAPTURE_FILE, "w+");

    if (NULL != fp_stdout_file)
    {
        g_arrays_fd_stdout = fileno(fp_stdout_file);
    }
}

static char *_capture_and_restore_stdout()
{
    char *p_output = NULL;
    g_arrays_fd_stdout = cur_stdout_fd;

    if (NULL != fp_stdout_file)
    {
        fseek(fp_stdout_file, 0L, SEEK_END);
        uint64_t file_size = ftell(fp_stdout_file);

        fseek(fp_stdout_file, 0, SEEK_SET);

        p_output = calloc(1, file_size + 1);
	if(NULL == p_output) { goto CALLOC_ERROR; }
        fread(p_output, file_size, 1, fp_stdout_file);

        fclose(fp_stdout_file);
    }
    fp_stdout_file = NULL;
    if (NULL != p_output)
    {
#ifdef SHOW_OUTPUT
        printf("\n======Captured Output========\n%s======End Captured Output====\n", p_output);
#endif
    }

    return p_output;

 CALLOC_ERROR:
    fprintf(stderr, "!!! Error in _capture_and_restore_stdout calloc\n");
    return NULL;
}

void jqr_test_print_queue(void)
{
    QUEUE_p_t emptyQueue = create_queue(NULL, 0);
    char *p_output = NULL;
    _redirect_stdout();
    print_queue(emptyQueue);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "Queue()\n");
    destroy(emptyQueue);
    free(p_output);

    int test[] = {1, 2, 3, 4, 5};
    QUEUE_p_t testQueue = create_queue(test, 5);
    _redirect_stdout();
    print_queue(testQueue);
    p_output = _capture_and_restore_stdout();
    CU_ASSERT_PTR_NOT_NULL_FATAL(p_output);
    CU_ASSERT_STRING_EQUAL(p_output, "Queue(1, 2, 3, 4, 5)\n");
    destroy(testQueue);
    free(p_output);
}

void jqr_test_clear(void)
{
    int test[] = {1, 2, 3, 4, 5, 5};
    QUEUE_p_t testQueue = create_queue(test, 6);
    CU_ASSERT_EQUAL(testQueueContents(testQueue, test, 6), 1);
    clear(testQueue);
    CU_ASSERT_PTR_NULL(testQueue->head);
    CU_ASSERT_PTR_NULL(testQueue->tail);
    free(testQueue);
}

void jqr_test_p_clear(void)
{
    PQ_ITEM_t test[] = {{1, 1}, {2, 2}, {3, 1}};
    PQ_ITEM_t result[] = {{1, 1}, {3, 1}, {2, 2}};
    PQUEUE_p_t testQueue = create_p_queue(test, 3);
    CU_ASSERT_EQUAL(testPQueueContents(testQueue, result, 3), 1);
    p_clear(testQueue);
    CU_ASSERT_PTR_NULL(testQueue->head);
    free(testQueue);
}

#endif
