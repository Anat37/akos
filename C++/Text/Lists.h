#include <iostream>

#include "T_Args.h"
#include "T_String.h"

class T_List
{
    T_String* data;
    size_t pos;
    size_t len;
    ostream& print(ostream& os);
    virtual T_String begining(const char *tmp);
    virtual T_String ending(const char *tmp);
public:
    T_List();
    ~T_List();
    void append(const char * tmp);
    friend ostream& operator << (ostream& os,  T_List& tmp);
};

class Paragraph:public T_List
{
    T_String indent;
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Paragraph(T_Args& tmp);
};

class Unordered_List:public T_List
{
    T_String indent;
    int level;
    int t_v;
    char m_v;
    void set_level(int new_level);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Unordered_List(T_Args& tmp);
    void next_level();
    void prev_level();
};

class Ordered_List:public T_List
{
    T_String indent;
    int level;
    int t_v;
    int *counter;
    void set_level(int new_level);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Ordered_List(T_Args& tmp);
    ~Ordered_List();
    void next_level();
    void prev_level();
};

class Header: public T_List
{
    T_String indent;
    int level;
    int w_v;
    T_String begining(const char* tmp);
    T_String ending(const char* tmp);
public:
    Header(const T_Args& args);
    void next_level();
    void prev_level();
};
