#include <stdio.h>
#include <stdlib.h>

typedef int Tdata;
typedef struct Node List;

#define MEMORY_ERR  1
#define END_OF_LIST 2
#define SUCCESS     0

struct Node
{
    List *next;
    Tdata data;
};

int lpush(List** l, Tdata data)
{
    List *tmp = NULL;
    tmp = (List*)malloc(sizeof(List));
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
        List* i = *l;
        while (i->next!=NULL)
            i = i->next;
        i->next = tmp;
    }
    return SUCCESS;
}

int lpop(List **l)
{
    List *tmp = *l;
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

int linsert(List **l, int n, int data)
{
    List *new = (List*)malloc(sizeof(List));
    int i = 1;
    List *tmp;
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

int ldelete(List** l, int n)
{
    List *tmp = *l;
    List *next = NULL;
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

int lsearch(List *l, Tdata key)
{
    int i = 0;
    List *tmp = l;
    
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

void lclear(List **l)
{
    List *tmp = *l;
    while ((*l) != NULL)
    {
        tmp = (*l)->next;
        free(*l);
        *l = tmp;
    }
    *l = NULL;
}

void lprint(List *l)
{
    List *tmp = l;
    if (tmp == NULL)
    {
        printf("Empty\n");
        return;
    }

    while(tmp != NULL)
    {
        printf("%i \n", tmp->data);
        tmp = tmp->next;
    }
    return;
}
