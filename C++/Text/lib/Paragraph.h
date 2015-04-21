#ifndef __PARAGRAPH__
#define __PARAGRAPH__

#include "T_Args.h"
#include "T_String.h"
#include "T_List.h"

class Paragraph:public T_List
{
    int first, w_v;
    T_String indent;
    T_String split_by_words(T_String str, int curr_level);
    T_String begining(T_String tmp, int curr_level);
    T_String ending(T_String tmp, int curr_level);
public:
    unsigned long int countSymbols();
    unsigned long int countWords();
    void print();
    Paragraph(T_Args& tmp);
};

#endif