#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash.h"

#define NUM_TEST_ELEMS 256
#define MAX_KEY 1000000
#define MAX_KEY_LEN 16

int main(int argc, char** argv) {

  struct hash* hash = hash_create();
  int values[NUM_TEST_ELEMS];
  char* keys[NUM_TEST_ELEMS];

  for (int i = 0; i < NUM_TEST_ELEMS; i++) {
    values[i] = rand() % MAX_KEY;
    keys[i] = malloc(MAX_KEY_LEN * sizeof(char));
    snprintf(keys[i], MAX_KEY_LEN, "%d", values[i]);
    hash_insert(hash, keys[i], &values[i]);
  }

  printf("== Load factor after %d inserts: %f\n", NUM_TEST_ELEMS,
    hash_load_factor(hash));

  for (int i = 0; i < NUM_TEST_ELEMS; i++) {
    int* val = hash_get(hash, keys[i]);
    assert(val != NULL && *val == values[i]);
  }

  printf("== Successfully found all inserted elements.\n");

  for (int i = 0; i < NUM_TEST_ELEMS; i++) {
    free(keys[i]);
  }
  hash_free(hash);
}
