#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LROWS 1001
#define NCOLUMNS 30

typedef struct
{
  bool fitted;
  bool missing;
  bool toBeFitted;
} Mode;

Mode modes[LROWS][NCOLUMNS];

void defaultModesArray();
void addModesFromFile(char *file, bool fitted, bool missing, bool hasRedundantValues);
void findModesToBeFitted();
bool isFittable(int l, int n);
int primeModeSearch(int l, int n, int depth);
void printModesToBeFitted();
void printAllModes();

int main(int argc, char *argv[])
{
  char *fittedModesFile = argv[1];
  char *missingModesFile = argv[2];
  char *modesToBeFitted = "";

  // sets all modes in array to not fitted and not missing
  defaultModesArray();

  // adds fitted and missing modes from files to modes array
  addModesFromFile(fittedModesFile, true, false, false);
  addModesFromFile(missingModesFile, false, true, true);

  // find modes to be fit and marks them in modes array
  findModesToBeFitted();

  // print fittable modes
  printModesToBeFitted();

  return 0;
}

void defaultModesArray()
{
  // iterates through modes and sets all mode values to false
  int l, n;
  for (l = 0; l < LROWS; l++)
  {
    for (n = 0; n < NCOLUMNS; n++)
    {
      modes[l][n].fitted = false;
      modes[l][n].missing = false;
      modes[l][n].toBeFitted = false;
    }
  }
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

  // close file
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

  // check if adjacent modes has been fitted
  if (modes[l][n + 1].fitted) fittedModesFound++; // right
  if (modes[l][n - 1].fitted) fittedModesFound++; // left
  if (modes[l - 1][n].fitted) fittedModesFound++; // top
  if (modes[l + 1][n].fitted) fittedModesFound++; // bottom

  // if three or more fitted modes are found adjacent, set missing mode to be fitted
  if (fittedModesFound >= 3)
  {
    return true;
  }
  else
  {
    int deepFittedModesFound = primeModeSearch(l, n, 1100);
    if (deepFittedModesFound >= 3)
      //printf("%d\n", deepFittedModesFound);
      return true;
  }

  return false;
}

int primeModeSearch(int l, int n, int depth)
{
  bool foundTop = false;
  bool foundBottom = false;
  bool foundLeft = false;
  bool foundRight = false;
  int found = 0;
  int i;
  
  for (i = 0; i < depth; i++)
  {
    if (n + i <= NCOLUMNS && modes[l][n + i].fitted) foundRight = true; // right
    if (n - i >= 0 && modes[l][n - i].fitted) foundLeft = true; // left
    if (l - i >= 0 && modes[l - i][n].fitted) foundTop = true; // top
    if (l + i <= LROWS + 1 && modes[l + i][n].fitted) foundBottom = true; // bottom
  }

  if (foundTop) found++;
  if (foundBottom) found++;
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
