#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int safe_gets(FILE* f,char** res)
{
  if (f == NULL)
    return 1;
  if (feof(f))
    return 1; 
  int size = 1;
  char* buf;
  buf = malloc(110 * sizeof(char));
  if (*res != NULL)
      free(*res);
  *res = malloc(sizeof(char));
  char* ptr; 
  do
  {
    if (fgets(buf, 100, f) == NULL)
      return 2;
    ptr = realloc(*res, (size + 99) * sizeof(char));
    if (ptr == NULL)
    {
      free(*res);
      return 3;
    }
    *res = ptr;
    strcpy(*res + size - 1, buf); 
    size += 99;
  } while (!feof(f) && strlen(buf) == 99);
  return 0;
}
