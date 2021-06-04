#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define NEIGHBOUR_COL 3
#define NEIGHBOUR_ROW 3

int round_and_cap(double number);
int get_neighbours(int height, int width, int MAX_H, int MAX_W,
                   RGBTRIPLE neighbours[], RGBTRIPLE image[MAX_H][MAX_W]);
void get_edge_neighbours(int height, int width, int MAX_H, int MAX_W,
                         RGBTRIPLE neighbours[NEIGHBOUR_ROW][NEIGHBOUR_COL],
                         RGBTRIPLE image[MAX_H][MAX_W]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      RGBTRIPLE pixel = image[h][w];
      BYTE avg =
          round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);
      pixel.rgbtBlue = avg;
      pixel.rgbtGreen = avg;
      pixel.rgbtRed = avg;
      image[h][w] = pixel;
    }
  }
  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < (int)width / 2; w++) {
      int reflected_index = (width - 1) - w;
      RGBTRIPLE counterPartPixel = image[h][reflected_index];
      image[h][reflected_index] = image[h][w];
      image[h][w] = counterPartPixel;
    }
  }
  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
  const int MAX_NEIGHBOURS = 9;
  RGBTRIPLE backup_img[height][width];
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      backup_img[h][w] = image[h][w];
    }
  }

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      RGBTRIPLE neighbours[MAX_NEIGHBOURS];
      int neighbours_count =
          get_neighbours(h, w, height, width, neighbours, backup_img);

      RGBTRIPLE curr_pixel = image[h][w];
      int total_red = 0, total_blue = 0, total_green = 0;
      for (int n = 0; n < neighbours_count; n++) {
        RGBTRIPLE pixel = neighbours[n];
        total_red += pixel.rgbtRed;
        total_blue += pixel.rgbtBlue;
        total_green += pixel.rgbtGreen;
      }

      curr_pixel.rgbtRed = round(total_red / (float)neighbours_count);
      curr_pixel.rgbtBlue = round(total_blue / (float)neighbours_count);
      curr_pixel.rgbtGreen = round(total_green / (float)neighbours_count);
      image[h][w] = curr_pixel;
    }
  }
  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]) {
  const int GX_kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  const int GY_kernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  RGBTRIPLE backup_img[height][width];
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      backup_img[h][w] = image[h][w];
    }
  }

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      RGBTRIPLE neighbours[NEIGHBOUR_ROW][NEIGHBOUR_COL];
      get_edge_neighbours(h, w, height, width, neighbours, backup_img);

      RGBTRIPLE curr_pixel = image[h][w];
      int total_gx_red = 0, total_gx_blue = 0, total_gx_green = 0;
      int total_gy_red = 0, total_gy_blue = 0, total_gy_green = 0;

      for (int m = 0; m < NEIGHBOUR_ROW; m++) {
        for (int n = 0; n < NEIGHBOUR_COL; n++) {
          RGBTRIPLE pixel = neighbours[m][n];
          total_gx_red += pixel.rgbtRed * GX_kernel[m][n];
          total_gx_blue += pixel.rgbtBlue * GX_kernel[m][n];
          total_gx_green += pixel.rgbtGreen * GX_kernel[m][n];

          total_gy_red += pixel.rgbtRed * GY_kernel[m][n];
          total_gy_blue += pixel.rgbtBlue * GY_kernel[m][n];
          total_gy_green += pixel.rgbtGreen * GY_kernel[m][n];
        }
      }

      curr_pixel.rgbtRed =
          round_and_cap(sqrt(pow(total_gx_red, 2) + pow(total_gy_red, 2)));
      curr_pixel.rgbtBlue =
          round_and_cap(sqrt(pow(total_gx_blue, 2) + pow(total_gy_blue, 2)));
      curr_pixel.rgbtGreen =
          round_and_cap(sqrt(pow(total_gx_green, 2) + pow(total_gy_green, 2)));
      image[h][w] = curr_pixel;
    }
  }
  return;
}

int round_and_cap(double number) {
  if (number > 255.0)
    number = 255.0;
  else if (number < 0.0)
    number = 0.0;

  int result = round(number);
  return result;
}

// Returns length of neighbours surrounding a pixel.
int get_neighbours(int height, int width, int MAX_H, int MAX_W,
                   RGBTRIPLE neighbours[], RGBTRIPLE image[MAX_H][MAX_W]) {

  int neighbours_count = 0;
  // Since array start at 0 MAX_h will be 3 when maximum height is 2
  const int h_bound = MAX_H - 1, w_bound = MAX_W - 1;

  // up down case
  if (height != 0) {
    RGBTRIPLE up = image[height - 1][width];
    neighbours[neighbours_count++] = up;
  }
  if (height != h_bound) {
    RGBTRIPLE down = image[height + 1][width];
    neighbours[neighbours_count++] = down;
  }

  // Left right case
  if (width != 0) {
    RGBTRIPLE left = image[height][width - 1];
    neighbours[neighbours_count++] = left;
  }
  if (width != w_bound) {
    RGBTRIPLE right = image[height][width + 1];
    neighbours[neighbours_count++] = right;
  }

  // Diagnol case
  if (height != 0 && width != 0) {
    RGBTRIPLE ul = image[height - 1][width - 1];
    neighbours[neighbours_count++] = ul;
  }
  if (height != 0 && width != w_bound) {
    RGBTRIPLE ur = image[height - 1][width + 1];
    neighbours[neighbours_count++] = ur;
  }
  if (height != h_bound && width != 0) {
    RGBTRIPLE dl = image[height + 1][width - 1];
    neighbours[neighbours_count++] = dl;
  }
  if (height != h_bound && width != w_bound) {
    RGBTRIPLE dr = image[height + 1][width + 1];
    neighbours[neighbours_count++] = dr;
  }

  neighbours[neighbours_count++] = image[height][width];
  return neighbours_count;
}

// Returns length of neighbours surrounding a pixel.
void get_edge_neighbours(int height, int width, int MAX_H, int MAX_W,
                         RGBTRIPLE neighbours[NEIGHBOUR_ROW][NEIGHBOUR_COL],
                         RGBTRIPLE image[MAX_H][MAX_W]) {

  // Since array start at 0 MAX_h will be 3 when maximum height is 2
  const int h_bound = MAX_H - 1, w_bound = MAX_W - 1;
  // Beyond egdes and conrners, consider black pixels
  RGBTRIPLE black_pixel;
  black_pixel.rgbtBlue = 0, black_pixel.rgbtGreen = 0, black_pixel.rgbtRed = 0;

  RGBTRIPLE up = black_pixel, down = black_pixel, right = black_pixel,
            left = black_pixel, ur = black_pixel, ul = black_pixel,
            dl = black_pixel, dr = black_pixel;

  if (height != 0 && width != 0)
    ul = image[height - 1][width - 1];

  if (height != 0)
    up = image[height - 1][width];

  if (height != 0 && width != w_bound)
    ur = image[height - 1][width + 1];

  neighbours[0][0] = ul;
  neighbours[0][1] = up;
  neighbours[0][2] = ur;

  // Left right case
  if (width != 0)
    left = image[height][width - 1];
  if (width != w_bound)
    right = image[height][width + 1];

  neighbours[1][0] = left;
  neighbours[1][1] = image[height][width];
  neighbours[1][2] = right;

  // Diagnol case
  if (height != h_bound && width != 0)
    dl = image[height + 1][width - 1];
  if (height != h_bound)
    down = image[height + 1][width];
  if (height != h_bound && width != w_bound)
    dr = image[height + 1][width + 1];

  neighbours[2][0] = dl;
  neighbours[2][1] = down;
  neighbours[2][2] = dr;
}
