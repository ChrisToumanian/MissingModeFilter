#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.c"

#define LROWS 1001
#define NCOLUMNS 30

typedef struct
{
  bool fitted;
  bool missing;
  bool toBeFitted;
} Mode;

Mode modes[LROWS][NCOLUMNS];
bool exportImageOption;
char *bitmapFileName;

void addModesFromFile(char *file, bool fitted, bool missing, bool hasRedundantValues);
void findModesToBeFitted();
bool isFittable(int l, int n);
int primeModeSearch(int l, int n, int depth);
void printModesToBeFitted();
void printAllModes();
void exportImage();

int main(int argc, char *argv[])
{
  char *fittedModesFile = argv[1];
  char *missingModesFile = argv[2];
  char *modesToBeFitted = "";
  int commandCount = 0;
  int i;

  // commands
  while (argv[++commandCount] != NULL);
  for (i = 0; i < commandCount; i++)
  {
    if (strcmp(argv[i], "-i") == 0) // export image
    {
      exportImageOption = true;
      bitmapFileName = argv[i + 1];
    }
  }
  
  // adds fitted and missing modes from files to modes array
  addModesFromFile(fittedModesFile, true, false, false);
  addModesFromFile(missingModesFile, false, true, true);

  // finds modes to be fit and marks them in modes array
  findModesToBeFitted();

  // prints fittable modes
  printModesToBeFitted();

  // exports image if option is set
  if (exportImageOption) exportImage();
  
  return 0;
}

void addModesFromFile(char *file, bool fitted, bool missing, bool hasRedundantValues)
{
  char line[100];
  char *l;
  char *n;
  FILE *fptr;

  // opens file and checks if it exists
  if ((fptr = fopen(file, "r")) == NULL)
  {
    printf("%s%s\n", "File does not exist: ", file);
    return;
  }

  // loops through file by newlines
  if (!hasRedundantValues)
  {
    while (fgets(line, sizeof(line), fptr))
    {
      l = strtok(line, " "); // sets l value
      n = strtok(NULL, " "); // sets n value
      modes[atoi(l)][atoi(n)].fitted = fitted;
      modes[atoi(l)][atoi(n)].missing = missing;
    }
  }
  else // has redundant values
  {
    while (fgets(line, sizeof(line), fptr))
    {
      l = strtok(line, " "); // sets l value
      l = strtok(NULL, " "); // sets 2nd l value
      n = strtok(NULL, " "); // sets n value
      modes[atoi(l)][atoi(n)].fitted = fitted;
      modes[atoi(l)][atoi(n)].missing = missing;
    }
  }

  // closes file
  fclose(fptr);
}

void findModesToBeFitted()
{
  // iterates through all missing modes
  int l, n;
  for (l = 0; l < LROWS; l++)
  {
    for (n = 0; n < NCOLUMNS; n++)
    {
      if (modes[l][n].missing)
      {
        modes[l][n].toBeFitted = isFittable(l, n);
      }
    }
  }
}

bool isFittable(int l, int n)
{
  int fittedModesFound = 0;

  // checks if adjacent modes have been fitted
  if (modes[l][n + 1].fitted) fittedModesFound++; // right
  if (modes[l][n - 1].fitted) fittedModesFound++; // left
  if (modes[l - 1][n].fitted) fittedModesFound++; // top
  if (modes[l + 1][n].fitted) fittedModesFound++; // bottom

  // if three or more fitted modes are found adjacent, sets missing mode to be fitted
  if (fittedModesFound >= 3)
  {
    return true;
  }
  else
  {
    int deepFittedModesFound = primeModeSearch(l, n, 1100);
    if (deepFittedModesFound >= 3)
      return true;
  }

  return false;
}

int primeModeSearch(int l, int n, int depth)
{
  bool foundUp = false;
  bool foundDown = false;
  bool foundLeft = false;
  bool foundRight = false;
  int found = 0;
  int i;

  // searches for modes in 4 directions across 2D array and marks the directions found as true
  /* for (i = 0; i < depth; i++)
  {
    if (n + i <= NCOLUMNS && modes[l][n + i].fitted) foundRight = true; // right
    if (n - i >= 0 && modes[l][n - i].fitted) foundLeft = true; // left
    if (l - i >= 0 && modes[l - i][n].fitted) foundUp = true; // up
    if (l + i <= LROWS && modes[l + i][n].fitted) foundDown = true; // down
  } */

  // search up
  for (i = 0; i < depth; i++)
  {
    if (l - i >= 0 && modes[l - i][n].fitted)
    {
      foundUp = true;
      break;
    }
  }

  // search down
  for (i = 0; i < depth; i++)
  {
    if (l + i <= LROWS && modes[l + i][n].fitted)
    {
      foundDown = true;
      break;
    }
  }

  // search left
  for (i = 0; i < depth; i++)
  {
    if (n - i >= 0 && modes[l][n - i].fitted)
    {
      foundLeft = true;
      break;
    }
  }

  // search right
  for (i = 0; i < depth; i++)
  {
    if (n + i <= NCOLUMNS && modes[l][n + i].fitted)
    {
      foundRight = true;
      break;
    }
  }
  
  // sets number of found modes
  if (foundUp) found++;
  if (foundDown) found++;
  if (foundLeft) found++;
  if (foundRight) found++;
  
  return found;
}

void printModesToBeFitted()
{
  int l, n;
  for (l = 0; l < LROWS; l++)
  {
    for (n = 0; n < NCOLUMNS; n++)
    {
      if (modes[l][n].toBeFitted)
      {
        printf("%d%s%d\n", l, " ", n);
      }
    }
  }
}

void printAllModes()
{
  int l, n;
  for (l = 0; l < LROWS; l++)
  {
    for (n = 0; n < NCOLUMNS; n++)
    {
      if (modes[l][n].fitted)
      {
        printf("1 ");
      }
      else if (modes[l][n].toBeFitted)
      {
        printf("2 ");
      }
      else if (modes[l][n].missing)
      {
        printf("3 ");
      }
      else
      {
        printf("0 ");
      }
    }
    printf("\n");
  }
}

void exportImage()
{
  unsigned char fittedColor = 255;
  unsigned char toBeFittedColor = 175;
  unsigned char missingColor = 75;
  unsigned char nullColor = 0;
  unsigned char image[LROWS][NCOLUMNS];
  int l, n;
  
  // Set each pixel's value
  for (l = 0; l < LROWS; l++)
  {
    for (n = 0; n < NCOLUMNS; n++)
    {
      if (modes[l][n].toBeFitted)
      {
	image[l][n] = toBeFittedColor;
      }
      else if (modes[l][n].fitted)
      {
	image[l][n] = fittedColor;
      }
      else if (modes[l][n].missing)
      {
	image[l][n] = missingColor;
      }
      else
      {
	image[l][n] = nullColor;
      }
    }
  }

  // use bitmap.c to save image
  saveImageBitmap(bitmapFileName, LROWS, NCOLUMNS, image);
}
