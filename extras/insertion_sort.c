#include <stdio.h>

int main(void){
  int array[] = {5,4,3,2};
  int array_len = sizeof(array) / sizeof(int);

  for(int i=1; i<array_len; i++){
    int hole = i;
    int value = array[i];
    while(hole > 0 && array[hole-1] > value){
      array[hole] = array[hole-1];
      hole--;
    }
    array[hole] = value;
  }
  for(int i=0; i<array_len; i++){
    printf("%d ", array[i]);
  }
  return 0;

}
