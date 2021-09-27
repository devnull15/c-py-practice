#include "../include/hash_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//stolen from hash_table_test.c, not sure why this isn't in a header somewhere...
#define HASH_KEY_SIZE 20
#define INIT_NUM_BUCKETS HASH_KEY_SIZE
#define INIT_BUCKET_SIZE HASH_KEY_SIZE

/**
 * @brief Create a new empty hash table
 * @param hash_func The hash function that will be used
 */
hash_table_t *create_hashtable(hash_func hash_f) {
  if(hash_f == NULL) { return NULL; }
  
  hash_table_t *t = calloc(1, sizeof(hash_table_t));
  t->num_buckets = INIT_NUM_BUCKETS;
  t->bucket_sizes = calloc(t->num_buckets, sizeof(int));
  t->items = calloc(t->num_buckets, sizeof(data_item_t**));
  for(int i = 0; i < t->num_buckets; i++) {
    t->bucket_sizes[i] = INIT_BUCKET_SIZE;
    t->items[i] = calloc(t->bucket_sizes[i], sizeof(data_item_t*));
  }

  t->hash_f = hash_f;
  
  return t;
}

/**
 * @brief Insert a new item into the hash table.
 * 
 * @param table The table to insert into
 * @param key The new key
 * @param data The new data
 * 
 * @return 0 on success
 */
void _realloc_bucket(hash_table_t *table, int hash) {

  int oldsize = table->bucket_sizes[hash];
  table->bucket_sizes[hash] *= 2;
  data_item_t **bucket = calloc(table->bucket_sizes[hash], sizeof(data_item_t*));
  memcpy(bucket, table->items[hash], oldsize*sizeof(data_item_t));
  free(table->items[hash]); //technically a bad practice, but not sure of a better way to do it.
  table->items[hash] = bucket;

  return;
}


int insert_hashtable(hash_table_t *table, int key, int data) {
  if(table == NULL) { return -1; }
  
  int hash = (*table->hash_f)(key);
  int i = 0;
  

  while(table->items[hash][i] != NULL &&
	i < table->bucket_sizes[hash]) {
    i++;
  }
  
  if(i == table->bucket_sizes[hash]) { _realloc_bucket(table, hash);  }
  
  data_item_t *item = get_from_hashtable(table,key);
  if(item == NULL) {
    item = calloc(1, sizeof(data_item_t));
    item->key = key;
    item->data = data;
    table->items[hash][i] = item;
  }
  else{ item->data = data; }
  
  return 0;
}

/**
 * @brief Remove an item from the hash table.
 * 
 * @param table The table to delete from
 * @param key The key to remove
 * 
 * @return <0 on error, 0 on success, >0 if key not in hash table
 */
int delete_from_hashtable(hash_table_t *table, int key) {
 if(table == NULL) { return -1; }

  int hash = (*table->hash_f)(key);
  int i = 0;
  
  while(i < table->bucket_sizes[hash]) {
    if(table->items[hash][i] != NULL) {
      if(table->items[hash][i]->key == key) { break; }
    }
    i++;
  }

  if(i >= table->bucket_sizes[hash]) { return 1; }
  else {
    free(table->items[hash][i]); //technically a bad practice, but not sure of a better way to do it.
    table->items[hash][i] = NULL;
  }
    
  return 0;
}

/**
 * @brief Retrieve an item from the hash table.
 * 
 * @param table The table to get the item from
 * @param key The key to retrieve
 * 
 * @return Pointer to the requested item or NULL
 */
data_item_t *get_from_hashtable(hash_table_t *table, int key) {
  if(table == NULL) { return NULL; }

  int hash = (*table->hash_f)(key);
  int i = 0;
  
  while(i < table->bucket_sizes[hash]) {
    if(table->items[hash][i] != NULL) {
      if(table->items[hash][i]->key == key) { break; }
    }
    i++;
  }
  if(i >= table->bucket_sizes[hash]) { return NULL; }
  
  return table->items[hash][i];
}

/**
 * @brief Remove all items from the hash table.
 * 
 * @param table The table to empty.
 * 
 * @return 0 on success
 */
int empty_hashtable(hash_table_t *table) {
  if(table == NULL) { return -1; }
  
  for(int i = 0; i < table->num_buckets; i++) {
    for(int j = 0; j < table->bucket_sizes[i]; j++) {
      free(table->items[i][j]); //technically a bad practice, but not sure of a better way to do it.
      table->items[i][j] = NULL;
    }
  }
  
  return 0;
}

/**
 * @brief Destroy the hash table.
 * 
 * @param table The table to destroy
 * 
 * @return 0 on success
 */
int destroy_hashtable(hash_table_t *t) {
  if(t == NULL) { return -1; }

  for(int i = 0; i < t->num_buckets; i++) {
    for(int j = 0; j < t->bucket_sizes[i]; j++) {
      free(t->items[i][j]);
    }
    free(t->items[i]);
  }
  free(t->items);
  free(t->bucket_sizes);
  free(t);

  t = NULL;
  
  return 0;
}
