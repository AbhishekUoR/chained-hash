#ifndef __HASH_H
#define __HASH_H

/*
 * Structure used to represent a hash table.
 */
struct hash;

/*
 * Create a new hash table.
 */
struct hash* hash_create();

/*
 * Free the memory associated with a hash table.
 */
void hash_free(struct hash* hash);

/*
 * Returns the load factor (the average number of elements per bucket) of a
 * hash table.
 */
float hash_load_factor(struct hash* hash);

/*
 * Inserts (or updates) a value with a given key into a hash table.  A NULL
 * value removes the specified element from the table.
 */
void hash_insert(struct hash* hash, char* key, void* value);

/*
 * Returns the value of an element with a given key from a hash table.  Returns
 * NULL if the element doesn't exist.
 */
void* hash_get(struct hash* hash, char* key);

#endif
