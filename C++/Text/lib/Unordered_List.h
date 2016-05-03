#ifndef __UNORDERED_LIST__
#define __UNORDERED_LIST__

#include "T_Args.h"
#include "T_String.h"
#include "T_List.h"

class Unordered_List:public T_List
{
    int t_v,
    w_v;
    char m_v;
    T_String indent(int curr_level);
    T_String split_by_words(T_String str, int curr_level);
    T_String begining(T_String tmp, int curr_level);
    T_String ending(T_String tmp, int curr_level);
public:
    unsigned long int countSymbols();
    unsigned long int countWords();
    Unordered_List(T_Args& tmp, int t_lvl = 0);
    void next_level();
    void prev_level();
    void print();
};

#endif