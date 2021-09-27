#ifndef JQR_3124_TP_TEST_H
#define JQR_3124_TP_TEST_H

#define _GNU_SOURCE

#include "../include/threadpool.h"
#include "../libmalloc/libmalloc.h"

#include <unistd.h>
#include <CUnit/CUnitCI.h>
#include <CUnit/CUnit.h>
#include <pthread.h>

#define DEFAULT_THREADPOOL_SIZE 10
#define DEFAULT_SLEEP_DURATION_MS 500
#define USEC_PER_MS 1000
#define DEFAULT_SLEEP_DURATION_USEC (DEFAULT_SLEEP_DURATION_MS * USEC_PER_MS)

int TEST_FAIL = 0;

/**
 * @brief Will be passed as arguments to job_f.
 * 
 */
typedef struct inc_parms_t
{
    _Atomic int entry_counter;
    _Atomic int counter;
    _Atomic bool flag;
 } inc_parms_t;

/**
 * @brief job_f for testing.
 */
static void increment_int_wait(_Atomic bool* should_shutdown, inc_parms_t* args)
{
    args->entry_counter++;
    // This is only marginally better than a spinlock.
    // do not use this implementation in your code
    while (! (*should_shutdown))
    {
        if (args->flag)
        {
            args->counter++;
            break;
        }
        pthread_yield();
    }
}

/**
 * @brief job_f for testing
 */
static void increment_int(_Atomic bool* should_shutdown, _Atomic int* arg)
{
    (*arg)++;
}

void test_threadpool_new(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_new);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_delete);

    int errcode;

    threadpool_t* pool = threadpool_new(DEFAULT_THREADPOOL_SIZE, &errcode);
    CU_ASSERT_PTR_NOT_NULL_FATAL(pool);
    CU_ASSERT_EQUAL(threadpool_delete(pool), 0);
}

void test_threadpool_err_code(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_new);

    TEST_FAIL = 1;
    int errcode = 0;
    threadpool_t *pool = threadpool_new(DEFAULT_THREADPOOL_SIZE, &errcode);
    TEST_FAIL = 0;

    CU_ASSERT_PTR_NULL(pool);
    CU_ASSERT_EQUAL(errcode, 1);
}

void test_threadpool_queue_job(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_new);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_queue_job);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_delete);

    int errcode;
    threadpool_t* pool = threadpool_new(DEFAULT_THREADPOOL_SIZE, &errcode);
    CU_ASSERT_PTR_NOT_NULL_FATAL(pool);
    _Atomic int counter = 0;
    for (size_t i = 0; i < DEFAULT_THREADPOOL_SIZE; i++)
    {
        CU_ASSERT_EQUAL_FATAL(threadpool_queue_job(pool, (job_f)increment_int, &counter), 0);
    }
    usleep(DEFAULT_SLEEP_DURATION_USEC);
    CU_ASSERT_EQUAL_FATAL(counter, DEFAULT_THREADPOOL_SIZE);
    CU_ASSERT_EQUAL(threadpool_delete(pool), 0);
}

void test_threadpool_queue_job_null_pool(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_queue_job);
    _Atomic int counter = 0;
    int res = threadpool_queue_job(NULL, (job_f) increment_int, &counter);
    CU_ASSERT_NOT_EQUAL(res, 0);
}

void test_threadpool_queue_null_job(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_new);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_queue_job);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_delete);

    int err_code = 0;
    threadpool_t* pool = threadpool_new(DEFAULT_THREADPOOL_SIZE, &err_code);
    CU_ASSERT_PTR_NOT_NULL_FATAL(pool);
    int res = threadpool_queue_job(pool, NULL, NULL);
    CU_ASSERT_NOT_EQUAL_FATAL(res, 0);

    CU_ASSERT_EQUAL(threadpool_delete(pool), 0);
}

void test_threadpool_limit(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_new);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_queue_job);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_delete);

    int errcode;
    threadpool_t* pool = threadpool_new(DEFAULT_THREADPOOL_SIZE, &errcode);
    CU_ASSERT_PTR_NOT_NULL_FATAL(pool);
    inc_parms_t args = {
        .flag = false,
        .counter = 0
    };
    for (size_t i = 0; i < DEFAULT_THREADPOOL_SIZE; i++)
    {
        CU_ASSERT_EQUAL_FATAL(threadpool_queue_job(pool, (job_f) increment_int_wait, &args), 0);
    }
    usleep(DEFAULT_SLEEP_DURATION_USEC);

    // threadpool should be full from above loop b/c workers have made no progress
    CU_ASSERT_EQUAL_FATAL(threadpool_queue_job(pool, (job_f) increment_int_wait, &args), 0);
    usleep(DEFAULT_SLEEP_DURATION_USEC);
    CU_ASSERT_EQUAL_FATAL(args.entry_counter, DEFAULT_THREADPOOL_SIZE);
    CU_ASSERT_EQUAL_FATAL(args.counter, 0);
    args.flag = true;
    usleep(DEFAULT_SLEEP_DURATION_USEC);
    CU_ASSERT_EQUAL_FATAL(args.counter, DEFAULT_THREADPOOL_SIZE + 1);
    CU_ASSERT_EQUAL(threadpool_delete(pool), 0);
}

void test_threadpool_shutdown(void)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_new);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_queue_job);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_shutdown);
    CU_ASSERT_PTR_NOT_NULL_FATAL(threadpool_delete);

    int errcode;
    threadpool_t* pool = threadpool_new(DEFAULT_THREADPOOL_SIZE, &errcode);
    CU_ASSERT_PTR_NOT_NULL_FATAL(pool);
    inc_parms_t args = {
        .flag = false,
        .counter = 0
    };
    for (size_t i = 0; i < DEFAULT_THREADPOOL_SIZE; i++)
    {
        CU_ASSERT_EQUAL_FATAL(threadpool_queue_job(pool, (job_f)increment_int_wait, &args), 0);
    }
    usleep(DEFAULT_SLEEP_DURATION_USEC);

    // threadpool should be full from above loop b/c workers have made no progress
    CU_ASSERT_EQUAL_FATAL(threadpool_queue_job(pool, (job_f)increment_int_wait, &args), 0);
    usleep(DEFAULT_SLEEP_DURATION_USEC);
    CU_ASSERT_EQUAL_FATAL(args.entry_counter, DEFAULT_THREADPOOL_SIZE);
    CU_ASSERT_EQUAL_FATAL(args.counter, 0);
    CU_ASSERT_EQUAL_FATAL(threadpool_shutdown(pool), 0);
    usleep(DEFAULT_SLEEP_DURATION_USEC);
    CU_ASSERT_EQUAL_FATAL(args.entry_counter, DEFAULT_THREADPOOL_SIZE + 1);
    CU_ASSERT_EQUAL_FATAL(args.counter, 0);
    CU_ASSERT_EQUAL(threadpool_delete(pool), 0);
}

#endif
