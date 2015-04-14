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

    data[pos] = split_by_words(begining(tmp)+T_String(tmp)+ending(tmp));
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

T_String T_List::split_by_words(T_String str)
{
    return str;
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
    w_v = tmp.w_v;
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

T_String Paragraph::split_by_words(T_String str)
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


//--------------------------------------------------Unoredered list--------------------------------------------

void Unordered_List::set_level(int new_level)
{
    level = new_level;
    char* tmp_indent = new char[2*level+t_v+1];
    memset(tmp_indent, ' ' , (2*level+t_v)*sizeof(char));
    tmp_indent[2*level+t_v] = '\0';
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
    w_v = tmp.w_v;
    set_level(t_lvl);
}

T_String Unordered_List::begining(const char *tmp) 
{
    return indent+T_String(m_v);
}

T_String Unordered_List::ending(const char *tmp) 
{
    return T_String();
}

size_t Unordered_List::get_indent(T_String str)
{
    size_t i=0;
    for(i=0; str[i] !='*'; i++);
    return i+1;
}

T_String Unordered_List::split_by_words(T_String str)
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
            if (i-last_pos+get_indent(str)>w_v)
            {
                cout<<"word len error!"<<endl;
                exit(0);
            }else
            {
                str = str.slice(0, last_pos)+T_String('\n')+indent+str.slice(last_pos+1, strlen(str));
                i = ++last_pos;
                col = 0;
            }
        }
    }

    if (col>w_v)
    {
        if (i-last_pos+get_indent(str)>w_v)
        {
            cout<<"word len error!"<<endl;
            exit(0);
        }else
        {
            str = str.slice(0, last_pos)+T_String('\n')+indent+str.slice(last_pos+1, strlen(str));
        }
    }
    return str;
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
    w_v = tmp.w_v;
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

size_t Ordered_List::get_indent(T_String str)
{
    size_t i=0;
    for(i=0; str[i] !='.'; i++);    
    return i+1;
}

T_String Ordered_List::split_by_words(T_String str)
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
            if (i-last_pos+get_indent(str)>w_v)
            {
                cout<<"word len error!"<<endl;
                exit(0);
            }else
            {
                str = str.slice(0, last_pos)+T_String('\n')+indent+str.slice(last_pos+1, strlen(str));
                i = ++last_pos;
                col = 0;
            }
        }
    }

    if (col>w_v)
    {
        if (i-last_pos+get_indent(str)>w_v)
        {
            cout<<"word len error!"<<endl;
            exit(0);
        }else
        {
            str = str.slice(0, last_pos)+T_String('\n')+indent+str.slice(last_pos+1, strlen(str));
        }
    }
    return str;
}

//--------------------------------------------------Header-----------------------------------------------------


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

T_String Header::begining(const char *tmp)
{
    return T_String();
}

T_String Header::ending(const char *tmp)
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

T_String Header::split_by_words(T_String str)
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
