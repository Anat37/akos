#ifndef STDLIB
    #include <stdlib.h>
    #define STDLIB
#endif

#ifndef STDIO
    #include <stdio.h>
    #define STDIO
#endif

#ifndef STRING
    #include <string.h>
    #define STRING
#endif

struct Dictionary
{
    char **key;
    char **value;
    size_t pos;
    size_t max_len;
};

typedef struct Dictionary Dict;

Dict* dict_init()
{
    Dict *d = (Dict*)malloc(sizeof(Dict));
    d->pos = 0;
    d->max_len = 1;
    d->key = (char**)malloc(sizeof(char*));
    (d->key)[0] = NULL;
    d->value = (char**)malloc(sizeof(char*));
    (d->value)[0] = NULL;
    return d;
}

void dict_append(Dict *d,char* key,char*value)
{
    (d->key)[d->pos] = (char*)malloc(sizeof(char)*(strlen(key)+1));
    strcpy( (d->key)[d->pos],key );

    (d->value)[d->pos] = (char*)malloc(sizeof(char)*(strlen(value)+1));
    strcpy( (d->value)[d->pos],value );
    
    d->pos += 1;
    
    if(d->pos == d->max_len)
    {
        d->max_len *= 2;
        d->key = (char**)realloc(d->key,sizeof(char*)*(d->max_len));
        d->value = (char**)realloc(d->value,sizeof(char*)*(d->max_len));
    }
}

char* dict_get(Dict *d,char* key)
{
    size_t i;
    for(i = 0; i<d->pos; i++)
    {
        if ( !strcmp( (d->key)[i], key ) )
            return (d->value)[i];
    }
    return NULL;
}

void dict_clear(Dict** d)
{
    size_t i;
    for(i = 0;i<(*d)->pos;i++)
    {
        free(((*d)->key)[i]);
        free(((*d)->value)[i]);
    }
    free((*d)->key);
    free((*d)->value);
    free((*d));
    *d = NULL;
}



