#include "Types.h"

Node::Node(const Node& sample)
{
    left_header = sample.left_header;
    right_header = sample.right_header;
    pos = sample.pos;
    max_size = sample.max_size;
    delete[] data;
    data = new string[sample.max_size];
    for(int i=0; i<pos; i++)
        data[i] = sample.data[i];
}

void Node::append_left_header(string sample)
{
    left_header = sample;
}

void Node::append_right_header(Base<string> sample)
{
    right_header = sample;
}

void Node::print()
{
    cout<<"left header = "<<left_header<<"; right_header = ";
    for(int i = 0; i<right_header.len(); i++)
        cout<<right_header[i]<<' ';
    cout<<endl;

    for(int i = 0; i < pos ;i++)
    {
        cout<<data[i]<<endl;
    }
}

Node& Node::operator = (const Node& sample)
{
    left_header = sample.left_header;
    right_header = sample.right_header;
    pos = sample.pos;
    max_size = sample.max_size;
    delete[] data;
    data = new string[sample.max_size];
    for(int i=0; i<pos; i++)
        data[i] = sample.data[i];
    return *this;
}


//----------------------------------------------------------------------------------------------------------------------------------

void Module::print()
{
    for(int i = 0; i<pos; i++)
        data[i].print();
}

//----------------------------------------------------------------------------------------------------------------------------------

void Map::append(string key, string value)
{
    keys.append(key);
    values.append(value);
}

void Map::print()
{
    for(int i = 0; i<keys.len(); i++)
    {
        cout<<keys[i]<<" - "<<values[i]<<endl;
    }
}

string& Map::operator[](string key)
{
    for(int i=0;i<keys.len();i++)
        if (!keys[i].compare(key))
            return values[i];
    throw string("Variables error ");
}

//----------------------------------------------------------------------------------------------------------------------------------
