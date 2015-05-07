#ifndef __TYPES__
#define __TYPES__

#include <iostream>

using namespace std;

class T_String{
    char *data;
public:
    T_String();
    T_String(const char* sample);
    T_String(const char sample);
    T_String(const int& sample);
    T_String(const T_String& sample);
    ~T_String();
    operator char *();
    T_String& operator = (const T_String& sample);
    T_String operator + (const T_String& sample);
    T_String slice(int from, int to);
    int index(char sample);
    T_String strip();
    int empty();
};

#endif
