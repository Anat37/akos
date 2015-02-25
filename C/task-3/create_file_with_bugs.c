#include <stdio.h>
#include <unistd.h>


int main()
{
    FILE *fp = fopen("input_data_with_bugs.txt","w");
    fprintf(fp,"try to read me!");
    ftruncate(fileno(fp), 1000000);

    fclose(fp);
    return 0;

}

