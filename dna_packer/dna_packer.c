#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int safe_gets(FILE* f,char** res)
{
  if (f == NULL)
    perror("ошибка открытия");
  if (feof(f))
    return 1; 
  int size = 1;
  char* buf;
  buf = malloc(110);
  *res = malloc(1);
  char* ptr; 
  do
  {
    if (fgets(buf, 100, f) == NULL)
      return 2;
    ptr = realloc(*res, size + 99);
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

int unpacker(FILE *inf, FILE *outf)
{
}

int packer(FILE *inf, FILE *outf)
{
  unsigned int buf = 0;
  int pos = 0;
  int err;
  char c;
  char* str;
  c = fgetc(inf);
  if (c == '>') 
  {
    err = safe_gets(inf, &str);
    if (err != 0)
      return err;
    fputc('>', outf);
    fputs(str, outf);
  }
  else 
    return 4;
  while (!feof(inf))
  {
    c = fgetc(inf);
    if (c == '>')
    {   
      fwrite((const void* ) &buf, sizeof(int), 1, outf);
      fprintf(outf,"\n");
      pos = 0;
      buf = 0;
      err = safe_gets(inf, &str);
      if (err != 0)
        return err;
      fputc('>', outf);
      fputs(str, outf);
      continue;
    }
    switch(c)
    {
      case 'a' :buf += 1 << pos;
		pos += 3;
                break;
      case 't' :buf += 2 << pos;
		pos += 3;
                break;
      case 'g' :buf += 3 << pos;
		pos += 3;
                break;
      case 'c' :buf += 4 << pos;
		pos += 3;
                break;
      case 'u' :buf += 5 << pos;
		pos += 3;
                break;
    }
    if (pos >= (sizeof(int) * 8))
    {
      fwrite((const void* ) &buf, sizeof(int), 1, outf);
      pos = 0;
      buf = 0;
    }
  }
  fwrite((const void* ) &buf, sizeof(int), 1, outf);
  printf("packed\n");
}


int main(int argc, char *argv[])
{ 
  if (argc < 3)
  { 
      printf("few arguments");
      return 0;
  }
  FILE *fin = fopen(argv[2], "r");
  if (fin == NULL)
  {
      printf("Error opening input file\n");
      return 0;
  }
  FILE *fout = fopen(argv[3], "w");
  if (fout == NULL)
  {
     printf("Error opening output file");
     return 0;
  } 
  if (argv[1][3] == 'k')
  {
      printf("packingg\n");
      return packer(fin, fout);
  }
  else 
  {
      printf("unpacking\n");
      return unpacker(fin, fout);
  }
      return 0;
}
