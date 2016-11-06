#include<stdio.h>

int main()
{
    FILE* f = fopen("input2","w");
    int i;
    for (i = 0; i < 100; ++i)
        fwrite((void*)(&i), sizeof(float), 1, f);
    return 0;
}
