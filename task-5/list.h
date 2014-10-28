struct Node;

typedef struct Node List;
typedef int Tdata;

int lpush(List **l,Tdata data);

int lpop(List **l);

int linsert(List **l, int n, int data);

int ldelete(List** l, int n);

int lsearch(List *l, Tdata key);

void lprint(List *l);

void lclear(List **l);
