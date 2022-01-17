#include "CUnit/Basic.h"

#include "../include/hash_table.h"

// Arbitrarily chosen value for number of buckets and initial size of bucket
const int HASH_KEY_SIZE = 20;

/**
 * @brief Calculate the hash for a key
 *
 * @param key The key to hash
 */
int
hash_key(int key)
{
    return key % HASH_KEY_SIZE;
}

void test_create(void) {
    hash_table_t *new_table = create_hashtable(hash_key);    
    CU_ASSERT_PTR_NOT_NULL(new_table);
    destroy_hashtable(new_table);
}

void test_create_null_func(void)
{
  hash_table_t *new_table = create_hashtable(NULL);
    CU_ASSERT_PTR_NULL(new_table);
}

void test_insert_null_table(void) {
  int retval = insert_hashtable(NULL, 1, 1);
    CU_ASSERT_EQUAL(retval, -1);
}

void test_insert(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int retval = insert_hashtable(table, 1, 2);
    CU_ASSERT_EQUAL(retval, 0);

    data_item_t *ret = table->items[1][0];
    CU_ASSERT_PTR_NOT_NULL(ret);
    CU_ASSERT_EQUAL(ret->key, 1);
    CU_ASSERT_EQUAL(ret->data, 2);
    destroy_hashtable(table);
}

void test_insert_same(void)
{
    hash_table_t *table;
    data_item_t *ret;
    int rv;
    int i;
    int key = 10;
    
    table = create_hashtable(hash_key);
    for (i = 0; i < 20; ++i)
    {
        rv = insert_hashtable(table, key, i);
        CU_ASSERT_EQUAL(rv, 0);
        ret = get_from_hashtable(table, key);
        CU_ASSERT_EQUAL(ret->data, i);
    }
    destroy_hashtable(table);
}

void test_insert_realloc(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int num_insert = 20000;
    int i;
    int retval;
    for (i = 0; i < num_insert; i++) {
        retval = insert_hashtable(table, i, i);
        CU_ASSERT_EQUAL(retval, 0);
    }

    data_item_t *ret;
    for (i = 0; i < num_insert; i++) {
        ret = get_from_hashtable(table, i);
        CU_ASSERT_EQUAL(ret->key, i);
        CU_ASSERT_EQUAL(ret->data, i);
    }
    destroy_hashtable(table);
}

void test_delete_null_table(void) {
    int retval = delete_from_hashtable(NULL, 1);
    CU_ASSERT(retval < 0);
}

void test_delete_key_dne(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int retval = delete_from_hashtable(table, 0);
    CU_ASSERT(retval > 0);
    destroy_hashtable(table);
}

void test_delete(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int retval = insert_hashtable(table, 1, 2);
    CU_ASSERT_EQUAL(retval, 0);
    retval = delete_from_hashtable(table, 1);
    CU_ASSERT_EQUAL(retval, 0);
    destroy_hashtable(table);
}

void test_get_null_table(void) {
    data_item_t *item = get_from_hashtable(NULL, 1);
    CU_ASSERT_PTR_NULL(item);
}

void test_get_item_dne(void) {
    hash_table_t *table = create_hashtable(hash_key);
    data_item_t *item = get_from_hashtable(table, 1);
    CU_ASSERT_PTR_NULL(item);
    destroy_hashtable(table);
}

void test_get(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int retval;
    retval = insert_hashtable(table, 1, 2);
    CU_ASSERT_EQUAL(retval, 0);
    retval = insert_hashtable(table, 3, 4);
    CU_ASSERT_EQUAL(retval, 0);

    data_item_t *item;
    item = get_from_hashtable(table, 1);
    CU_ASSERT_PTR_NOT_NULL(item);
    CU_ASSERT_EQUAL(item->key, 1);
    CU_ASSERT_EQUAL(item->data, 2);

    item = get_from_hashtable(table, 3);
    CU_ASSERT_PTR_NOT_NULL(item);
    CU_ASSERT_EQUAL(item->key, 3);
    CU_ASSERT_EQUAL(item->data, 4);
    destroy_hashtable(table);
}

void test_empty_null(void) {
    int retval = empty_hashtable(NULL);
    CU_ASSERT_EQUAL(retval, -1);
}

void test_empty(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int retval = empty_hashtable(table);
    CU_ASSERT_EQUAL(retval, 0);
    destroy_hashtable(table);
}

void test_destroy_null(void) {
    int retval = destroy_hashtable(NULL);
    CU_ASSERT_EQUAL(retval, -1);
}

void test_destroy(void) {
    hash_table_t *table = create_hashtable(hash_key);
    int retval = destroy_hashtable(table);
    CU_ASSERT_EQUAL(retval, 0);
}
