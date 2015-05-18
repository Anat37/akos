#ifndef __TYPES__
#define __TYPES__

#include <iostream>

using namespace std;

template <class T>
class Base;

class T_String{
    char *data;
public:
    T_String();
    T_String(const char* sample);
    T_String(const char sample);
    T_String(const int sample);
    T_String(const T_String& sample);
    T_String(const Base<char>& sample);
    ~T_String();
    operator char *();
    T_String& operator = (const T_String& sample);
    T_String operator + (const T_String& sample);
    T_String slice(int from, int to);
    int index(char sample);
    T_String strip();
    int empty();
    Base<T_String> split();
};

//----------------------------------------------------------------------------------------------------------------------------------

template <class T>
class Base
{
protected:
    T* data;
    int pos, max_size;
public:
    Base();
    Base(const Base& sample);
    virtual ~Base();
    virtual Base<T>& operator = (Base<T> sample);
    T& operator [] (int pos);
    T operator [] (int pos) const;
    virtual void print(){};
    int len() const;
    void append(T sample);
    T pop();
    int index(T sample);
};

template <class T>
Base<T>::Base()
{
    pos = 0;
    max_size = 1;
    data = new T [max_size];
}

template <class T>
Base<T>::Base(const Base<T>& sample)
{
    pos = sample.pos;
    max_size = sample.max_size;
    data = new T [sample.max_size];
    for(int i=0; i<pos; i++)
        data[i] = sample.data[i];
}

template <class T>
Base<T>::~Base()
{
    delete[] data;
}

template <class T>
void Base<T>::append(T sample)
{
  if (pos == max_size)
    {
        max_size *= 2;
        T* tmp_data = new T [max_size];
        for(int i =0; i < pos; i++)
            tmp_data[i] = data[i];
        delete[] data;
        data = tmp_data;
    }
    data[pos] = sample;
    ++pos;
}

template <class T>
Base<T>& Base<T>::operator = (Base<T> sample)
{
    pos = sample.pos;
    max_size = sample.max_size;
    delete[] data;
    data = new T [sample.max_size];
    for(int i=0; i<pos; i++)
        data[i] = sample.data[i];
    return *this;
}

template <class T>
T& Base<T>::operator [] (int pos)
{
    return data[pos];
}

template <class T>
T Base<T>::operator [] (int pos) const
{
    return data[pos];
}

template <class T>
int Base<T>::len() const
{
    return pos;
}

template <class T>
T Base<T>::pop()
{
    --pos;
    return data[pos];
}

template <class T>
int Base<T>::index(T sample)
{
    for(int i=0; i<pos; i++)
        if (data[i] == sample)
            return i;
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------

class Node: public Base< T_String >
{
public:
    T_String left_header;
    Base<T_String> right_header;
    Node(){}
    Node(const Node& sample);
    void append_left_header(T_String sample);
    void append_right_header(T_String sample);
    void print();
    Node& operator = (const Node& sample);
};

//----------------------------------------------------------------------------------------------------------------------------------

class Module: public Base<Node>
{
public:
    void print();
};

//----------------------------------------------------------------------------------------------------------------------------------

class Map
{
    Base<T_String> keys, values;
public:
    void append(T_String key, T_String value);
    void print();
    T_String& operator[](T_String key);
};

//----------------------------------------------------------------------------------------------------------------------------------

#endif
