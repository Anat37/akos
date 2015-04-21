#ifndef __ORDERED_LIST__
#define __ORDERED_LIST__

#include "T_Args.h"
#include "T_String.h"
#include "T_List.h"

class Ordered_List:public T_List
{
    int t_v,
    w_v;
    int *counter;
    T_String indent(int curr_level);
    T_String split_by_words(T_String str, int curr_level);
    T_String begining(T_String tmp, int curr_level);
    T_String ending(T_String tmp, int curr_level);
    unsigned long int countSymbols() const { return 0;} 
    unsigned long int countWords() const { return 0;}
public:
    Ordered_List(T_Args& tmp, int t_lvl = 0);
    ~Ordered_List();
    void next_level();
    void prev_level();
};

#endif