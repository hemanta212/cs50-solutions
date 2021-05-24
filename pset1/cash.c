#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>


int main() {
  float amount_owed;
  int min_coins = 0;
  const int quarter=25, dimes=10, nickel=5, pennies=1;
  int isinvalid;

  do{
    isinvalid = 1;
    char amount[100];
    printf("Cash Owed: ");
    scanf("%s", amount);
    for(int i =0; amount[i] != '\0'; i++){
      if(isalpha(amount[i]))
	isinvalid = 0;
    }
    amount_owed = strtof(amount, NULL);

  }while( amount_owed < 0.0 || !isinvalid);

  amount_owed = round(amount_owed * 100);

  while(amount_owed > 0){
    if (amount_owed >= quarter)
      amount_owed -= quarter;
    else if (amount_owed >= dimes)
      amount_owed -= dimes;
    else if (amount_owed >= nickel)
      amount_owed -= nickel;
    else if (amount_owed >= pennies)
      amount_owed -= pennies;

    min_coins++;
  }

  printf("%d\n", min_coins);
  return 0;
}

