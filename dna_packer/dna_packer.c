#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int safe_gets(FILE* f,char** res)
{
  if (f == NULL)
    perror("ошибка открытия");
  if (feof(f))
    return 1; 
  int size = 1;
  char* buf = NULL;
  buf = malloc(110);
  if (*res != NULL)
      free(*res);
  *res = malloc(1);
  char* ptr = NULL; 
  do
  {
    if (fgets(buf, 100, f) == NULL)
    {
      free(buf);
      return 2;
    }
    ptr = realloc(*res, size + 99);
    if (ptr == NULL)
    {
      free(buf);
      free(*res);
      return 3;
    }
    *res = ptr;
    strcpy(*res + size - 1, buf); 
    size += 99;
  } while (!feof(f) && strlen(buf) == 99);
  free(buf);
  return 0;
}

int unpacker(FILE *inf, FILE *outf)
{
  int err;
  char *str = NULL;
  unsigned int buf = 0;
  int flag;
  int pos, groupw, writen;
  size_t read;
  while(!feof(inf))
  { 
    groupw = 0;
    writen = 0;
    err = safe_gets(inf, &str);
    if (err != 0)
        return err;
    if (str[0] != '>' )
        return 4;
    fputs(str, outf);
    flag = 1;
    while(flag)
    {
      read = fread(&buf, sizeof(int), 1, inf);
      pos = 0;
      while (pos <= (sizeof(int) *  8) - 3)
      { 
        switch ((buf >> pos) & 7)
        {
          case 1: fputc('a',outf);
                  break;
          case 2: fputc('t',outf);
                  break; 
          case 3: fputc('g',outf);
                  break;
          case 4: fputc('c',outf);
                  break;
          case 5: fputc('u',outf);
                  break;
          case 0: fputc('\n',outf);
                  pos = sizeof(int) * 8;
                  flag = 0;
                  continue;
                  break;
        }
        pos += 3;
        ++writen;
        if (writen == 10)
        {
          ++groupw;
          if (groupw == 6)
          {
              fputc('\n',outf);
              groupw = 0;
          }
          else 
              fputc(' ',outf);
          writen = 0;
        }
      }
    }
    fgetc(inf);
  }
  free(str);
  fclose(inf);
  fclose(outf);
  printf("unpacked\n");
}

int packer(FILE *inf, FILE *outf)
{
  unsigned int buf = 0;
  int pos = 0;
  int err;
  char c;
  char* str = NULL;
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
    c = tolower(c);
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
    if (pos > (sizeof(int) * 8) - 3)
    {
      fwrite((const void* ) &buf, sizeof(int), 1, outf);
      pos = 0;
      buf = 0;
    }
  }
  fwrite((const void* ) &buf, sizeof(int), 1, outf);
  free(str);
  fclose(inf);
  fclose(outf);
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
     fclose(fin);
     printf("Error opening output file");
     return 0;
  } 
  if (argv[1][3] == 'k')
  {
      printf("packing\n");
      return packer(fin, fout);
  }
  else 
  {
      printf("unpacking\n");
      return unpacker(fin, fout);
  }
      fclose(fin);
      fclose(fout);
      return 0;
}
