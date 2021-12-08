/**
 * @brief tests create_hashtable()
 */
void test_create(void);

/**
 * @brief tests create with null hash function
 */
void test_create_null_func(void);

/**
 * @brief tests insert_hashtable() with null table
 */ 
void test_insert_null_table(void);

/**
 * @brief tests insert_hashtable()
 */
void test_insert(void);

/**
 * @brief tests that values change when same key inserted multiple times
 */
void test_insert_same(void);

/**
 * @brief tests insert_hashtable() with large number of values.
 */
void test_insert_realloc(void);

/**
 * @brief tests delete_from_hashtable() with null table
 */
void test_delete_null_table(void);

/**
 * @brief tests delete_from_hashtable() when the key does not exist
 */
void test_delete_key_dne(void);

/**
 * @brief tests delete_from_hashtable()
 */
void test_delete(void);

/**
 * @brief tests get_from_hahtable() when the table is null
 */
void test_get_null_table(void);

/**
 * @brief tests get_from_hashtable() when the item does not exist
 */
void test_get_item_dne(void);

/**
 * @brief tests get_from_hashtable()
 */
void test_get(void);

/**
 * @brief tests empty_hashtable() with null table
 */
void test_empty_null(void);

/**
 * @brief tests empty_hashtable()
 */
void test_empty(void);

/**
 * @brief tests destroy_hashtable() with null table
 */
void test_destroy_null(void);

/**
 * @brief tests destroy_hashtable()
 */
void test_destroy(void);
