#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t HEADERBYTE;
typedef int16_t BYTE;

int main(int argc, char *argv[]) {
  const int HEADER_SIZE = 44;

  if (argc != 4) {
    fprintf(stderr, "Usage: ./volume input.wav output.wav factor\n");
    return 1;
  }

  float factor = atof(argv[3]);
  FILE *file = fopen(argv[1], "r");
  FILE *to_file = fopen(argv[2], "w");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file!\n");
    return 1;
  }

  if (to_file == NULL) {
    fclose(file);
    fprintf(stderr, "Failed to copy!\n");
    return 1;
  }

  // the headers and body bits are two types you get in a binary file
  // extracting the headers
  HEADERBYTE headers[HEADER_SIZE];
  fread(&headers, sizeof(HEADERBYTE), HEADER_SIZE, file);
  fwrite(&headers, sizeof(HEADERBYTE), HEADER_SIZE, to_file);

  BYTE sample;
  while (fread(&sample, sizeof(BYTE), 1, file) == 1) {
    sample *= factor;
    fwrite(&sample, sizeof(BYTE), 1, to_file);
  }

  fclose(to_file);
  fclose(file);
  return 0;
}
