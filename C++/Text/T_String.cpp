#include "T_String.h"

T_String::T_String()
{
    data = new char[1];
    data[0] = '\0';
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

T_String::T_String(const int& sample)
{
    data = new char[255];
    sprintf(data, "%i", sample);
    len = strlen(data);
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

T_String T_String::operator + (const T_String& sample) const
{
    T_String tmp;
    delete[] tmp.data;
    tmp.len = this->len + sample.len;
    tmp.data = new char[tmp.len+1];
    strcpy(tmp.data, this->data);
    strcat(tmp.data, sample.data);
    return tmp;
}


size_t length(const T_String& sample)
{
    return sample.len;
}