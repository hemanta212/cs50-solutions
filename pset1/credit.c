/*

THEORY : 1

Separate out digit from the number how to?

consider a number 19
To get the last digit we can: 19 % 10 -> 9 remainder
Now we can subtract 9 from 19 -> 10 and divide it by 10 to get 1

Extend this logic to numbers with 3 digit.
eg. 345
345 % 10 -> 5
345 - 5 -> 340
340 / 10 -> 34
34%10 -> 4
34 - 4 -> 30
30/10 -> 3

THUS WE GOT 5 4 3 in that order

NOTE: In C 34/10 and 30/10 is same if the variable you assign is int so the
subtraction step is not really necessary

Theory : 2

Reversing the step Reconstructing number from digit
eg. Given 1 0 0 12 34 4 Produce int: 10012344

Start With 0
Addign single digits like 1, 0, 4
Multiply by 10 and add that digit

Adding 1
0 * 10 + 1 -> 1
Adding 0
1 * 10 + 0 -> 10
Adding 4
10 * 10 -> 100 + 4 -> 104

Adding Double digits like 34 or 12
Multily by 100 this time.

Initial number 104.
Add 12
104 * 100 -> 10400 + 12 -> 10412
Add 34
10412 * 100 -> 1041200 + 34 -> 1041234
*/

#include <stdio.h>
#include <math.h>

int main() {
  unsigned long long card_number;

  printf("Card Number: ");
  scanf("%llu", &card_number);

  int sum_of_unprocessed_digits = 0;
  unsigned long long product_number = 0;
  int first_two_digits;
  int number_length = 0;

  while (card_number > 0) {
    int digit = card_number % 10;

    if (number_length % 2 == 0)
      sum_of_unprocessed_digits += digit;
    else {
      int product = digit * 2;
      if(product >= 10)
        product_number = product_number * 100;
      else
        product_number = product_number * 10;
      product_number += product;
    }

    if (card_number >= 10 && card_number < 100)
      first_two_digits = card_number;

    unsigned long long divider = 10; 
    card_number -= digit;
    card_number = card_number / divider;
    number_length++;
  }

  int sum_of_product_digits = 0;
  for (int i = 0; product_number > 0; i++) {
    int digit = product_number % 10;
    sum_of_product_digits += digit;
    product_number = product_number / 10;
  }

  int checksum = sum_of_product_digits + sum_of_unprocessed_digits;
  int is_checksum_valid = 0;
  if (checksum % 10 == 0)
    is_checksum_valid = 1;

  int is_american_express =
      (first_two_digits == 34 || first_two_digits == 37) && number_length == 15;
  int is_visa = (first_two_digits >= 40 && first_two_digits <= 49) &&
                (number_length == 13 || number_length == 16);
  int is_mastercard =
      (first_two_digits >= 51 && first_two_digits <= 55) && number_length == 16;

  if(is_checksum_valid && is_american_express)
    printf("AMEX\n");
  else if(is_checksum_valid && is_visa)
    printf("VISA\n");
  else if(is_checksum_valid && is_mastercard)
    printf("MASTERCARD\n");
  else
    printf("INVALID\n");

  return 0;
}
