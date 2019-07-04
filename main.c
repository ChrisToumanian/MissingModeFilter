#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LROWS 1000
#define NCOLUMNS 29

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
  //printAllModes();
  printModesToBeFitted();

  return 0;
}

void defaultModesArray()
{
  // iterates through modes and sets all mode values to false
  for (int l = 0; l < LROWS; l++)
  {
    for (int n = 0; n < NCOLUMNS; n++)
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
  for (int l = 0; l < LROWS; l++)
  {
    for (int n = 0; n < NCOLUMNS; n++)
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
  if (modes[l][n + 1].fitted) fittedModesFound++; // top
  if (modes[l][n - 1].fitted) fittedModesFound++; // bottom
  if (modes[l - 1][n + 1].fitted) fittedModesFound++; // left
  if (modes[l + 1][n - 1].fitted) fittedModesFound++; // right
  
  // if three or more fitted modes are found adjacent, set missing mode to be fitted
  if (fittedModesFound >= 3)
  {
    return true;
  }

  return false;
}

void printModesToBeFitted()
{
  for (int l = 0; l < LROWS; l++)
  {
    for (int n = 0; n < NCOLUMNS; n++)
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
  for (int l = 0; l < LROWS; l++)
  {
    for (int n = 0; n < NCOLUMNS; n++)
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