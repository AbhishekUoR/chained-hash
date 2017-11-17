#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash.h"

// The initial capacity of the hash table array.
#define INITIAL_CAPACITY 16

// Threshold on load factor over which we double the capacity of the table.
#define LOAD_FACTOR_THR 5

/*
 * This structure is used to represent key/value pairs in the hash table.  We
 * also store a link to the next link in a chain directly in the association,
 * making it double as a linked list node.
 */
struct association {
  char* key;
  void* value;
  struct association* next;
};


struct hash {
  struct association** table;
  unsigned int capacity;
  unsigned int num_elems;
};

/*
 * Helper function to initialize a hash table's table array to a given capacity.
 */
void _hash_table_init(struct hash* hash, unsigned int capacity) {
  hash->table = malloc(capacity * sizeof(struct association*));
  assert(hash->table);
  memset(hash->table, 0, capacity * sizeof(struct association*));
  hash->capacity = capacity;
  hash->num_elems = 0;
}


struct hash* hash_create() {
  struct hash* hash = malloc(sizeof(struct hash));
  assert(hash);
  _hash_table_init(hash, INITIAL_CAPACITY);
  return hash;
}


void hash_free(struct hash* hash) {
  assert(hash);

  /*
   * Free all of the association structs stored in the table.
   */
  for (int i = 0; i < hash->capacity; i++) {
    struct association* next, * cur = hash->table[i];
    while (cur != NULL) {
      next = cur->next;
      free(cur);
      cur = next;
    }
  }

  free(hash->table);
  free(hash);
}


float hash_load_factor(struct hash* hash) {
  return hash->num_elems / (float)hash->capacity;
}


/*
 * The DJB hash function: http://www.cse.yorku.ca/~oz/hash.html.
 */
unsigned int _djb_hash(char* key) {
  unsigned long hash = 5381;
  int c;
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;  // hash * 33 + c
  }
  return hash;
}


void _hash_resize(struct hash* hash) {

  /*
   * Remember the old table array and its capacity and re-initialize the
   * hash with a new table array with twice the capacity.
   */
  struct association** old_table = hash->table;
  unsigned int old_capacity = hash->capacity;
  _hash_table_init(hash, old_capacity * 2);

  /*
   * Loop through the old table and re-hash all the old elements via
   * hash_insert().  This will handle updating the table's size for us.  By
   * definition, this will not cause a recursive call to _hash_resize(),
   * since the new table's capacity is doubled.  Free all the old assocation
   * structures.
   */
  for (int i = 0; i < old_capacity; i++) {
    struct association* cur = old_table[i];
    while (cur != NULL) {
      struct association* tmp = cur;
      hash_insert(hash, cur->key, cur->value);
      cur = cur->next;
      free(tmp);
    }
  }

  free(old_table);

}


void hash_insert(struct hash* hash, char* key, void* value) {
  assert(hash);
  assert(key);

  if (hash_load_factor(hash) > LOAD_FACTOR_THR) {
    _hash_resize(hash);
  }

  // Compute a hash value for the given key and mod to convert it to an index.
  unsigned int hashval = _djb_hash(key);
  unsigned int idx = hashval % hash->capacity;

  // Find the key if it already exists in the table.
  struct association* cur = hash->table[idx];
  struct association* prev = NULL;
  while (cur != NULL) {
    if (!strcmp(key, cur->key)) {
      break;
    }
    prev = cur;
    cur = cur->next;
  }

  if (cur != NULL) {

    if (value != NULL) {

      /*
       * If the key we're looking for exists in the table and we have a new
       * value for it, assign the new value.
       */
      cur->value = value;

    } else {

      /*
       * If the key we're looking for exists in the table and the user asked
       * to remove it (by passing a NULL value), the remove the association
       * from the table by updating link pointers.
       */
      if (prev != NULL) {
        prev->next = cur->next;
      } else {
        hash->table[idx] = cur->next;
      }

      free(cur);
      hash->num_elems--;

    }

  } else if (value != NULL) {

    /*
     * If the user wants to add a new key/value pair into the table, allocate
     * a new association structure for it and put the new association at the
     * head of the chain for its bucket.
     */
    cur = malloc(sizeof(struct association));
    cur->key = key;
    cur->value = value;
    if (hash->table[idx] != NULL) {
      cur->next = hash->table[idx];
    } else {
      cur->next = NULL;
    }
    hash->table[idx] = cur;
    hash->num_elems++;

  }
}


void* hash_get(struct hash* hash, char* key) {
  assert(hash);
  assert(key);

  // Compute a hash value for the given key and mod to convert it to an index.
  unsigned int hashval = _djb_hash(key);
  unsigned int idx = hashval % hash->capacity;

  // Find the key if it exists in the table.
  struct association* cur = hash->table[idx];
  while (cur != NULL) {
    if (!strcmp(key, cur->key)) {
      return cur->value;
    }
    cur = cur->next;
  }

  // If we made it here, we haven't found the key we're looking for.
  return NULL;
}
