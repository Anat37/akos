#include <iostream>
#include <cstring>
#include "T_Args.h"
#include "T_String.h"

class Abstract_Class
{
    T_String* data;
    int pos;
    int len;
    virtual T_String begining(const char *tmp);
    virtual T_String ending(const char *tmp);
    virtual T_String split_by_words(T_String str);
    ostream& print(ostream& os);
    virtual unsigned long int countSymbols() const = 0;
    virtual unsigned long int countWords() const = 0;
public:
    Abstract_Class();
    virtual ~Abstract_Class();
    void append(const char * tmp);
    friend ostream& operator << (ostream& os,  Abstract_Class& tmp);
};

class Paragraph:public Abstract_Class
{
    T_String indent;
    int first, w_v;
    T_String split_by_words(T_String str);
    unsigned long int countSymbols() const {return 0;};
    unsigned long int countWords() const {return 0;};
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
public:
    Paragraph(T_Args& tmp);
};

class Unordered_List:public Abstract_Class
{
    T_String indent;
    int level;
    int t_v,
    w_v;
    char m_v;
    void set_level(int new_level);
    int get_indent(T_String str);
    T_String split_by_words(T_String str);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
    unsigned long int countSymbols() const {return 0;};
    unsigned long int countWords() const {return 0;};
public:
    Unordered_List(T_Args& tmp, int t_lvl = 0);
    void next_level();
    void prev_level();
};

class Ordered_List:public Abstract_Class
{
    T_String indent;
    int level;
    int t_v,
    w_v;
    int *counter;
    void set_level(int new_level);
    int get_indent(T_String str);
    T_String split_by_words(T_String str);
    T_String begining(const char *tmp);
    T_String ending(const char *tmp);
    unsigned long int countSymbols() const {return 0;};
    unsigned long int countWords() const {return 0;};
public:
    Ordered_List(T_Args& tmp, int t_lvl = 0);
    ~Ordered_List();
    void next_level();
    void prev_level();
};

class Header: public Abstract_Class
{
    T_String border;
    int level;
    int w_v;
    T_String get_start_indent(T_String str);
    T_String split_by_words(T_String str);
    T_String begining(const char* tmp);
    T_String ending(const char* tmp);
    unsigned long int countSymbols() const {return 0;};
    unsigned long int countWords() const {return 0;};
public:
    Header(const T_Args& args, int t_lvl = 0);
    void next_level();
    void prev_level();
};
