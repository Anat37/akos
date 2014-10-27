#include <stdio.h>
#include <stdlib.h>

typedef int Tdata;
typedef struct Node list;

#define MEMORY_ERR 1
#define END_OF_LIST 2
#define SUCCESS 0

struct Node
{
    list *next;
    Tdata data;
};

int lpush(list** l, Tdata data)
{
    list *tmp = NULL;
    tmp = (list*)malloc(sizeof(list));
    if (tmp == NULL)
    {
        return MEMORY_ERR;
    }
    tmp->next = NULL;
    tmp->data = data;
    if (*l == NULL)
    {
        *l = tmp;
    }else{
        list* i = *l;
        while (i->next!=NULL)
            i = i->next;
        i->next = tmp;
    }
    return SUCCESS;
}

int lpop(list **l)
{
    list *tmp = *l;
    Tdata t;
    if(tmp->next == NULL)
    {
        Tdata t = tmp->data;
        free(tmp);
        *l = NULL;
        return t;
    }
    
    while(tmp->next->next != NULL)
    {
       tmp = tmp->next; 
    }
    t = tmp->next->data;
    free(tmp->next);
    tmp->next = NULL;
    return t;
}

int linsert(list **l, int n, int data)
{
    list *new = (list*)malloc(sizeof(list));
    int i = 1;
    list *tmp;
    if (new == NULL)
    {
        return MEMORY_ERR;
    }

    if (n==0)
    {
        new -> next = *l;
        new -> data = data;
        *l = new;
    }

    tmp = *l;
    while(1)
    {
        if(tmp==NULL)
        {
            return END_OF_LIST;
        }
        if(i==n)
        {
            break;
        }
        tmp = tmp->next;
        i++;
    }

    new -> next = tmp->next;
    new -> data = data;
    tmp -> next = new;

    return 0;
}

int ldelete(list** l, int n)
{
    list *tmp = *l;
    list *next = NULL;
    int i = 0;
    if(tmp == NULL)  
    {
        return END_OF_LIST;
    }
    if(n==0)
    {
        next = (*l) -> next; 
        free(*l);
        *l = next;
        return SUCCESS;
    }
    while(1)
    {
        if(tmp -> next == NULL)
        {
            return END_OF_LIST;
        }
        if(i==n-1)
        {
            break;
        }
        tmp = tmp -> next;
        i++;
    }
    next = tmp->next->next;
    free(tmp->next);
    tmp->next = next;
    return SUCCESS;
}

int lsearch(list *l, Tdata key)
{
    int i = 0;
    list *tmp = l;
    
    if(tmp == NULL)
        return -1;
    
    while((tmp!= NULL)&&(tmp->data != key))
    {
        i++;
        tmp = tmp->next;
    }
    if(tmp==NULL)
    {
        return -1;
    }else{
        return i;
    }
}

void lclear(list **l)
{
    list *tmp = *l;
    while ((*l) != NULL)
    {
        tmp = (*l)->next;
        free(*l);
        *l = tmp;
    }
    *l = NULL;
}

void lprint(list *l)
{
    list *tmp = l;
    if (tmp == NULL)
    {
        printf("Empty");
        return;
    }

    while(tmp != NULL)
    {
        printf("%i \n", tmp->data);
        tmp = tmp->next;
    }
    return;
}
    

/*
int main()
{
    list *l = NULL;
    lpush(&l,1);
    lpush(&l,2);
    lpush(&l,3);
    lpush(&l,4);
    lprint(l);
    //linsert(&l,4,-1);
    //ldelete(&l,0);
    //ldelete(&l,0);
    printf("\n");
    //lprint(l);
    //printf("%i\n",lsearch(l,10));
    printf("%i\n",lpop(&l));
    printf("%i\n",lpop(&l));
    printf("%i\n",lpop(&l));
    printf("%i\n",lpop(&l));
    return 0;
}
*/
