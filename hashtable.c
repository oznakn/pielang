#include "hashtable.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "value.h"

// see https://gist.github.com/tonious/1377667/104d94bb2f5b87a09dfd8cb181e302f19e46e1aa


unsigned long int hash(size_t size, char *key) {
  unsigned long int hash = 0;
  size_t i = 0;
  size_t length = strlen(key);

  while (hash < ULONG_MAX && i < length) {
    hash = hash * 2;
    hash = hash ^ key[i];
    ++i;
  }

  return hash % size;
}

HashTable *new_hash_table(size_t size, HashTableType hash_table_type) {
  HashTable *hash_table = malloc(sizeof(HashTable));
  hash_table->hash_table_type = hash_table_type;
  hash_table->size = size;
  hash_table->table = malloc(size * sizeof(void *));

  for (size_t i = 0; i < size; i++) {
    hash_table->table[i] = NULL;
  }

  return hash_table;
}

HashTableEntry *hash_table_new_entry(char *key, void *literal) {
  HashTableEntry *entry = malloc(sizeof(HashTableEntry));
  entry->key = key;
  entry->literal = literal;
  entry->next = NULL;

  return entry;
}

void hash_table_set(HashTable *hash_table, char *key, void *literal) {
  HashTableEntry *entry = NULL;
  HashTableEntry *next = NULL;
  HashTableEntry *last = NULL;

  unsigned long int hash_value = hash(hash_table->size, key);

  next = hash_table->table[hash_value];

  while (next != NULL && strcmp(key, next->key) > 0) {
    last = next;
    next = next->next;
  }

  if (next != NULL && strcmp(key, next->key) == 0) {
    switch (hash_table->hash_table_type) {
      case HashTableTypeVariableMap: {
        free_variable(next->literal);
        break;
      }

      default : {
        free(next->literal);
      }
    }

    next->literal = literal;
  } else {
    entry = hash_table_new_entry(key, literal);

    if (next == hash_table->table[hash_value]) {
      entry->next = next;
      hash_table->table[hash_value] = entry;
    } else if (next == NULL) {
      last->next = entry;
    } else {
      entry->next = next;
      last->next = entry;
    }
  }
}

void *hash_table_get(HashTable *hash_table, char *key) {
  unsigned long int hash_literal = hash(hash_table->size, key);

  HashTableEntry *next = hash_table->table[hash_literal];

  if (next != NULL) {
    HashTableEntry *last;

    while (next != NULL && strcmp(key, next->key) > 0) {
      last = next;
      next = last->next;
    }

    if (next != NULL && strcmp(key, next->key) == 0) {
      return next->literal;
    }
  }

  return NULL;
}

void free_hash_table_entry(HashTable *hash_table, HashTableEntry *hash_table_entry) {
  if (hash_table_entry->next != NULL) {
    free_hash_table_entry(hash_table, hash_table_entry->next);
  }

  switch (hash_table->hash_table_type) {
    case HashTableTypeVariableMap: {
      free_variable((Variable *)hash_table_entry->literal); // TODO
      break;
    }

    default: {
      free(hash_table_entry->literal);
    }
  }

  free(hash_table_entry);
}

void free_hash_table(HashTable *hash_table) {
  for (size_t i = 0; i < hash_table->size; i += 1u) {
    free_hash_table_entry(hash_table, hash_table->table[i]);
  }

  free(hash_table);
}
