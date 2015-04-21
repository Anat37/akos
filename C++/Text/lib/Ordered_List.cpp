#include "Ordered_List.h"

T_String Ordered_List::indent(int curr_level)
{
    char* tmp_indent = new char[2*curr_level+t_v+1];
    memset(tmp_indent, ' ' , (2*curr_level+t_v)*sizeof(char));
    tmp_indent[2*curr_level+t_v] = '\0';
    T_String indent = T_String(tmp_indent);
    delete[] tmp_indent;
    return indent;
}

void Ordered_List::next_level()
{
    ++level;
    int *tmp_counter = new int[level+1];
    for(int i = 0; i < level; i++)
    {
        tmp_counter[i] = counter[i];
    }
    tmp_counter[level] = 0;
    delete[] counter;
    counter = tmp_counter;
}

void Ordered_List::prev_level()
{
    --level;
}

Ordered_List::Ordered_List(T_Args& tmp, int t_lvl)
{
    counter = new int[1];
    counter[0] = 0;
    t_v = tmp.t_v;
    w_v = tmp.w_v;
    level = t_lvl;
}

Ordered_List::~Ordered_List()
{
    delete[] counter;
}

T_String Ordered_List::begining(T_String tmp, int curr_level)
{
    counter[curr_level]++;
    return indent(curr_level)+T_String(counter[curr_level]);
}

T_String Ordered_List::ending(T_String tmp, int curr_level)
{
    return T_String();
}

T_String Ordered_List::split_by_words(T_String str, int curr_level)
{
    int col = 0,
    last_pos = 0,
    i = 0;

    for(i = 0; str[i]; i++, col++)
    {
        if ((str[i] == ' ')&&(col<=w_v))
        {
            last_pos = i;
        }

        if (col>w_v)
        {
            if (i-last_pos+curr_level*2+t_v>w_v)
            {
                cout<<"word len error!"<<endl;
                exit(0);
            }else
            {
                str = str.slice(0, last_pos)+T_String('\n')+indent(curr_level)+str.slice(last_pos+1, strlen(str));
                i = ++last_pos;
                col = 0;
            }
        }
    }

    if (col>w_v)
    {
        if (i-last_pos+curr_level*2+t_v>w_v)
        {
            cout<<"word len error!"<<endl;
            exit(0);
        }else
        {
            str = str.slice(0, last_pos)+T_String('\n')+indent(curr_level)+str.slice(last_pos+1, strlen(str));
        }
    }
    return str;
}