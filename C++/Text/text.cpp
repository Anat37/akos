#include <iostream>

#include "T_Args.h"
#include "T_String.h"
#include "Lists.h"

using namespace std;

enum T_Type {E_Paragraph, E_Unordered_List, E_Ordered_List, E_Header};

struct T_Line_Type
{
    T_Type type;
    int level;
};

class TextViewer
{
    FILE* fp;
    T_List **list;
    size_t pos, len;
public:
    TextViewer(const T_Args& args);
    ~TextViewer();
    T_String read_long_line();
    void append(T_List *tmp);
    friend ostream& operator << (ostream& os,  TextViewer& tmp);
};

TextViewer::TextViewer(const T_Args& args)
{
    fp = fopen(args.f_v, "r");
    pos = 0;
    len = 1;
    list = new T_List*[len];
}

TextViewer::~TextViewer()
{
    fclose(fp);
    for (size_t i = 0; i<pos; i++)
        delete list[i];
    delete[] list;
}

void TextViewer::append(T_List *tmp)
{
    if (pos == len)
    {
        len *= 2;
        T_List **tmp_arr = new T_List*[len];
        for (size_t i = 0; i < pos; i++)
            tmp_arr[i] = list[i];
        delete[] list;
        list = tmp_arr;
    }
    list[pos] = tmp;
    ++pos;
}

T_String TextViewer::read_long_line()
{
    int max_size = 1,
        pos = 0;
    char *str = (char*)malloc(sizeof(char)*max_size);
    char tmp;

    while(fread(&tmp,sizeof(char),1,fp))
    {
        if (pos == max_size)
        {
            max_size<<=1;
            str = (char*)realloc(str,sizeof(char)*max_size);
        }

        if (tmp == '\n')
            break;
        if (tmp == '\r')
            continue;
        
        str[pos] = tmp;
        ++pos;
    }

    str = (char*)realloc(str,sizeof(char)*(pos+1));
    str[pos] = '\0';

    if (feof(fp)&&(pos == 0))
    {
        free(str);
        str = (char*)NULL;
    }

    T_String line = T_String(str);
    free(str);
    return line;
}

ostream& operator << (ostream& os,  TextViewer& tmp)
{
    for (size_t i = 0; i<tmp.pos; i++)
    {
        os << *tmp.list[i];
    }
    return os;
}

void strip_string(T_String& str, int pos)
{
    int i;
    for(i = 0; str[i+pos]; i++)
        str[i] = str[i+pos];
    str[i] = '\0';
}

T_Line_Type get_type_and_strip(T_String& str)
{
    T_Line_Type type;
    int count = 0;
    int i;
    for (i = 0; str[i] == ' ' ; i++)
        count += 1;
    type.level = count/2;
    count = 0;

    if (str[i])
        for (int j = i; str[j+1]; j++)
            while ((str[j] == ' ')&&(str[j+1] == ' '))
                for (int k = j+1; str[k]; k++)
                    str[k] = str[k+1];
    
    if (str[i] == '#')
    {
        for (; str[i] == '#'; i++)
            count += 1;
        type.level = count;
        type.type = E_Header;
        strip_string(str, i);
        return type;
    }

    if (isdigit(str[i]))
    {
        int tmp_i = i;
        for (; isdigit(str[i]); i++)
            ;
        if (str[i] == '.')
        {
            type.type = E_Ordered_List;
            strip_string(str, i);
            return type;
        }else
        {
            i = tmp_i;
        }
    }

    if (str[i] == '*')
    {
        type.type = E_Unordered_List;
        strip_string(str, i+1);
        return type;
    }

    type.type = E_Paragraph;
    strip_string(str, i);
    return type;
}

int main(int argc, char** argv)
{
    T_Args args(argc, argv);
    TextViewer a(args);
    T_Line_Type type;
    T_String str;
    Header *h_tmp = NULL;
    Paragraph *p_tmp = NULL;
    Unordered_List *ul_tmp = NULL;
    Ordered_List *ol_tmp = NULL;
    int ol_flag = 0,
    ol_level = 0,
    ul_flag = 0,
    ul_level = 0,
    p_flag = 0;

    while ( (char*)(str = a.read_long_line()) )
    {
        type = get_type_and_strip(str);
        
        if (ol_flag && (type.type != E_Ordered_List))
        {
            a.append(ol_tmp);
            ol_flag = 0;
        }
        
        if (ul_flag && (type.type != E_Unordered_List))
        {
            a.append(ul_tmp);
            ul_flag = 0;
        }

        if ((p_flag && (type.type != E_Paragraph))||(str[0] == '\0'))
        {
            if (p_tmp != NULL) 
            {
                a.append(p_tmp);
                p_tmp = NULL;
            }
            p_flag = 0;
            if (str[0] == '\0')
                continue;
        }

        switch( type.type )
        {
            case E_Header:
                h_tmp = new Header(args, type.level);
                h_tmp->append(str);
                a.append(h_tmp);
                break;
            
            case E_Paragraph:
                if (!p_flag)
                {
                    p_tmp = new Paragraph(args);
                    p_flag = 1;
                }
                p_tmp->append(str);
                break;

            case E_Ordered_List:
                if (!ol_flag)
                {
                    ol_tmp = new Ordered_List(args, type.level);
                    ol_level = type.level;
                    ol_flag = 1;
                }else
                {
                    if (ol_level>type.level)
                    {
                        for (int q = 0 ; q < ol_level-type.level; q++)
                            ol_tmp->prev_level();
                        ol_level = type.level;
                    }

                    if (ol_level<type.level)
                    {
                        for (int q = 0 ; q < type.level-ol_level; q++)
                            ol_tmp->next_level();
                        ol_level = type.level;
                    }
                }
                ol_tmp->append(str);
                break;

            case E_Unordered_List:
                if (!ul_flag)
                {
                    ul_tmp = new Unordered_List(args, type.level);
                    ul_level = type.level;
                    ul_flag = 1;
                }else
                {
                    if (ul_level>type.level)
                    {
                        for (int q = 0 ; q < ul_level-type.level; q++)
                            ul_tmp->prev_level();
                        ul_level = type.level;
                    }

                    if (ul_level<type.level)
                    {
                        for (int q = 0 ; q < type.level-ul_level; q++)
                            ul_tmp->next_level();
                        ul_level = type.level;
                    }
                }
                ul_tmp->append(str);
                break;
        }
    }

    if (ol_flag)
    {
        a.append(ol_tmp);
    }

    if (ul_flag)
    {
        a.append(ul_tmp);
    }

    if (p_flag)
    {
        a.append(p_tmp);
    }

    cout<< a;
    return 0;
}