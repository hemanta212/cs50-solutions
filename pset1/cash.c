#include <stdio.h>
#include <math.h>

int main() {
  float amount_owed;
  int min_coins = 0;
  const int quarter=25, dimes=10, nickel=5, pennies=1;

  do{
    printf("Cash Owed: ");
    scanf("%f", &amount_owed);
  }while( amount_owed < 0.0);

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

