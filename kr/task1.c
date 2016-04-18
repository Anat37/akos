#include<stdio.h>

int transform(unsigned int *a)
{
    int i = 0;
    unsigned int cnt = 0;
    unsigned int base = 0;
    for (i; i < 8; ++i)
    {
        unsigned int left = 0;
        unsigned int right = 0;
        left = *a & (1 << i);
        if (left) ++cnt;
        *a -= left;
        left = left << (15 - 2*i);
        right = *a &(1 << (15 - i));
        *a -= right;
        if (right) ++cnt;
        right = right >> (15 - 2*i);
        *a += right + left;
    }
    base -= 1 + (63 << 24);
    *a = *a & base;
    *a += cnt << 24;
    return 0;
}

int main()
{
    unsigned int num;
    scanf("%u", &num);
    transform(&num);
    printf("%u\n", num);
    return 0;
}
