#include <stdio.h>
#include "list.h"

int main()
{
    list *l = NULL;
    lpush(&l,1);
    lpush(&l,2);
    lpush(&l,3);
    lpush(&l,4);
    lprint(l);
    printf("\n");
    ldelete(&l,0);
    lprint(l);
    printf("\n");
    linsert(&l,0, 10);
    lprint(l);
    printf("\n");
    printf("%i\n",lpop(&l));
    printf("\n");
    printf("%i\n",lsearch(l,2));
    lclear(&l);
    return 0;
}
