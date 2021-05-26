/*
BROKEN APPROACH

Why?
First, print winner should be able to print multiple winner scenario and hsould pass following tests

TEST EXAMPLES

3 candidate (a,b,c) 2 voters
a b c
b a c

3 candidate (a,b,c) 3 voters
a b c
b a c
c a b

In both above cases it is not draw a is clear winner

Tied scenario test
3 candidates and 2 voteh
a b c
b a c

Both a and b are source of the graph so both winner

*/

#include <stdio.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9
#define true 1
#define false 0
#define bool int

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
int candidate_weights[MAX];

// Function prototypes
void print_array(int array[], int array_len);
bool vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool detect_cycle(void);
bool is_path_between(int from, int to);
bool cycle_graph(int column, int cycle_info[], int cycle_info_len);
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

  for (int i = 0; i < pair_count; i++) {
    pair curr_pair = pairs[i];
    int weight = preferences[curr_pair.winner][curr_pair.loser];
    printf("%d. (%d, %d): %d \n", i, curr_pair.winner, curr_pair.loser, weight);
  }
  return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
  for (int i = 0; i < pair_count; i++) {
    pair curr_pair = pairs[i];
    int winner = curr_pair.winner, loser = curr_pair.loser;
    printf(":: Trying to lock pair (%d,%d)\n", winner, loser);
    puts("---");
    for (int j = 0; j < candidate_count; j++) {
      for (int k = 0; k < candidate_count; k++) {
        printf("%d ", locked[j][k]);
      }
      puts("");
    }
    puts("---");

    if (!is_path_between(loser, winner)) {
      locked[winner][loser] = true;
    }
  }
  return;
}

// Print the winner of the election
void print_winner(void) {
  for (int i = 0; i < candidate_count; i++) {
    for (int j = 0; j < candidate_count; j++) {
      if (locked[i][j]) {
        candidate_weights[i]++;
      }
    }
  }
  print_array(candidate_weights, candidate_count);
  int highest = 0;
  for (int i = 0; i < candidate_count; i++) {
    if (candidate_weights[i] > highest) {
      highest = candidate_weights[i];
    }
  }
  for (int i = 0; i < candidate_count; i++) {
    if (candidate_weights[i] == highest) {
      printf("%s\n", candidates[i]);
    }
  }
  // TODO
  return;
}

bool detect_cycle(void) {
  int cycle_info_len = 0;
  int cycle_info[candidate_count + 1];
  for (int i = 0; i < candidate_count; i++) {
    cycle_info[cycle_info_len++] = i;
    int cycle = cycle_graph(i, cycle_info, cycle_info_len);
    printf(":: Got value cycle_graph: %d\n", cycle);
    if (cycle) {
      printf(":: Cycle detected");
      return true;
    }
    cycle_info_len = 0;
  }
  return false;
}

bool cycle_graph(int column, int cycle_info[], int cycle_info_len) {
  printf("::Target:\n");
  print_array(cycle_info, cycle_info_len);
  printf("scanning column %d \n", column);

  for (int i = 0; i < candidate_count; i++) {
    printf("::Locked (%d,%d): %d\n", column, i, locked[column][i]);
    if (locked[column][i]) {
      print_array(cycle_info, cycle_info_len);
      for (int j = 0; j < cycle_info_len; j++) {
        if (cycle_info[j] == i) {
          puts(":: Target matched");
          return true;
        }
      }
      cycle_info[cycle_info_len++] = i;
      if (cycle_graph(i, cycle_info, cycle_info_len))
        return true;
    }
  }
  cycle_info_len--;
  puts(":: Returning Cycle_graph emptyhanded");
  print_array(cycle_info, cycle_info_len);
  return false;
}

bool is_path_between(int from, int to){
  if(from == to)
    return true;
  else{
    for(int i=0; i<candidate_count; i++){
      if(locked[from][i]){
	return is_path_between(i, to);
      }
    }
    return false;
  }
}

void print_array(int array[], int array_len) {
  for (int i = 0; i < array_len; i++) {
    printf("%d ", array[i]);
  }
  puts("");
}
