#include "Paragraph.h"

Paragraph::Paragraph(T_Args& tmp)
{
    char* tmp_indent = new char[tmp.r_v+1];
    memset(tmp_indent, ' ' , tmp.r_v*sizeof(char));
    tmp_indent[tmp.r_v] = '\0';
    indent = T_String(tmp_indent);
    delete[] tmp_indent;
    first = 1;
    w_v = tmp.w_v;
}

T_String Paragraph::begining(T_String tmp, int curr_level) 
{
    if (first)
    {
        first = 0;
        return indent;
    }else
    {
        return T_String();
    }
}

T_String Paragraph::ending(T_String tmp, int curr_level) 
{
    return T_String();
}

T_String Paragraph::split_by_words(T_String str, int curr_level)
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
            if (i-last_pos>w_v)
            {
                cout<<"word len error!"<<endl;
                exit(0);
            }else
            {
                str[last_pos] = '\n';
                i = ++last_pos;
                col = 0;
            }
        }
    }

    if (col>w_v)
    {
        if (i-last_pos>w_v)
        {
            cout<<"word len error!"<<endl;
            exit(0);
        }else
        {
            str[last_pos] = '\n';
        }
    }

    return str;
}
