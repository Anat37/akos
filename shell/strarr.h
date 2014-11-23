struct strarr
{
    char **argv;
    int argc;
};

typedef struct strarr strarr;

strarr* strarr_init();

void strarr_push(strarr *tmp,char* str);

strarr* strarr_slice(strarr *tmp,int start,int end);

void strarr_clear(strarr* tmp);
