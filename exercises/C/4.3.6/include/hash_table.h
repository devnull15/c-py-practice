#ifndef JQR_338_HASH_TABLE_H
#define JQR_338_HASH_TABLE_H

/**
 * @brief The basic node that will be stored in the hash table.
 *
 * We're going to be sticking with integers just because it's easy to write
 * a hash function. Given that 3.1.13 already covered writing a hash function
 * it seems to make sense on the actual data structure here.
 */
typedef struct _data_item_t {
    int data; ///< The data stored 
    int key; ///< the key
} data_item_t;

/**
 * @brief a function pointer to a custom-defined hash function.
 * takes an integer key and returns an integer hash
 */
typedef int (*hash_func)(int key);

/**
 * @brief The actual hash table itself.
 * 
 * The new value replaces the old value if inserting the same key multiple times
 *
 * Your design must include some way of mitigating hash collisions. The fields 
 * defined in hash_table_t should provide a hint on how to do so.
 */
typedef struct _hash_table_t {
    data_item_t ***items; ///< Buckets and their corresponding items
    int *bucket_sizes;  ///< Size of each of the buckets
    hash_func hash_f; ///< Hash function 
    int num_buckets; ///< Might be useful to keep track of how many buckets you
                     ///< have
} hash_table_t;

/**
 * @brief Create a new empty hash table
 * @param hash_func The hash function that will be used
 */
hash_table_t *create_hashtable(hash_func hash_f);

/**
 * @brief Insert a new item into the hash table.
 * 
 * @param table The table to insert into
 * @param key The new key
 * @param data The new data
 * 
 * @return 0 on success
 */
int insert_hashtable(hash_table_t *table, int key, int data);

/**
 * @brief Remove an item from the hash table.
 * 
 * @param table The table to delete from
 * @param key The key to remove
 * 
 * @return <0 on error, 0 on success, >0 if key not in hash table
 */
int delete_from_hashtable(hash_table_t *table, int key);

/**
 * @brief Retrieve an item from the hash table.
 * 
 * @param table The table to get the item from
 * @param key The key to retrieve
 * 
 * @return Pointer to the requested item or NULL
 */
data_item_t *get_from_hashtable(hash_table_t *table, int key);

/**
 * @brief Remove all items from the hash table.
 * 
 * @param table The table to empty.
 * 
 * @return 0 on success
 */
int empty_hashtable(hash_table_t *table);

/**
 * @brief Destroy the hash table.
 * 
 * @param table The table to destroy
 * 
 * @return 0 on success
 */
int destroy_hashtable(hash_table_t *table);

#endif
