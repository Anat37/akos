#include <iostream>

#include "T_Args.h"
#include "T_String.h"

using namespace std;

class T_List
{
    T_String* data;
    size_t pos;
    size_t len;
    virtual ostream& print(ostream& os) const;
    virtual T_String begining() const;
    virtual T_String ending() const;
public:
    T_List();
    ~T_List();
    void append(const char * tmp);
    friend ostream& operator << (ostream& os, const T_List& tmp);
};

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

T_String T_List::begining() const
{
    return T_String();
}

T_String T_List::ending() const
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

    data[pos] = begining()+T_String(tmp)+ending();
    pos += 1;
}

ostream& T_List::print(ostream& os) const
{
    for (size_t i = 0; i<pos; i++)
    {
        os<< data[i] << endl;
    }
    return os;
}

ostream& operator << (ostream& os, const T_List& tmp)
{
    return (&tmp)->print(os);
}

class Paragraph:public T_List
{
    T_Args args;
    T_String indent;
    T_String begining() const;
    T_String ending() const;
public:
    Paragraph(T_Args& tmp);
};

Paragraph::Paragraph(T_Args& tmp):args(tmp)
{
    indent = new char[tmp.t_v];
    memset((char*)indent, ' ', tmp.t_v*sizeof(char));
}

T_String Paragraph::begining() const
{
    return indent;
}

T_String Paragraph::ending() const
{
    return T_String("\n");
}

int main(int argc, char** argv)
{
    T_Args args(argc, argv);
    Paragraph tmp(args);
    tmp.append("qwe\nasdasdasd\nqweqweqwasd");
    tmp.append("asd");
    tmp.append("zxc");
    cout << tmp;
    return 0;
}