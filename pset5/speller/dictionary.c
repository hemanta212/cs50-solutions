// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "dictionary.h"

/* Represents a node in a hash table */
typedef struct ht_node {
  char word[LENGTH + 1];
  struct ht_node *next;
} ht_node;

// utility functions
static int explore_node(const ht_node *node);
static void del_node(ht_node *node);
static int load_to_index(const char *word, const int index);
static int read_word(char word[], FILE *file);

ht_node **table;

/* Returns true if word is in dictionary, else false */
bool check(const char *word) {
  static const int ram = 3;
  int index = hash(word);
  if (table[index]) {
    ht_node *curr_node = table[index];
    while (curr_node != NULL)
      if (strcmp(curr_node->word, word) == 0)
        return true;
      else
        curr_node = curr_node->next;
  }
  return false;
}

// Hashes word to a number
unsigned int hash(const char *word) {
  unsigned long long hash_value = 0;
  const int PRIME_SEED = 7.0;
  int key_len = strlen(word);
  for (int i = 0; i < key_len; i++) {
    hash_value += pow(PRIME_SEED, ((key_len - i) + word[i]) / PRIME_SEED);
  }
  hash_value = hash_value % N;
  return hash_value;
}

/* Loads dictionary into memory, returning true if successful, else false */
bool load(const char *dictionary) {
  table = calloc(N, sizeof(ht_node *));

  if (dictionary == NULL) {
    return false;
  }

  FILE *file = fopen(dictionary, "r");
  if (file == NULL) {
    return false;
  }

  char word[LENGTH + 1];
  int read;
  while ((read = read_word(word, file)) == 1) {
    int index = hash(word);
    unsigned int loaded = load_to_index(word, index);
    printf(" :: Loaded %s : %d --> %d\n", word, index, loaded);
  }
  fclose(file);
  return true;
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
    if (table[i] != 0)
      del_node(table[i]);
  }
  free(table);
  return true;
}

static int load_to_index(const char *word, const int index) {
  ht_node *curr_node = table[index];
  ht_node *item = malloc(sizeof(ht_node));
  strcpy(item->word, word);

  if (curr_node == 0) {
    printf("a");
    item->next = NULL;
    table[index] = item;
  } else if (check(word)){
    // same word already exists in dictionary
    printf("c");
    free(item->word);
    free(item);
    return 0;
  } else {
    printf("b");
    item->next = curr_node;
    table[index] = item;
  }
  return 1;
}

static int explore_node(const ht_node *node) {
  int sum = 0;
  while (node != NULL) {
    explore_node(node->next);
    sum++;
  }
  return sum;
}

static void del_node(ht_node *node) {
  while (node != NULL) {
    del_node(node->next);
  }
  free(node->word);
  free(node);
}

static int read_word(char word[], FILE *file) {
  int word_len = 0;
  // keep reading char by char to buf until fread returns 0
  for (char buf; fread(&buf, sizeof(char), 1, file) == 1; word_len++) {
    if (buf == '\n') {
      // we've encounred end of line wrap up the word and print it
      word[word_len] = '\0';
      return 1;
    } else {
      word[word_len] = buf;
    }
  }
  // for loop ended without hittng return 1 that means fread is exhaused file is
  // finished
  return 0;
}
