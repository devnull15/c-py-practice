/*
    Reference the CUnit Users Guide:
    http://cunit.sourceforge.net/doc/writing_tests.html#tests

    Step 1. Create the test cases
    Step 2. _add the test suite to the registry
    Step 3. _add the test to the suite.
*/

/**
 * @brief tests create_queue() as an individual function. 
 * 
 */
void jqr_test_create_queue(void);

/**
 * @brief tests create_p_queue() as an individual function. 
 * 
 */
void jqr_test_create_p_queue(void);

/**
 * @brief tests enqueue() as an individual function. 
 * 
 */
void jqr_test_enqueue(void);

/**
 * @brief tests p_enqueue() as an individual function. 
 * 
 */
void jqr_test_p_enqueue(void);

/**
 * @brief tests dequeue() as an individual function. 
 * 
 */
void jqr_test_dequeue(void);

/**
 * @brief tests p_dequeue() as an individual function. 
 * 
 */
void jqr_test_p_dequeue(void);

/**
 * @brief tests peek() as an individual function. 
 * 
 */
void jqr_test_peek(void);

/**
 * @brief tests p_peek() as an individual function. 
 * 
 */
void jqr_test_p_peek(void);

/**
 * @brief tests print_queue() as an individual function
 * 
 */

void jqr_test_print_queue(void);

/**
 * @brief tests clear() as an individual function
 * 
 */
void jqr_test_clear(void);

/**
 * @brief tests p_clear() as an individual function
 * 
 */
void jqr_test_p_clear(void);