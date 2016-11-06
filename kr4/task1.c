#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<threads.h>


float get(FILE* f, int i)
{
    float ans;
    fseek(f, i, SEEK_SET);
    fread((void *)(&ans), sizeof(float), 1, f);
    return ans;
}

int swapf(FILE* f, int first, int second, float fi, float sec)
{
    fseek(f, first, SEEK_SET);
    fwrite((void *)(&sec), sizeof(float), 1 ,f);
    fseek(f, second, SEEK_SET);
    fwrite((void *)(&fi), sizeof(float), 1, f);
    return 0;
}

int main(int argc, char* argv[])
{
    FILE* fptr = open(argv[1], "rw");
    char* buf = NULL;
    

    buf = malloc(sizeof(char) * strlen("Vectors"));
    fread(buf, strlen("Vectors") + 1, 1, fptr);
    
    fread(buf, sizeof(int), 1, fptr);
    
    
    stat(argv[1], &buf);
    size = buf.st_size / sizeof(float);
    
    for (i = 0; i < size; ++i)
    { 
       a = get(fptr, i); 
       for (j = i; j < size; ++j)
        {
            b = get(fptr, j);
            if (b > a)
            {
                swapf(fptr, i, j, a, b);
                a = b;
            }
        }        
    }
    fclose(fptr);
    return 0;
}