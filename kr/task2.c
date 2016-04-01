#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int safe_gets(FILE* f,char** res)
{
    char* ptr;
    int size = 1;
    char* buf;
    if (f == NULL)
        return 1;
    if (feof(f))
        return 1;
    buf = malloc(110);
    if (*res != NULL)
      free(*res);
    *res = malloc(1);
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

int transform(char *str)
{
    int cnt = 0;
    int sum = 0;
    int i = 0;
    int ptr = 0;
    while (*(str + i) != '\0')
    {
        char ch = *(str + i);
        if (ch <= '9' && ch >= '0')
        {
            if (cnt == 0) 
                ptr = i;
            ++cnt;
            sum += ch - '0';
        } else if (cnt > 0)
        {
            int j = ptr;
            sum = (cnt < sum) ? cnt : sum;
            for (j; j < ptr + sum; ++j)
                *(str + j) = '*';
            if (j != i)
            {
                ptr = i;
                i = j;
                while (*(str + ptr) != '\0')
                   *(str + j++) = *(str + ptr++);
                *(str + j) = '\0'; 
            }
            ptr = 0;
            cnt = 0;
            sum = 0;
        }
        ++i;
    }
    if (cnt != 0) 
    {
        int j = ptr;
        sum = (cnt < sum) ? cnt : sum;
        for (j; j < ptr + sum; ++j)
            *(str + j) = '*';
        *(str + j) = '\0';
        i = j;
    }
    str = realloc(str, i + 1);/* тут строка только обрезается, поэтому не будем считать это выделением памяти) Иначе функции вывода будут выводить символы в строке до последнего /0 в ней, а нам этого не хочется*/
}
int main(int argc, char* argv[])
{
    FILE *fptr = fopen(argv[1], "r");
    char* str = NULL;

    if (argc < 1) 
    {
        perror("Не введен файл");
        return 1;
    }
    
    while (!feof(fptr))
    {   
        if (safe_gets(fptr, &str) != 0)
            continue;
        transform(str);
        printf("%s", str);
        free(str);
    }
    fclose(fptr);
    return 0;
}
