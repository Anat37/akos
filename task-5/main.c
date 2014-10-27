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
    return 0;
}
