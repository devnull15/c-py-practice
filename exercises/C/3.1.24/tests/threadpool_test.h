/**
 * @brief Test create new threadpool.
 * 
 */
void test_threadpool_new(void);

/**
 * @brief Test threadpool error on new.
 */
void test_threadpool_err_code(void);

/**
 * @brief Test queueing a job in the threadpool.
 * 
 */
void test_threadpool_queue_job(void);

/**
 * @brief Test queue job handles null pool.
 * 
 */
void test_threadpool_queue_job_null_pool(void);

/**
 * @brief Test queue job handles null job.
 * 
 */
void test_threadpool_queue_null_job(void);

/**
 * @brief Test limits of threadpool.
 * 
 */
void test_threadpool_limit(void);

/**
 * @brief Test shutdown threadpool.
 * 
 */
void test_threadpool_shutdown(void);
