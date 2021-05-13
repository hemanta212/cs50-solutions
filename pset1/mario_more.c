#include <stdio.h>

int main(){
  int n;

  do{
    printf("Height: ");
    scanf("%d", &n);
  }while( n < 1 || n > 8);

  for(int i=1; i <= n; i++){
      int hash = i;
      int space = n - i;

      for(int j = 0; j < space; j++){
        printf(" ");
      }

      for(int k=0; k < hash; k++){
	printf("#");
      }
      printf("  ");

      for(int l=0; l < hash; l++){
	printf("#");
      }

      printf("\n");
  }
}

