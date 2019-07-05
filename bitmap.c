#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveImageBitmap(char* filename, int w, int h, unsigned char data[w][h])
{
  FILE *f;
  unsigned char *image = NULL;
  int fileSize = 54 + 3 * w * h;
  int i;

  // bitmap header values
  unsigned char fileHeader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
  unsigned char infoHeader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
  unsigned char pad[3] = {0, 0, 0};
  
  // allocates memory for image
  image = (unsigned char*)malloc(3 * w * h);
  memset(image, 0, 3 * w * h);

  // create file header
  fileHeader[2] = (unsigned char)(fileSize);
  fileHeader[3] = (unsigned char)(fileSize >> 8);
  fileHeader[4] = (unsigned char)(fileSize >> 16);
  fileHeader[5] = (unsigned char)(fileSize >> 24);

  // create info header
  infoHeader[4] = (unsigned char)(w);
  infoHeader[5] = (unsigned char)(w >> 8);
  infoHeader[6] = (unsigned char)(w >> 16);
  infoHeader[7] = (unsigned char)(w >> 24);
  infoHeader[8] = (unsigned char)(h);
  infoHeader[9] = (unsigned char)(h >> 8);
  infoHeader[10] = (unsigned char)(h >> 16);
  infoHeader[11] = (unsigned char)(h >> 24);

  // sets single value to RGB (black & white) for each element of image
  for (i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      int x = i;
      int y = j;
      int r = data[i][j];
      int g = data[i][j];
      int b = data[i][j];

      image[(x + y * w) * 3 + 2] = (unsigned char)(r);
      image[(x + y * w) * 3 + 1] = (unsigned char)(g);
      image[(x + y * w) * 3 + 0] = (unsigned char)(b);
    }
  }
  
  // open or create file and write header
  f = fopen(filename, "wb");
  fwrite(fileHeader, 1, 14, f);
  fwrite(infoHeader, 1, 40, f);

  // write image data to file
  for (i = 0; i < h; i++)
  {
    fwrite(image + (w * (h - i - 1) * 3), 3, w, f);
    fwrite(pad, 1, (4 - (w * 3) % 4 ) % 4, f);
  }

  // deallocate image memory and close file
  free(image);
  fclose(f);
}
