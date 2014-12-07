struct strarr
{
    char **argv;
    int argc;
};

typedef struct strarr strarr;

strarr* strarr_init();

void strarr_push(strarr*,char* );

strarr* strarr_slice(strarr*,int,int);

char* strarr_pop(strarr*,int);

void strarr_clear(strarr*);
