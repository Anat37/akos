typedef int Tdata;
typedef struct __List List;
typedef struct __Node Node;

struct __Node
{
    Node *next;
    Tdata data;
};

struct __List{
    Node *head;
    int len;
};

void init(List *l);

int lpush(List *l, Tdata data);

Tdata lpop(List *l);

int linsert(List *l, int n, int data);

int ldelete(List *l, int n);

int lsearch(List l, Tdata key);

void lclear(List *l);
