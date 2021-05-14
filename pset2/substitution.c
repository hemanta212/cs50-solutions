#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  const int UPPER_ALPHA_MIN = 65, UPPER_ALPHA_MAX = 90, LOWER_ALPHA_MIN = 97,
            LOWER_ALPHA_MAX = 122;

  if (argc != 2) {
    puts("Usage: ./substitution key");
    return 1;
  }

  char *input_k_value = argv[1];
  int index = 0;
  char keys_index[26];
  // This stores encrypted version of alphabet from 0-25
  // However we have to start at A=1 at keys_index[] because 0 ascii symbolizes '\0'
  int SPECIAL_KEYS_INDEX_OFFSET = 1;

  while (input_k_value[index] != '\0') {
    const char current_char = input_k_value[index];
    int is_lower_alpha =
        current_char >= LOWER_ALPHA_MIN && current_char <= LOWER_ALPHA_MAX;
    int is_upper_alpha =
        current_char >= UPPER_ALPHA_MIN && current_char <= UPPER_ALPHA_MAX;
    if (!is_lower_alpha && !is_upper_alpha) {
      puts("Usage: ./substitution key");
      return 1;
    }

    int offset_value =
        is_lower_alpha ? LOWER_ALPHA_MIN : UPPER_ALPHA_MIN;
    char offseted_alpha = current_char - (offset_value - SPECIAL_KEYS_INDEX_OFFSET);
    int isDuplicate = 0;
    for (int j = 0; input_k_value[j] != '\0'; j++) {
      if (index != j && current_char == input_k_value[j]) {
        isDuplicate = 1;
      }
    }

    if (!isDuplicate) {
      keys_index[index] = offseted_alpha;
    } else {
      printf("ERROR: Duplicate values in key (%c)\n", current_char);
      return 1;
    }

    index++;
  }

  int input_length = index;
  if (input_length != 26) {
    puts("Usage: ./substitution key");
    return 1;
  }

  char plain_text[100];
  char cipher_text[100];

  printf("plaintext: ");
  // Rads whole line and assignms only first 100 chars to plain text & discard
  // other
  scanf("%100[^\n]%*[^\n]", plain_text);

  for (int i = 0; plain_text[i] != '\0'; i++) {
    int is_lower_alpha =
        plain_text[i] >= LOWER_ALPHA_MIN && plain_text[i] <= LOWER_ALPHA_MAX;
    int is_upper_alpha =
        plain_text[i] >= UPPER_ALPHA_MIN && plain_text[i] <= UPPER_ALPHA_MAX;
    int offset_value = is_lower_alpha ? LOWER_ALPHA_MIN : UPPER_ALPHA_MIN;

    if (is_lower_alpha || is_upper_alpha) {
      int alpha_index = plain_text[i] - offset_value;
      char encrypted_alpha = offset_value + keys_index[alpha_index];
      cipher_text[i] = encrypted_alpha - SPECIAL_KEYS_INDEX_OFFSET;
    } else {
      cipher_text[i] = plain_text[i];
    }
  }

  printf("ciphertext: %s\n", cipher_text);
  return 0;
}
