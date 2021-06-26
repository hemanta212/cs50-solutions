// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "dictionary.h"

/* Represents a node in a hash table */
typedef struct ht_node {
  char word[LENGTH + 1];
  struct ht_node *next;
} ht_node;

unsigned int explore_node(const ht_node *node);
void del_node(ht_node *node);
unsigned int load_to_index(const char *word, const int index);
unsigned int hash(const char *word);

// Number of buckets in hash table
const unsigned int N = 51;

ht_node **table = calloc(N, sizeof(ht_node*));

/* Returns true if word is in dictionary, else false */
bool check(const char *word) {
  int index = hash(word);
  if (table[index])
    return true;
  return false;
}

// Hashes word to a number
unsigned int hash(const char *word) {
  unsigned long long hash_value = 0;
  const int PRIME_SEED = 7.0;
  int key_len = strlen(word);
  for (int i = 0; i < key_len; ++i) {
    hash_value += pow(PRIME_SEED, ( (key_len - i) * word[i] ) / PRIME_SEED);
  }
  printf("Hash value %llu\n", hash_value);
  hash_value = hash_value % N;
  return hash_value;
}

/* Loads dictionary into memory, returning true if successful, else false */
bool load(const char *dictionary) {
  if (dictionary == NULL) {
    return false;
  }
  FILE *file = fopen(dictionary, "r");
  if (file == NULL) {
    return false;
  }
  char buf;
  int word_index = 0;
  char word[LENGTH + 1];
  printf("a\n");
  while ((fread(&buf, sizeof(char), 1, file)) == 1) {
    if (buf == '\n') {
      printf("n\n");
      word[word_index] = '\0';
      int index = hash(word);
      load_to_index(word, index);
      printf("l\n");
      word_index = 0;
    } else {
      word[word_index] = tolower(buf);
      word_index++;
    }
  }
  printf("c");
  fclose(file);
  // TODO
  return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void) {
  int sum = 0;
  for (int i = 0; i < N; i++) {
    if (table[i] == NULL)
      continue;
    sum += explore_node(table[i]);
  }
  return sum;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void) {
  for (int i = 0; i < N; i++) {
    if (table[i] != NULL)
      del_node(table[i]);
  }
  return true;
}

unsigned int load_to_index(const char *word, const int index) {
  printf("t\n");
  ht_node *curr_node = table[index];
  ht_node *item = malloc(sizeof(ht_node));
  strcpy(item->word, word);

  printf("t\n");
  if (curr_node == NULL) {
    printf("N\n");
    item->next = NULL;
    table[index] = item;
  } else if (strcmp(curr_node->word, word) != 0) {
    item->next = curr_node;
    table[index] = item;
  } else {
    free(item->word);
    free(item);
    return 0;
  }
  return 1;
}

unsigned int explore_node(const ht_node *node) {
  int sum = 0;
  while (node != NULL) {
    explore_node(node->next);
    sum++;
  }
  return sum;
}

void del_node(ht_node *node) {
  while (node != NULL) {
    del_node(node->next);
  }
  free(node->word);
  free(node);
}
