#include<memory.h>

typedef char Pair[2][255];

struct list
{
    char str[255];
    struct list* ptr;
};

int transform(struct list* listp, Pair* pairp)
{
    int cnt = 0;
    int i = 0;
    struct list* prev = NULL;
    for (i; i < 255; ++i)
        if (*pairp[0][i] == ' ')
            ++cnt;
    while (listp != NULL)
    {
        int loccnt = 0;
        for (i = 0; i < 255; ++i)
            if ((*listp).str[i] == ' ') 
                ++loccnt;
        if (loccnt <= cnt)
        {
            struct list* nw = (struct list*) malloc(sizeof(struct list));
            if (prev != NULL)
                (*prev).ptr = nw;
            (*nw).ptr = listp;
            for (i = 0; i < 255; ++i)
               (*nw).str[i] = (*pairp)[1][i]; 
            nw = (struct list*) malloc(sizeof(struct list));
            (*nw).ptr = (*listp).ptr;
            (*listp).ptr = nw;
            for (i = 0; i < 255; ++i)
                (*nw).str[i] = (*pairp)[0][i];
            return 0;
        }
        prev = listp;
        listp = (*listp).ptr;
    }
    return 0;
}


int main()
{
    return 0;
}
