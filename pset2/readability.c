#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // Not allowed in pset1 but necessary to emulate get_string from cs50
  char *given_text = malloc(sizeof(char) * 10000);
  printf("Text: ");
  scanf("%10000[^\n]%*[^\n]", given_text);

  int char_buffer = 0;
  int word_count = 1;
  int sentence_count = 0;

  for (int i = 0; given_text[i] != '\0'; i++) {
    char current_char = given_text[i];
    // Sentence fileter (fullstop, exclamation and ques mark)
    if (current_char == '.' || current_char == '!' || current_char == '?')
      sentence_count++;
    // World fileter : (newline, spaces)
    else if (current_char == ' ' || current_char == '\n')
      word_count++;
    // lowercase alphabet ascii 97-122 and uppercase 65-90
    const int UPPER_MIN = 65, UPPER_MAX = 90, LOWER_MIN = 97, LOWER_MAX = 122;
    int is_lower_alpha = current_char >= LOWER_MIN && current_char <= LOWER_MAX;
    int is_upper_alpha = current_char >= UPPER_MIN && current_char <= UPPER_MAX;
    if (is_lower_alpha || is_upper_alpha)
      char_buffer++;
  }

  double per_hundred = 100.0;
  double word_complexity_L = (char_buffer / (double)word_count) * per_hundred;
  double sentence_complexity_S =
      (sentence_count / (double)word_count) * per_hundred;

  // Coleman Lieu index Formula
  double grade =
      (0.0588 * word_complexity_L) - (0.296 * sentence_complexity_S) - 15.8;
  int rounded_grade = round(grade);

  if (rounded_grade >= 16)
    puts("Grade 16+");
  else if (rounded_grade < 1)
    puts("Before Grade 1");
  else
    printf("Grade %d\n", rounded_grade);

  // Not allowed in pset1/week1 but necessary to emulate get_string from cs50
  free(given_text);
  return 0;
}
