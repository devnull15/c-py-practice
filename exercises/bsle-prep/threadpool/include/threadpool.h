// ref: https://github.com/Pithikos/C-Thread-Pool/blob/master/thpool.h

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <stdint.h>


typedef struct threadpool_ threadpool;

/**
 * @brief initilizes the thread pool
 *
 * @param num_threads - number of threads for the pool
 *
 * @return pointer to intilized thread pool or NULL on error
 *
 */
threadpool* thpool_init(int num_threads);


/**
 * @brief add jobs to the thread pool
 *
 * @param pool - pointer to thread pool
 *
 * @param jobdef - pointer to the function definition of the job being added
 *
 * @param args - pointer to args for the job function
 *
 * @return 0 on success; nonzero on error
 *
 */
int thpool_add_job(threadpool* pool, void (*jobdef)(void *), void* args);


/**
 * @brief stops execution of the thread pool; 
 *        frees resources and join threads
 *
 * @param pool - thread pool to be stopped and freed
 *
 * @return 0 on success; nonzero on error
 *
 */
int thpool_destroy(threadpool* pool);

#endif /* _THREADPOOL_H */
