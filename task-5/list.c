#include <stdlib.h>

typedef int Tdata;
typedef struct __List List;
typedef struct __Node Node;

#define MEMORY_ERR  1
#define END_OF_LIST 2
#define SUCCESS     0

struct __Node
{
    Node *next;
    Tdata data;
}; 

struct __List{
    Node *head;
    int len;
};

void init(List* l)
{
    l->len = 0;
    l->head = NULL;
}

int lpush(List* l, Tdata data)
{
    Node *tmp = (Node*)malloc(sizeof(Node));
    Node *i = NULL;
    tmp->next = NULL;
    tmp->data = data;
    
    if (tmp == NULL)
    {
        return MEMORY_ERR;
    }
    
    if (l->len == 0)
    {
        l->head = tmp;
        l->len += 1;
        return SUCCESS;
    }
    
    i = l->head;
    while(i->next!=NULL)
    {
        i = i->next;
    }
    i->next = tmp;
    l->len += 1;
    return SUCCESS;
}

Tdata lpop(List *l)
{
    Node *tmp = l->head;
    Tdata t;
    if(tmp->next == NULL)
    {
        Tdata t = tmp->data;
        free(tmp);
        l->head = NULL;
        l->len -= 1;
        return t;
    }
    
    while(tmp->next->next != NULL)
    {
       tmp = tmp->next; 
    }
    t = tmp->next->data;
    free(tmp->next);
    tmp->next = NULL;
    l->len -= 1;
    return t;
}

int linsert(List *l, int n, int data)
{
    Node *new = (Node*)malloc(sizeof(Node));
    int i = 1;
    Node *tmp;
    
    if (new==NULL)
    {
        return MEMORY_ERR;
    }
    
    if(n > l->len)
    {
        return END_OF_LIST;
    }
    
    if (n==0)
    {
        new -> next = l->head;
        new -> data = data;
        l->head = new;
        l->len += 1;
        return SUCCESS;
    }

    tmp = l->head;
    while(1)
    {
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
    l->len += 1;
    return SUCCESS;
}

int ldelete(List* l, int n)
{
    Node *tmp = l->head;
    Node *next = NULL;
    int i = 0;
    
    if((tmp == NULL)||(n>=l->len))  
    {
        return END_OF_LIST;
    }

    if(n==0)
    {
        next = l->head-> next; 
        free(l->head);
        l->head = next;
        l->len -= 1;
        return SUCCESS;
    }

    while(1)
    {
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
    l->len -= 1;
    return SUCCESS;
}

int lsearch(List l, Tdata key)
{
    int i = 0;
    Node *tmp = l.head;
    
    if(tmp == NULL)
    {
        return -1;
    }
    
    while( (tmp!= NULL)&&(tmp->data != key) )
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

void lclear(List *l)
{
    Node *tmp = l->head;
    while ( l->head != NULL)
    {
        tmp = l->head->next;
        free(l->head);
        l->head = tmp;
    }
    l->head = NULL;
    l->len = 0;
}
