#include <stdint.h>
#include <stdio.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

void get_next_filename(int file_count, char *filename);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./recover file.raw\n");
    return 1;
  }
  // Get the file command arg
  char *file_arg = argv[1];

  // prerpare the card raw file to open else just close and return
  FILE *raw_file = fopen(file_arg, "r");
  if (raw_file == NULL) {
    fprintf(stderr, "Couldnot read file %s\n", file_arg);
    return 1;
  }

// --int-main-block--
  // couner for keeping tarck of nu of files and help name them
  int file_count = 0;
  FILE *current_file = NULL;

  // Instead of scanning each bytes we count in batch of 512
  BYTE block[BLOCK_SIZE];

  while (fread(block, sizeof(BYTE), BLOCK_SIZE, raw_file) == BLOCK_SIZE) {

// --int-main-block--
  // --while-block--
    // last byte of 4 jpg byte ranges from 0xe0-0xef so we make sure it stats
    // with 0xe
    int jpeg_last = block[3] / 16 == 0xe;

    // check if we found a starting mark of jpeg
    if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && jpeg_last) {
          // Abort writing to current file if there is one
          if (current_file != NULL)
                fclose(current_file);

// --int-main-block--
  // --while-block--
    // --if-block--
      // ### for filename and .jpg for ext and \0 --> total 8
      char filename[8];

      // Get the next filename
      get_next_filename(file_count, filename);

      // Open the next file
      current_file = fopen(filename, "w");
      if (current_file == NULL) {
        fclose(raw_file);
        fprintf(stderr, "Couldnot write to file %s\n", filename);
        return 1;
      }

      file_count++;
    }

// --int-main-block--
  // --while-block--

    // If we didnot encounter jpeg demarkation in this block just continue
    // writing to current file
    if (current_file != NULL) {
      fwrite(block, sizeof(BYTE), BLOCK_SIZE, current_file);
    }
  }

  // the current image output file wont close on the last block
  fclose(current_file);
  fclose(raw_file);
  return 0;
}

void get_next_filename(int file_count, char *filename) {
  if (file_count < 10)
    sprintf(filename, "00%d.jpg", file_count);
  else if (file_count < 100)
    sprintf(filename, "0%d.jpg", file_count);
  else
    sprintf(filename, "%d.jpg", file_count);
}
