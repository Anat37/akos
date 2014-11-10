#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main()
{
    List l;
    init(&l);
    char* command = (char*)malloc(10*sizeof(char));
    int data,pos;
    while(1)
    {
        printf("command > ");
        scanf("%s",command);
        
        if(!strcmp(command,"exit"))
        {
            break;
        }
        
        if(!strcmp(command,"push"))
        {
            printf(" data > ");
            scanf("%i",&data);
            lpush(&l,data);
        }
        
        if(!strcmp(command,"pop"))
        {
            if (l.len!=0)
            {
                printf("%i\n",lpop(&l));
            }else
            {
                printf("Empty\n");
            }
        }

        if(!strcmp(command,"insert"))
        {
            printf(" data > ");
            scanf("%i",&data);
            printf(" position > ");
            scanf("%i",&pos);
            linsert(&l, pos, data);
        }
        
        if(!strcmp(command,"delete"))
        {
            printf(" position > ");
            scanf("%i",&pos);
            ldelete(&l, pos);
        }
        
        if(!strcmp(command,"search"))
        {
            printf(" data > ");
            scanf("%i",&data);
            printf("%i",lsearch(l,data));
        }
    }
    lclear(&l);
    free(command);
    return 0;
}
