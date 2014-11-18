struct Dictionary
{
  char **key;
  char **value;
  size_t pos;
  size_t max_len;
};

typedef struct Dictionary Dict;

Dict* dict_init();

void dict_append(Dict *,char*,char*);

char* dict_get(Dict *,char*);

void dict_clear(Dict **);
