#include <stdio.h>

int main()
{
    FILE *fp = fopen("input_data.txt","w");
    unsigned short i = 1;
    while (i!=0)
    {
        fprintf(fp,"%i\n",i);
        i++;
    }

    fclose(fp);

    return 0;
}
