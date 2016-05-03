#ifndef __T_LIST__
#define __T_LIST__

#include <iostream>
#include "T_Args.h"
#include "T_String.h"

class T_List
{
    virtual T_String begining(T_String tmp, int curr_level);
    virtual T_String ending(T_String tmp, int curr_level);
    virtual T_String split_by_words(T_String str, int curr_level);
protected:
    T_String* data;
    size_t level;
    size_t pos;
    size_t len;
    int *levels;
public:
    virtual unsigned long int countSymbols() = 0; 
    virtual unsigned long int countWords() = 0; 
    T_List();
    virtual ~T_List();
    void append(const char * tmp);
    virtual void print() = 0;
};

#endif