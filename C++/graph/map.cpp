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

map& map::operator = (const map& sample)
{
        number = sample.number;
        names = (char**)malloc(sizeof(char*)*number);
        if (names == NULL)
            throw "Memory err!\n";
        for(int i = 0; i<number; i++)
        {
            names[i] = (char*)malloc(sizeof(char)*(strlen(sample.names[i])+1));
            if (names[i] == NULL)
                throw "Memory err!\n";
            strcpy(names[i],sample.names[i]);
        }
        return *this;
}

int map::add_vertice(char* name)
{
        number++;
        names = (char**)realloc(names,sizeof(char*)*number);
        if (names == NULL)
            throw "Memory err!\n";
        names[number-1] = (char*)malloc(sizeof(char)*strlen(name)+1);
        if (names[number-1] == NULL)
            throw "Memory err!\n";
        strcpy(names[number-1],name);
        return number-1;
}

char* map::get_vertice(int number)
{
    if (number != -1)
        return names[number];
    return NULL;
}

int map::get_number(char* name)
{
    for(int i = 0; i<number; i++)
        if (!strcmp(names[i],name))
            return i;
    printf("There is no vertice with name '%s'\n",name);
    return -1;
}

map::~map()
{
    for(int i = 0; i<number; i++)
        free(names[i]);
    free(names);
}
