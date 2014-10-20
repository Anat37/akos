#include <stdio.h>

int main(int argv,char** argc)
{
    FILE *in = fopen(argc[1],"rb");
    FILE *out = fopen(argc[2],"wb");

    char c;
    while( fread(&c,sizeof(char),1,in))
    {
        fwrite(&c,sizeof(char),1,out);
    }
    
    fclose(in);
    fclose(out);
    return 0;
}
