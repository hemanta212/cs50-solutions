#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct {
  int winner;
  int loser;
} pair;

// Array of candidates
char *candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool is_path_between(int from, int to);
void lock_pairs(void);
void print_winner(void);

int main(int argc, char *argv[]) {
  // Check for invalid usage
  if (argc < 2) {
    printf("Usage: tideman [candidate ...]\n");
    return 1;
  }

  // Populate array of candidates
  candidate_count = argc - 1;
  if (candidate_count > MAX) {
    printf("Maximum number of candidates is %i\n", MAX);
    return 2;
  }
  for (int i = 0; i < candidate_count; i++) {
    candidates[i] = argv[i + 1];
  }

  // Clear graph of locked in pairs
  for (int i = 0; i < candidate_count; i++) {
    for (int j = 0; j < candidate_count; j++) {
      locked[i][j] = false;
    }
  }

  pair_count = 0;
  int voter_count;
  printf("Number of voters: ");
  scanf("%d", &voter_count);

  // Query for votes
  for (int i = 0; i < voter_count; i++) {
    // ranks[i] is voter's ith preference
    int ranks[candidate_count];

    // Query for each rank
    for (int j = 0; j < candidate_count; j++) {
      char name[100];
      printf("Rank %d: ", j + 1);
      scanf("%s", name);

      if (!vote(j, name, ranks)) {
        printf("Invalid vote.\n");
        return 3;
      }
    }

    record_preferences(ranks);

    printf("\n");
  }

  add_pairs();
  sort_pairs();
  lock_pairs();
  print_winner();
  return 0;
}

// Update ranks given a new vote
bool vote(int rank, char *name, int ranks[]) {
  for (int i = 0; i < candidate_count; i++) {
    if (strcmp(candidates[i], name) == 0) {
      ranks[rank] = i;
      return true;
    }
  }
  return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) {
  for (int i = 0; i < candidate_count; i++) {
    int winner = ranks[i];
    int losers_no = (candidate_count - 1) - i;
    for (int j = i + 1; j <= i + losers_no; j++) {
      int loser = ranks[j];
      preferences[winner][loser]++;
    }
  }
  return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) {
  for (int i = 0; i < candidate_count; i++) {
    for (int j = 0; j < candidate_count; j++) {
      int tied_or_lost = preferences[i][j] <= preferences[j][i];
      if (!tied_or_lost) {
        pair new_pair;
        new_pair.winner = i;
        new_pair.loser = j;
        pairs[pair_count++] = new_pair;
      }
    }
  }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void) {
  for (int i = 1; i < pair_count; i++) {
    int hole = i;
    pair curr_pair = pairs[i];
    int weight = preferences[curr_pair.winner][curr_pair.loser];
    pair next_pair = pairs[hole - 1];
    int next_weight = preferences[next_pair.winner][next_pair.loser];
    while (hole > 0 && next_weight < weight) {
      pairs[hole] = pairs[hole - 1];
      hole--;
      next_pair = pairs[hole - 1];
      next_weight = preferences[next_pair.winner][next_pair.loser];
    }
    pairs[hole] = curr_pair;
  }
  return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
  for (int i = 0; i < pair_count; i++) {
    pair curr_pair = pairs[i];
    int winner = curr_pair.winner, loser = curr_pair.loser;
    if (!is_path_between(loser, winner)) {
      locked[winner][loser] = true;
    }
  }
  return;
}

void print_winner(void) {
  for (int i = 0; i < candidate_count; i++) {
    int is_source = true;
    for (int j = 0; j < candidate_count; j++) {
      if (i !=j && is_path_between(j,i) )
        is_source = false;
    }
    if(is_source){
      printf("%s\n", candidates[i]);
    }
  }
  return;
}

bool is_path_between(int from, int to){
  if(from == to)
    return true;
  else{
    for(int i=0; i<candidate_count; i++){
      if(locked[from][i]){
	if(is_path_between(i, to))
	  return true;
      }
    }
    return false;
  }
}
