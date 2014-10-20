#include <stdio.h>


int my_getc(FILE *in)
{
    char c;
    if(!fread(&c,sizeof(char),1,in))
    {
        return EOF;
    }
    return c;
}

int main(int argv,char** argc)
{
    FILE *in = fopen(argc[1],"rb");
    FILE *out = fopen(argc[2],"wb");

    char c;
    while( (c=my_getc(in)) != EOF)
    {
        fwrite(&c,sizeof(char),1,out);
    }
    
    fclose(in);
    fclose(out);
    return 0;
}
