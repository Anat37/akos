#include <iostream>
#include "T_Args.h"

class T_String{
    char *data;
    size_t len;
public:
    T_String();
    T_String(const char* sample);
    T_String(const T_String& sample);
    ~T_String();
    operator char *();
    T_String& operator = (const T_String& sample);
    friend size_t length(const T_String& sample);
};

T_String::T_String()
{
    data = NULL;
    len = 0;
}

T_String::T_String(const char* sample)
{
    len = strlen(sample);
    data = new char[len+1];
    strcpy(data, sample);
}

T_String::T_String(const T_String& sample)
{
    len = sample.len;
    data = new char[len+1];
    strcpy(data, sample.data);
}

T_String::~T_String()
{
    delete[] data;
}

T_String::operator char *()
{
    return data;
}

T_String& T_String::operator = (const T_String& sample)
{
    delete[] data;
    len = sample.len;
    data = new char[len+1];
    strcpy(data, sample.data);
    return *this;
}

size_t length(const T_String& sample)
{
    return sample.len;
}

int main(int argc, char** argv)
{
    T_Args args(argc, argv);
    //cout << args << endl ;
    T_String a("asd"), b(a);
    cout<< a << b << endl;
    T_String c;
    c = a;
    cout<< c << endl;
    return 0;
}