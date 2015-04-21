#include "T_List.h"

T_List::T_List()
{
    data = new T_String[1];
    levels = new int[1];
    pos = 0;
    len = 1;
    level = 0;
}

T_List::~T_List()
{
    delete[] data;
    delete[] levels;
}

T_String T_List::begining(T_String tmp, int curr_level) 
{
    return T_String();
}

T_String T_List::ending(T_String tmp, int curr_level) 
{
    return T_String();
}

void T_List::append(const char* tmp)
{
    if (pos==len)
    {
        len *= 2;
        T_String* n_data = new T_String[len];
        for(size_t i = 0; i<pos; i++)
        {
            n_data[i] = data[i];
        }
        delete[] data;
        data = n_data;

        int* n_levels = new int[len];
        for(size_t i = 0; i<pos; i++)
        {
            n_levels[i] = levels[i];
        }
        delete[] levels;
        levels = n_levels;
    }

    data[pos] = T_String(tmp);
    levels[pos] = level;
    ++pos;
}

ostream& T_List::print(ostream& os) 
{
    for (size_t i = 0; i<pos; i++)
    {
        os << begining(data[i], levels[i]) + split_by_words( data[i], levels[i]) + ending(data[i], levels[i]) << endl;
    }
    os << endl;
    return os;
}

ostream& operator << (ostream& os,  T_List& tmp)
{
    return (&tmp)->print(os);
}

T_String T_List::split_by_words(T_String str, int curr_level)
{
    return str;
}