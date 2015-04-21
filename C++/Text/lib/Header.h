#ifndef __HEADER__
#define __HEADER__

#include "T_Args.h"
#include "T_String.h"
#include "T_List.h"

class Header: public T_List
{
    T_String border;
    int level;
    int w_v;
    T_String get_start_indent(T_String str);
    T_String split_by_words(T_String str, int curr_level);
    T_String begining(T_String tmp, int curr_level);
    T_String ending(T_String tmp, int curr_level);
public:
    unsigned long int countSymbols();
    unsigned long int countWords();
    Header(const T_Args& args, int t_lvl = 0);
    void next_level();
    void prev_level();
    void print();
};

#endif