#include <stdio.h>
#include "list.h"

int main()
{
    List *l = NULL;
    lpush(&l,1);
    lpush(&l,2);
    lpush(&l,3);
    lpush(&l,4);
    lprint(l);
    printf("will delete 0s symbol\n");
    ldelete(&l,0);
    lprint(l);
    printf("will insertn 10 in 0s pos\n");
    linsert(&l,0, 10);
    lprint(l);
    printf("will pop\n");
    printf("%i\n",lpop(&l));
    printf("will search for 2\n");
    printf("%i\n",lsearch(l,2));
    lclear(&l);
    
    return 0;
}
