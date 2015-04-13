#include "Lists.h"

T_List::T_List()
{
    data = new T_String[1];
    pos = 0;
    len = 1;
}

T_List::~T_List()
{
    delete[] data;
}

T_String T_List::begining(const char *tmp) 
{
    return T_String();
}

T_String T_List::ending(const char *tmp) 
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
    }

    data[pos] = begining(tmp)+T_String(tmp)+ending(tmp);
    pos += 1;
}

ostream& T_List::print(ostream& os) 
{
    for (size_t i = 0; i<pos; i++)
    {
        os << data[i] << endl;
    }
    os << endl;
    return os;
}

ostream& operator << (ostream& os,  T_List& tmp)
{
    return (&tmp)->print(os);
}

//--------------------------------------------------Paragraph--------------------------------------------------

Paragraph::Paragraph(T_Args& tmp)
{
    char* tmp_indent = new char[tmp.r_v+1];
    memset(tmp_indent, ' ' , tmp.r_v*sizeof(char));
    tmp_indent[tmp.r_v] = '\0';
    indent = T_String(tmp_indent);
    delete[] tmp_indent;
    first = 1;
}

T_String Paragraph::begining(const char *tmp) 
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

T_String Paragraph::ending(const char *tmp) 
{
    return T_String();
}

//--------------------------------------------------Unoredered list--------------------------------------------

void Unordered_List::set_level(int new_level)
{
    level = new_level;
    char* tmp_indent = new char[2*level+t_v+2];
    memset(tmp_indent, ' ' , (2*level+t_v)*sizeof(char));
    tmp_indent[2*level+t_v] = m_v;
    tmp_indent[2*level+t_v+1] = '\0';
    indent = T_String(tmp_indent);
    delete[] tmp_indent;
}

void Unordered_List::next_level()
{
    set_level(++level);
}

void Unordered_List::prev_level()
{
    set_level(--level);
}

Unordered_List::Unordered_List(T_Args& tmp, int t_lvl)
{
    m_v = tmp.m_v;
    t_v = tmp.t_v;
    set_level(t_lvl);
}

T_String Unordered_List::begining(const char *tmp) 
{
    return indent;
}

T_String Unordered_List::ending(const char *tmp) 
{
    return T_String();
}

//--------------------------------------------------Oredered list----------------------------------------------

void Ordered_List::set_level(int new_level)
{
    level = new_level;
    char* tmp_indent = new char[2*level+t_v+1];
    memset(tmp_indent, ' ' , (2*level+t_v)*sizeof(char));
    tmp_indent[2*level+t_v] = '\0';
    indent = T_String(tmp_indent);
    delete[] tmp_indent;
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
    set_level(level);
}

void Ordered_List::prev_level()
{
    set_level(--level);
}

Ordered_List::Ordered_List(T_Args& tmp, int t_lvl)
{
    counter = new int[1];
    counter[0] = 0;
    t_v = tmp.t_v;
    set_level(t_lvl);
}

Ordered_List::~Ordered_List()
{
    delete[] counter;
}

T_String Ordered_List::begining(const char *tmp)
{
    counter[level]++;
    return indent+T_String(counter[level]);
}

T_String Ordered_List::ending(const char *tmp)
{
    return T_String();
}

//--------------------------------------------------Header-----------------------------------------------------


Header::Header(const T_Args& args, int t_lvl)
{
    level = t_lvl;
    w_v = args.w_v;
}

T_String Header::begining(const char *tmp)
{
    char *border = new char[w_v];
    memset(border, '#', w_v);
    border[w_v-2] = '\n';
    border[w_v-1] = '\0';

    int border_start = w_v/2-strlen(tmp)/2;
    char* tmp_indent = new char[border_start+1];
    memset(tmp_indent, ' ' , border_start*sizeof(char));
    for (int i = 0; i < level; i++)
        tmp_indent[border_start-level+i] = '#';
    tmp_indent[border_start] = '\0';

    indent = T_String(border) + T_String(tmp_indent);
    
    delete[] tmp_indent;
    delete[] border;
    return indent;
}

T_String Header::ending(const char *tmp)
{
    char *border = new char[w_v];
    memset(border, '#', w_v);
    border[w_v-1] = '\0';


    char* tmp_indent = new char[level+3];
    memset(tmp_indent, '#', level+1);
    tmp_indent[0] = ' ';
    tmp_indent[level+1] = '\n';
    tmp_indent[level+2] = '\0';

    indent = T_String(tmp_indent)+T_String(border);
    delete[] border;
    delete[] tmp_indent;
    return indent;
}

void Header::next_level()
{
    ++level;
}

void Header::prev_level()
{
    --level;
}