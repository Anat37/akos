struct Node;

typedef struct Node list;
typedef int Tdata;

int lpush(list **l,Tdata data);

int lpop(list **l);

int linsert(list **l, int n, int data);

int ldelete(list** l, int n);

int lsearch(list *l, Tdata key);

void lprint(list *l);
