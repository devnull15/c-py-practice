#include "../include/threadpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>


#define THREADS 0



/** Structs **/

typedef struct job_t{
  job_f jobdef;
  void *jobarg;
}job_t;

typedef struct node_t{
  node_t *next;
  job_t *job;
}node_t;

typedef struct jobsem_t {
  _Atomic int num_jobs;
  pthread_cond_t has_jobs;
  pthread_cond_t no_jobs;
  pthread_mutex_t jobsemlock;
}jobsem_t;

typedef struct ll_t{
  node_t *top;
  node_t *end;
  pthread_mutex_t jq_lock;
  jobsem_t *jobsem;
}ll_t;

typedef struct threadpool_t{
  pthread_t *threads;
  ll_t *jq;
  _Atomic bool shutdown;
  _Atomic bool keepalive;
  _Atomic int num_threads;
} threadpool_t;


/** /Structs **/


/** My helper functions **/

/* semaphore.h doesn't have support for broadcasts so implementing my own. */
void _jobsem_init(jobsem_t *sem) {
  pthread_cond_init(&(sem->has_jobs),NULL);
  pthread_cond_init(&(sem->no_jobs),NULL);
  pthread_mutex_init(&(sem->jobsemlock), NULL);
  sem->num_jobs = 0;
}

void _jobsem_wait_threads(jobsem_t *sem) {
  pthread_mutex_lock(&(sem->jobsemlock));
  while(!sem->num_jobs) { pthread_cond_wait(&sem->has_jobs,&sem->jobsemlock); }
  if(!sem->num_jobs) { pthread_cond_signal(&sem->no_jobs); }
  else { sem->num_jobs--; }
  pthread_mutex_unlock(&(sem->jobsemlock));
}

void _jobsem_wait_delete(jobsem_t *sem) {
  pthread_mutex_lock(&(sem->jobsemlock));
  while(sem->num_jobs) { pthread_cond_wait(&sem->no_jobs,&sem->jobsemlock); }
  pthread_mutex_unlock(&(sem->jobsemlock));
}


void _jobsem_signal(jobsem_t *sem) {
  pthread_mutex_lock(&(sem->jobsemlock));
  sem->num_jobs++;
  pthread_cond_signal(&sem->has_jobs);
  pthread_mutex_unlock(&(sem->jobsemlock));
  
}

void _jobsem_broadcast(jobsem_t *sem) {
  pthread_mutex_lock(&(sem->jobsemlock));
  sem->num_jobs = -1; // signal to quit
  pthread_cond_broadcast(&sem->has_jobs);
  pthread_mutex_unlock(&(sem->jobsemlock));
}



job_t*  _ll_pop(ll_t *jq) {
  if(jq == NULL) {
    fprintf(stderr, "\n!! _ll_pop jobqueue not alloc'd\n");
    return NULL;
  }
 
  if(jq->top == NULL) {
    fprintf(stderr, "\n!! _ll_pop jobqueue empty\n");
    return NULL;
  }
 

  if(jq->top->job == NULL) {
    fprintf(stderr, "\n!! _ll_pop no job provided\n");
    return NULL;
  }


  pthread_mutex_lock(&(jq->jq_lock));
  
  
  node_t *done = jq->top;
  job_t *ret = done->job;
  jq->top = jq->top->next;
  free(done);
  pthread_mutex_unlock(&(jq->jq_lock));
  
   
  return ret;
}

void _ll_push(ll_t *jq, node_t *n) {

  if(jq == NULL) {
    fprintf(stderr, "\n!! _ll_push jobqueue empty\n");
    return;
  }

  pthread_mutex_lock(&(jq->jq_lock));
  
  if(jq->top != NULL) { jq->end->next = n;  }
  else { jq->top = n; } //jq is empy in this case

  jq->end = n;
  _jobsem_signal(jq->jobsem);
  pthread_mutex_unlock(&(jq->jq_lock));

  
  return;
}

node_t* _ll_makenode(ll_t *jq, job_f j, void *arg) {
  if(jq == NULL) {
    fprintf(stderr, "\n!! _ll_makenode jobqueue empty\n");
    return NULL;
  }

  if(j == NULL) {
    fprintf(stderr, "\n!! _ll_makenode no job provided\n");
    return NULL;
  }


  node_t *n = malloc(sizeof(node_t));
  n->job = malloc(sizeof(job_t));
  n->job->jobdef = j;
  n->job->jobarg = arg;
  n->next = NULL;
  return n;
}

static void* _thread_exec(threadpool_t *pool) {

#ifndef NDEBUG 
  fprintf(stderr, "\n ** _thread_exec **\n"); //DELETE
#endif //NDEBUG
  
  
  if(pool == NULL || pool->jq == NULL) {
    fprintf(stderr, "\n!! _thread_exec: pool or jq is NULL\n");
    return NULL;
  }
    
  while(pool->keepalive) {
  
    _jobsem_wait_threads(pool->jq->jobsem);
        
    if(!(pool->keepalive)&& !(pool->jq->jobsem->num_jobs) ) { break; } // free blocked threads after signal to destroy and there are no jobs
	    
    job_t *job = _ll_pop(pool->jq);
    if(job != NULL) {
      (job->jobdef)(&(pool->shutdown), job->jobarg);
      free(job);
    }
      
  }

  pool->num_threads--;
  
#ifndef NDEBUG
    fprintf(stderr, "\n ** _thread_exec done **\n");
#endif //NDEBUG
  

  return NULL;
}

/** /My helper functions **/

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
threadpool_t* threadpool_new(size_t thread_count, int* err_code) {

  // handle thread count <= 0 or NULL err_code
  if(thread_count<=0 || err_code == NULL) { 
    fprintf(stderr, "\n!! threadpool_new: NULL err_code or <=0 thread_count\n");	   
    goto ERR;
  }

  //make threadpool struct
  threadpool_t *pool = NULL;
  if((pool = (threadpool_t *)malloc(sizeof(struct threadpool_t))) == NULL) {
    fprintf(stderr, "\n!! threadpool_new: couln't malloc pool\n");
    goto ERR;
  }

  //make jobqueue
  if((pool->jq = malloc(sizeof(ll_t))) == NULL) {
    fprintf(stderr, "\n!! threadpool_new: couln't malloc jq\n");	
    goto ERR;
  }

  //make jobsem
  if((pool->jq->jobsem = malloc(sizeof(jobsem_t))) == NULL) {
    fprintf(stderr, "\n!! threadpool_new: couln't malloc jobsem\n");	
    goto ERR;
  }


  //make thread array
  if((pool->threads = (pthread_t *)malloc(thread_count*sizeof(pthread_t))) == NULL) {
    fprintf(stderr, "\n!! threadpool_new: couldn't malloc threads\n");	
    goto ERR;
  }

  
  pool->num_threads = thread_count;
  pthread_mutex_init(&(pool->jq->jq_lock),NULL);
  _jobsem_init(pool->jq->jobsem);
  pool->jq->top = NULL;
  pool->jq->end = NULL;
  pool->shutdown = 0;
  pool->keepalive = 1;

  
  //start threads
  for(size_t i = 0; i < thread_count; i++) {
#ifndef NDEBUG
    fprintf(stderr, "\n ** starting thread %lu **\n", i);
#endif //NDEBUG
    
    pthread_create(&(pool->threads[i]), NULL, (void * (*)(void *)) _thread_exec, pool);
  }

		   
  return pool;

 ERR:
  if(pool != NULL) {
    if(pool->jq != NULL) {
      if(pool->jq->jobsem != NULL) { free(pool->jq->jobsem); }
      free(pool->jq);
    }
    if(pool->threads != NULL) { free(pool->threads); }
    free(pool);
  } 
  *err_code = 1;
  return NULL;
 
}

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
int threadpool_queue_job(threadpool_t* pool, job_f job, void* arg) {

#ifndef NDEBUG
  fprintf(stderr, "\n ** queue_job **\n");
#endif //NDEBUG
  
  if(pool == NULL) {
    fprintf(stderr, "\n!! threadpool_queue_job: pool is NULL\n");
    return 1;
  }
  
  if(job == NULL) {
    fprintf(stderr, "\n!! threadpool_queue_job: job is NULL\n");
    return 1;
  }

  node_t *n =  _ll_makenode(pool->jq, job, arg);
  _ll_push(pool->jq, n);
  
  return 0;
}

/**
 * @brief Sets the shutdown flag and processes the remainder of the queues. This
 * is so the worker threads that are responsible for cleaning up resources are
 * afforded the opportunity.
 * 
 * @param[in] pool A threadpool context
 * @return int 0 on success
 */
int threadpool_shutdown(threadpool_t* pool) {
#ifndef NDEBUG
  fprintf(stderr, "\n ** shutdown **\n");
#endif //NDEBUG

  pool->shutdown = 1;
  return 0;
}

/**
 * @brief Waits for all queued jobs to finish and then kills the threadpool.
 * 
 * @param pool A threadpool context.
 * @return int 0 on success
 */
int threadpool_delete(threadpool_t* pool) {
#ifndef NDEBUG
  fprintf(stderr, "\n ** delete **\n");
#endif //NDEBUG


  if(pool==NULL) { return 0; }

  sleep(1); //give time for jobs to populate
  
  
  if(pool->jq->jobsem->num_jobs) { _jobsem_wait_delete(pool->jq->jobsem); }
  
  
  pool->keepalive = 0;
  
  while(pool->num_threads) { _jobsem_broadcast(pool->jq->jobsem); }    
    
  for(int i = 0; i < pool->num_threads; i++) {
#ifndef NDEBUG
    fprintf(stderr, "\n ** join thread %i **\n", i);
#endif //NDEBUG
    pthread_join(pool->threads[i],NULL);  
  }  

  free(pool->jq->jobsem);
  free(pool->jq);
  free(pool->threads);
  free(pool);
    
  return 0;
}
