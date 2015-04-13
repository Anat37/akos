#include "T_String.h"

T_String::T_String()
{
    data = new char[1];
    data[0] = '\0';
}

T_String::T_String(const char* sample)
{
    if (sample!=NULL)
    {
        data = new char[strlen(sample)+1];
        strcpy(data, sample);
    }else{
        data = NULL;
    }
}

T_String::T_String(const int& sample)
{
    data = new char[255];
    sprintf(data, "%i", sample);
}

T_String::T_String(const T_String& sample)
{
    if (sample.data!=NULL)
    {
        data = new char[strlen(sample.data)+1];
        strcpy(data, sample.data);   
    }else
    {
        data = NULL;
    }
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
    if (sample.data!=NULL)
    {
        data = new char[strlen(sample.data)+1];
        strcpy(data, sample.data);
    }else
    {
        data = NULL;
    }
    return *this;
}

T_String T_String::operator + (const T_String& sample)
{
    T_String tmp;
    delete[] tmp.data;
    tmp.data = new char[strlen(this->data)+strlen(sample.data)+1];
    strcpy(tmp.data, this->data);
    strcat(tmp.data, sample.data);
    return tmp;
}