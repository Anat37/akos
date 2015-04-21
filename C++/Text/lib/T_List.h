#ifndef __T_LIST__
#define __T_LIST__

#include <iostream>
#include "T_Args.h"
#include "T_String.h"

class T_List
{
    T_String* data;
    size_t pos;
    size_t len;
    int *levels;
    virtual T_String begining(T_String tmp, int curr_level);
    virtual T_String ending(T_String tmp, int curr_level);
    virtual T_String split_by_words(T_String str, int curr_level);
    virtual unsigned long int countSymbols() const = 0; 
    virtual unsigned long int countWords() const = 0; 
    ostream& print(ostream& os);
protected:
    size_t level;
public:
    T_List();
    virtual ~T_List();
    void append(const char * tmp);
    friend ostream& operator << (ostream& os, T_List& tmp);
};

#endif