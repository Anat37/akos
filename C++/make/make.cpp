#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "Types.h"

using namespace std;

FILE* get_file(int &argc, char** argv)
{
    char* f_v = NULL;

    switch(getopt(argc,argv,"f:"))
    {
        case 'f':
            f_v = optarg;
            break;
    }

    if (!f_v)
    {
        f_v = (char*)"tmp_makefile";
    }
    return fopen(f_v,"r");
}

T_String read_long_line(FILE* fp)
{
    int max_size = 1,
        pos = 0;
    char *tmp_str = (char*)malloc(sizeof(char)*max_size);
    char tmp;

    while(fread(&tmp,sizeof(char),1,fp))
    {
        if (pos == max_size)
        {
            max_size<<=1;
            tmp_str = (char*)realloc(tmp_str,sizeof(char)*max_size);
        }

        if (tmp == '\n')
            break;
        if (tmp == '\r')
            continue;
        
        tmp_str[pos] = tmp;
        ++pos;
    }

    tmp_str = (char*)realloc(tmp_str,sizeof(char)*(pos+1));
    tmp_str[pos] = '\0';

    if (feof(fp)&&(pos == 0))
    {
        free(tmp_str);
        tmp_str = (char*)NULL;
    }
    
    T_String str(tmp_str);
    free(tmp_str);
    return str;
}

class Node
{
    T_String left_header, right_header;
    T_String* data;
    int pos, max_size;
public:
    Node();
    ~Node();
    Node(const Node& sample);
    void append_header(T_String sample);
    void append(T_String sample);
    void print();
    Node& operator = (const Node& sample);
};

Node::Node()
{
    pos = 0;
    max_size = 1;
    data = new T_String [max_size];
}

Node::Node(const Node& sample)
{
    left_header = sample.left_header;
    right_header = sample.right_header;
    pos = sample.pos;
    max_size = sample.max_size;
    data = new T_String[sample.max_size];
    for(int i=0; i<pos; i++)
        data[i] = sample.data[i];
}

Node::~Node()
{
    delete[] data;
}

void Node::append_header(T_String sample)
{
    int index = sample.index(':');
    left_header = sample.slice(0, index);
    right_header = sample.slice(index+1, strlen(sample));
}

void Node::append(T_String sample)
{
  if (pos == max_size)
    {
        max_size *= 2;
        T_String* tmp_data = new T_String [max_size];
        for(int i =0; i < pos; i++)
            tmp_data[i] = data[i];
        delete[] data;
        data = tmp_data;
    }
    data[pos] = sample;
    ++pos;
}

void Node::print()
{
    cout<<left_header<<'|'<<right_header<<endl;
    for(int i = 0; i < pos ;i++)
        cout<<data[i]<<endl;
}

Node& Node::operator = (const Node& sample)
{
    left_header = sample.left_header;
    right_header = sample.right_header;
    pos = sample.pos;
    max_size = sample.max_size;
    delete[] data;
    data = new T_String[sample.max_size];
    for(int i=0; i<pos; i++)
        data[i] = sample.data[i];
    return *this;
}

class Modules
{
    Node* data;
    int pos, max_size;
public:
    Modules();
    ~Modules();
    void append(Node sample);
    void print();
    Node& operator [](int pos);
};

Modules::Modules()
{
    pos = 0;
    max_size = 1;
    data = new Node [max_size];
}

Modules::~Modules()
{
    delete[] data;
}

void Modules::append(Node sample)
{
    if (pos == max_size)
    {
        max_size *= 2;
        Node* tmp_data = new Node [max_size];
        for(int i =0; i < pos; i++)
            tmp_data[i] = data[i];
        delete[] data;
        data = tmp_data;
    }
    data[pos] = sample;
    ++pos;
}

void Modules::print()
{
    for(int i = 0; i < pos ;i++)
        data[i].print();
}

Node& Modules::operator [](int pos)
{
    return data[pos];
}

int main(int argc, char** argv)
{
    FILE* f = get_file(argc,argv);

    Modules module;
    int pos = 0;

    while(!feof(f))
    {
        T_String tmp = read_long_line(f);

        if (!tmp.empty())
            continue;
        
        if (tmp[0] != '\t')
        {
            Node node;
            module.append(node);
            ++pos;
            module[pos-1].append_header(tmp.strip());
        }
        else
        {
            module[pos-1].append(tmp.strip());
        }
    }
    
    fclose(f);
    
    ++pos;
    module.print();

    return 0;
}
