#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

#define SIZE 255


map::map()
{
    names = NULL;
    number = 0;
}

//XXX Это не правильный operator = 
void  map::operator = (const map& sample)
{
    number = sample.number;
    names = (char**)malloc(sizeof(char*)*number);
    for(int i = 0; i<number; i++)
    {
        names[i] = (char*)malloc(sizeof(char)*(strlen(sample.names[i])+1));
        strcpy(names[i],sample.names[i]);
    }
}

int map::add_vertice(char* name)
{
    number++;
    names = (char**)realloc(names,sizeof(char*)*number);
    names[number-1] = (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(names[number-1],name);
    return number-1;
}

const char* map::get_vertice(int number) const
{
    return names[number];
}

int map::get_number(char* name) const
{
    for(int i = 0; i<number; i++)
        if (!strcmp(names[i],name))
            return i;
    return -1;
}

map::~map()
{
    for(int i = 0; i<number; i++)
        free(names[i]);
    free(names);
}
