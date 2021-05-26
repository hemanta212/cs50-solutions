#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(int *array, int array_len, int *first, int *second, int first_len,
           int second_len);
void merge_sort(int *array, int array_len, int start, int end);

int main(int argc, char *argv[]) {
  if (argc == 1) 
    return 1;

  int array[100];
  int array_len = argc - 1;
  for (int i = 0; i < array_len; i++) {
    array[i] = strtod(argv[i+1], NULL);
  }    
  int end = array_len - 1;

  merge_sort(array, array_len, 0, end);
  for (int i = 0; i < array_len; i++) {
    printf("%d ", array[i]);
  }    
  puts("");
  return 0; // EXIT_SUCCESS;
}

void merge_sort(int *array, int array_len, int start, int end) {
  if (start < end) {
    int mid = (start + end) / 2;
    merge_sort(array, array_len, start, mid);
    merge_sort(array, array_len, mid + 1, end);

    int first_len = (mid - start) + 1;
    int second_len = (end - (mid + 1)) + 1;
    int *first_list = malloc(sizeof(int) * first_len);
    int *second_list = malloc(sizeof(int) * second_len);
    for (int i = 0; i < first_len; i++) {
      first_list[i] = start;
      start++;
    }
    for (int i = 0; i < second_len; i++) {
      second_list[i] = mid + 1;
      mid++;
    }

    merge(array, array_len, first_list, second_list, first_len, second_len);

    free(first_list);
    free(second_list);
  }
  return;
}

void merge(int *array, int array_len, int *first, int *second, int first_len,
           int second_len) {
  int first_i = 0;
  int second_i = 0;
  int merged_len = first_len + second_len;
  int *merged = malloc(sizeof(int) * merged_len);
  int curr_merged_i = 0;

  while (!(first_i == first_len && second_i == second_len)) {
    if (first_i == first_len) {
      merged[curr_merged_i++] = second[second_i++];
      continue;
    } else if (second_i == second_len) {
      merged[curr_merged_i++] = first[first_i++];
      continue;
    }

    int index_f = first[first_i];
    int index_s = second[second_i];
    if (array[index_f] < array[index_s]) {
      merged[curr_merged_i++] = index_f;
      first_i++;
    } else {
      merged[curr_merged_i++] = index_s;
      second_i++;
    }
  }

  int *original_order = malloc(sizeof(int) * merged_len);
  for (int i = 0; i < first_len; i++) {
    original_order[i] = first[i];
  }
  for (int i = first_len, j = 0; i < merged_len; i++) {
    original_order[i] = second[j++];
  }

  int *array_dup = malloc(sizeof(int) * array_len);
  for (int i = 0; i < array_len; i++) {
    array_dup[i] = array[i];
  }

  for (int i = 0; i < merged_len; i++) {
    array[original_order[i]] = array_dup[merged[i]];
  }

  free(merged);
  free(original_order);
  free(array_dup);
  return;
}
