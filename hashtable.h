#ifndef PIELANG_HASHTABLE_H
#define PIELANG_HASHTABLE_H

#include <stdlib.h>

typedef enum {
  HashTableTypeVariableMap = 1,
} HashTableType;


typedef struct HashTableEntry {
  char *key;
  void *literal;
  struct HashTableEntry *next;
} HashTableEntry;


typedef struct {
  HashTableType hash_table_type;
  size_t size;
  HashTableEntry **table;
} HashTable;


HashTable *new_hash_table(size_t size, HashTableType hash_table_type);


void hash_table_set(HashTable *hash_table, char *key, void *literal);


void *hash_table_get(HashTable *hash_table, char *key);


void free_hash_table(HashTable *hash_table);

#endif //PIELANG_HASHTABLE_H
