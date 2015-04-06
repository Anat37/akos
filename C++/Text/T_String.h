#ifndef __T_STRING__
#define __T_STRING__
#include <iostream>

using namespace std;

class T_String{
    char *data;
    size_t len;
public:
    T_String();
    T_String(const char* sample);
    T_String(const T_String& sample);
    T_String(const int& sample);
    ~T_String();
    operator char *();
    T_String& operator = (const T_String& sample);
    T_String operator + (const T_String& sample) const;
    friend size_t length(const T_String& sample);
};

#endif