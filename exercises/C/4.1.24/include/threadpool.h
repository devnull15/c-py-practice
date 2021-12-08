#ifndef JQR_3124_TP_H
#define JQR_3124_TP_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief typedef for work performed by worker thread
 * 
 * @param[in] should_shutdown Long running workers should check thi
 * periodically and exit if it is set
 * @param[in] arg Argument to function
 * 
 */
typedef void (*job_f)(_Atomic bool* should_shutdown, void* arg);

// TODO You should define the the threadpool struct here or in threadpool.c

// Typedef for the threadpool struct
typedef struct threadpool_t threadpool_t;
typedef struct ll_t ll_t;
typedef struct node_t node_t;


/**
 * @brief Creates a new threadpool
 *
 * On error set err_code = 1
 * 
 * @param[in] thread_count Max number of threads for this pool
 * @param[in] err_code Error code on error
 * @return threadpool_t* A new threadpool context of NULL on error and err_code
 * is set
 */
threadpool_t* threadpool_new(size_t thread_count, int* err_code);

/**
 * @brief Adds a new job to the threadpool. Jobs are completed first come first
 * serve.
 * 
 * @param[in] pool A threadpool context
 * @param[in] job A job function
 * @param[in] arg Data passed to the function. This data should be allocated on
 * the heap, unless you know what you're doing. This data should be free()'ed by
 * the job function.
 * @return int 0 on success
 */
int threadpool_queue_job(threadpool_t* pool, job_f job, void* arg);

/**
 * @brief Sets the shutdown flag and processes the remainder of the queues. This
 * is so the worker threads that are responsible for cleaning up resources are
 * afforded the opportunity.
 * 
 * @param[in] pool A threadpool context
 * @return int 0 on success
 */
int threadpool_shutdown(threadpool_t* pool);

/**
 * @brief Waits for all queued jobs to finish and then kills the threadpool.
 * 
 * @param pool A threadpool context.
 * @return int 0 on success
 */
int threadpool_delete(threadpool_t* pool);

#endif
