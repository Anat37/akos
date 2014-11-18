struct Dictionary
{
  char **key;
  char **value;
  size_t pos;
  size_t max_len;
};

typedef struct Dictionary Dict;

Dict* dict_init();

void dict_append(Dict *d,char* key,char*value);

char* dict_get(Dict *d,char* key);

void dict_clear(Dict **d);
