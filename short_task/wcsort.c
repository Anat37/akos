#include <stdlib.h>
#include <stdio.h>

volatile long words = 0;

int safe_gets(FILE* f,char** res)
{
  int size = 30;
  char* buf = NULL;
  char* ptr = NULL;
  char c = EOF; 
  int i = 0;
  int wflag = 0;
  int flag = 1;
  if (f == NULL)
    return 1;
  if (feof(f))
    return 1; 
  
  buf = malloc(size * sizeof(char));
  if (*res != NULL)
  {
      free(*res);
      *res = NULL;
  }
  while (!feof(f) && flag == 1)
  {
    c = fgetc(f);
    if (c == EOF){
        flag = 2;
    }
    if (c != ' ')
        wflag = 1;
    if (c == ' ')
    {
        if (wflag)
            ++words;
        wflag = 0;
    }
    if (i == size){
        size += 10;
        ptr = realloc(buf,  size * sizeof(char));
        if (ptr == NULL)
        {
            free(buf);
            return 3;
        }
        buf = ptr;
    }
    buf[i] = c;
    ++i;
    if (c == '\n') 
        flag = 0;
  }
  if (wflag)
  	++words;
  *res = buf;
  buf = NULL;
  ptr = NULL;
  return flag;
}

int mycmp(const char* a, const char* b)
{
    int i = 0;
    while (a[i] == b[i] && a[i] != '\n')
    ++i;
    if (a[i] == b[i])
        return 0;
    if (a[i] == '\n')
        return -1;
    if (b[i] == '\n')
        return 1;
    if (a[i] > b[i])
        return 1;
    else return -1;    
}

void myputs(FILE* f, char* str)
{
	int i = 0;
	if (str == NULL)
		return;
	while (str[i] != '\n' && str[i] != EOF)
	{
		fprintf(f, "%c", str[i]);
		++i;
	}
	fprintf(f, "\n");
}

int main(int argc, char* argv[])
{
    char** data = NULL;
    char* buf;
    int strcnt = 0;
    size_t strsize = 10;
    int i, j;
    FILE* inf;
    FILE* outf;
    int ret = 0;
    if (argc < 3)
    {
        printf("Few args");
        return 0;
    }
    inf = fopen(argv[1], "r");
    outf = fopen(argv[2], "w");
    if (outf == NULL || inf == NULL)
    {
        printf("Invalid file names");
        return 0;
    }
    data = malloc(10 * sizeof(char*));
    data[strcnt] = NULL;
    ret = safe_gets(inf, &data[strcnt]);
    while (ret == 0)
    {
        strcnt++;
        if (strcnt + 1 == strsize)
        {   
            strsize += 10;
            data = realloc(data, strsize * sizeof(char*));
            if (data == NULL)
            printf("whats up?");
        }
        /*printf("%s", data[strcnt - 1]);*/
        data[strcnt] = NULL;
        ret = safe_gets(inf, &data[strcnt]);
        
    }
    
    for (i = 0; i < strcnt; ++i)
        for (j = i + 1; j < strcnt; ++j)
        if (mycmp(data[i], data[j]) >= 0) {
            buf = data[j];
            data[j] = data[i];
            data[i] = buf;
        }
    printf("Words: %ld\n", words);
    for (i = 0; i < strcnt; ++i){
        myputs(outf, data[i]);
        free(data[i]);
    } 
    free(data[i]);   
    free(data);
    fclose(inf);
    fclose(outf);    
    return 0;    
}
