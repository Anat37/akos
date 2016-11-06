#include "Header.h"

Header::Header(const T_Args& args, int t_lvl)
{
    level = t_lvl;
    w_v = args.w_v;
    
    char *tmp_border = new char[w_v+1];
    memset(tmp_border, '#', w_v);
    tmp_border[w_v] = '\0';
    border = T_String(tmp_border);
    delete[] tmp_border;
}

T_String Header::begining(T_String tmp, int curr_level)
{
    return T_String();
}

T_String Header::ending(T_String tmp, int curr_level)
{
    return T_String();
}

void Header::next_level()
{
    ++level;
}

void Header::prev_level()
{
    --level;
}

T_String Header::split_by_words(T_String str, int curr_level)
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

    int last_i = 0;
    T_String tmp_str; 
    int first = 1;
    
    for(int i = 0; str[i]; i++)
    {
        if (str[i] == '\n')
        {
            if (first)
            {
                tmp_str = tmp_str+get_start_indent(str.slice(last_i, i));
                first = 0;
            }
            else
            {
                tmp_str = tmp_str+T_String('\n')+get_start_indent(str.slice(last_i, i));
            }
            last_i = i+1;
        }
    }

    if (first)
    {
        tmp_str = tmp_str+get_start_indent(str.slice(last_i, strlen(str)));
    }
    else
    {
        tmp_str = tmp_str+T_String('\n')+get_start_indent(str.slice(last_i, strlen(str)));
    }

    return border+T_String('\n')+tmp_str+T_String('\n')+border;
}

T_String Header::get_start_indent(T_String str)
{
    if (strlen(str)>w_v)
        return str;
    size_t pos = (w_v-strlen(str))/2;
    char *indent_start = new char [pos+2];
    memset(indent_start, ' ', pos);
    indent_start[pos] = '\0';
    T_String str_indent_start(indent_start);
    delete[] indent_start;
    return str_indent_start+str;
}

void Header::print() 
{
    T_String ans;
    for (size_t i = 0; i<pos; i++)
    {
        ans = ans + begining(data[i], levels[i]) + split_by_words( data[i], levels[i]) + ending(data[i], levels[i]) + T_String('\n');
    }
    ans = ans + T_String('\n');
    cout << ans;
}

unsigned long int Header::countSymbols()
{
    T_String ans;
    for (size_t i = 0; i<pos; i++)
    {
        ans = ans + begining(data[i], levels[i]) + split_by_words( data[i], levels[i]) + ending(data[i], levels[i]) + T_String('\n');
    }
    ans = ans + T_String('\n');

    return strlen(ans);
} 

unsigned long int Header::countWords()
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