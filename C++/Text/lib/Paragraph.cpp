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

void Paragraph::print() 
{
    T_String ans;
    for (size_t i = 0; i<pos; i++)
    {
        ans = ans + begining(data[i], levels[i]) + split_by_words( data[i], levels[i]) + ending(data[i], levels[i]) + T_String('\n');
    }
    ans = ans + T_String('\n');
    cout << ans;
}

unsigned long int Paragraph::countSymbols()
{
    T_String ans;
    for (size_t i = 0; i<pos; i++)
    {
        ans = ans + begining(data[i], levels[i]) + split_by_words( data[i], levels[i]) + ending(data[i], levels[i]) + T_String('\n');
    }
    ans = ans + T_String('\n');

    return strlen(ans);
} 

unsigned long int Paragraph::countWords()
{
    unsigned long int count = 0;
    for (size_t i = 0; i<pos; i++)
    {
        for(size_t j = 0; j<strlen(data[i]); j++)
        {
            if ((data[i][j] == ' ')||(data[i][j] == '\n'))
                count += 1;
        }
    }
    return count;
}

