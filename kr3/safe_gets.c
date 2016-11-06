#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int safe_gets(FILE* f,char** res)
{
  int size = 1 + sizeof(long);
  char* buf;
  char* ptr; 
  if (f == NULL)
    return 1;
  if (feof(f))
    return 1; 
  buf = malloc(110 * sizeof(char));
  if (*res != NULL)
  {
      free(*res);
      *res = NULL;
  }
  *res = malloc(sizeof(char) + sizeof(long));
  do
  {
    if (fgets(buf, 100, f) == NULL)
    {
      free(buf);
      free(*res);
      return 2;
    }
    ptr = realloc(*res, (size + 99) * sizeof(char) + sizeof(long));
    if (ptr == NULL)
    {
      free(*res);
      free(buf);
      return 3;
    }
    *res = ptr;
    strcpy(*res + size - 1 + sizeof(long), buf); 
    size += 99;
  } while (!feof(f) && strlen(buf) == 99);
  free(buf);
  return 0;
}
