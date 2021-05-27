#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const int HEADER_SIZE = 44;
 
  if (argc != 4){
    fprintf(stderr, "Usage: ./volume input.wav output.wav factor\n");
    return 1;
  }

  float factor = atof(argv[3]);
  FILE *file = fopen(argv[1], "r");
  FILE *to_file = fopen(argv[2], "w");
  if (!file){
    fprintf(stderr, "Failed to open file!\n");
    return 1;
  }

  if (!to_file) {
    fclose(file);
    fprintf(stderr, "Failed to copy!\n");
    return 1;
  }

  uint8_t headers[HEADER_SIZE];
  int16_t sample;
  int counter = 0;
  while (1) {
    if (counter < HEADER_SIZE) {
      fread(headers, sizeof(uint8_t), 1, file);
      fwrite(headers, sizeof(uint8_t), 1, to_file);
    } else {
      int read = fread(&sample, sizeof(int16_t), 1, file);
      if(!read)
	break;
      sample = sample * factor;
      fwrite(&sample, sizeof(uint16_t), 1, to_file);
    }
    counter++;
  }

  fclose(file);
  fclose(to_file);
  return 0;
}
