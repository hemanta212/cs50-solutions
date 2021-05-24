#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("Usage: ./caesar key");
    return 1;
  }

  char *input_k_value = argv[1];
  for (int i=0; input_k_value[i]!='\0'; i++){
    int isDigit =  (input_k_value[i] >= '0') && (input_k_value[i] <= '9');
    if (!isDigit){
      puts("Usage: ./caesar key");
      return 1;
    }
  }

  errno = 0; // This is sort of global variable of language already initialized
  unsigned long long k_value = strtoull(input_k_value, NULL, 10);
  if (errno == ERANGE) { // Cool we're good errno not in error range.
    puts("Usage: ./caesar <key>");
    return 1;
  }

  char plain_text[100], cipher_text[100];
  int i;

  printf("plaintext: ");
  // Rads whole line and assignms only first 100 chars to plain text & discard
  // other
  scanf("%100[^\n]%*[^\n]", plain_text);

  for (i = 0; plain_text[i] != '\0'; i++) {
    const int UPPER_MIN=65, UPPER_MAX=90, LOWER_MIN=97, LOWER_MAX=122;
    int is_lower_alpha = plain_text[i] >= LOWER_MIN && plain_text[i] <= LOWER_MAX;
    int is_upper_alpha = plain_text[i] >= UPPER_MIN && plain_text[i] <= UPPER_MAX;
    int offset_value = is_lower_alpha ? LOWER_MIN : UPPER_MIN;

    if (is_lower_alpha || is_upper_alpha){
      char offseted_alpha = plain_text[i] - offset_value;
      char encrypted_alpha = (offseted_alpha + k_value) % 26;
      cipher_text[i] = offset_value + encrypted_alpha;
    }
    else{
      cipher_text[i] = plain_text[i];
    }
  }
  cipher_text[i] = '\0';

  printf("ciphertext: %s\n", cipher_text);
  return 0;
}
