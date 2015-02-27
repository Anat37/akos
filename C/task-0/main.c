#include <stdio.h>

#define SIZE 64

int main()
{
    unsigned long long int num; /* ansi выдает warning на long long */
    char bin_nums[SIZE];
    int i = 0;
    short mul = 8;
    short num_to_hex = 0;
    scanf("%llu",&num);

    while (num)
    {
        bin_nums[i] = num%2;
        num>>=1;
        i++;
    }

    while (i<SIZE)
    {
        bin_nums[i] = 0;
        i++;
    }

    for(i = SIZE-1; i>=0; i--)
    {
        printf("%i",bin_nums[i]);
        if(i%4==0 && i!=0)
            printf(" ");
        if (i%8==0 && i!=0)
            printf("| ");
    }
    printf("\n");

    for(i = SIZE-1; i>=0; i--)
    {
        num_to_hex += mul*bin_nums[i];
        mul >>= 1;

        if (i%4==0)
        {
            printf("%4hX ",num_to_hex);
            num_to_hex = 0;
            mul = 8;
            if (i%8==0)
                printf("  ");
        }
    }
    printf("\n");

    return 0;
}
