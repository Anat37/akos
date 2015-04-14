#include <iostream>

#include "T_Args.h"
#include "T_String.h"

class T_List
{
    T_String* data;
    size_t pos;
    size_t len;
    virtual T_String begining(const char *tmp);
    virtual T_String ending(const char *tmp);
    virtual T_String split_by_words(T_String str);
    ostream& print(ostream& os);
public:
    T_List();
    virtual ~T_List();
    void append(const char * tmp);
    friend ostream& operator << (ostream& os,  T_List& tmp);
};

class Paragraph:public T_List
{
    T_String indent;
    int first, w_v;
    T_String split_by_words(T_String str);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Paragraph(T_Args& tmp);
};

class Unordered_List:public T_List
{
    T_String indent;
    int level;
    int t_v,
    w_v;
    char m_v;
    void set_level(int new_level);
    size_t get_indent(T_String str);
    T_String split_by_words(T_String str);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Unordered_List(T_Args& tmp, int t_lvl = 0);
    void next_level();
    void prev_level();
};

class Ordered_List:public T_List
{
    T_String indent;
    int level;
    int t_v,
    w_v;
    int *counter;
    void set_level(int new_level);
    size_t get_indent(T_String str);
    T_String split_by_words(T_String str);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Ordered_List(T_Args& tmp, int t_lvl = 0);
    ~Ordered_List();
    void next_level();
    void prev_level();
};

class Header: public T_List
{
    T_String border;
    int level;
    int w_v;
    T_String get_start_indent(T_String str);
    T_String split_by_words(T_String str);
    T_String begining(const char* tmp);
    T_String ending(const char* tmp);
public:
    Header(const T_Args& args, int t_lvl = 0);
    void next_level();
    void prev_level();
};
