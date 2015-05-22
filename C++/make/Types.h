#ifndef __TYPES__
#define __TYPES__

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Node
{
    vector<string> split(string sample);
    string strip(string sample);
public:
    vector<string> data;
    string left_header;
    vector<string> right_header;
    
    Node(){}
    Node(const Node& sample);
    void append_left_header(string sample);
    void append_right_header(vector<string> sample);
    void append(string sample);
    void print();
    string& operator [](const int pos);
    Node& operator = (const Node& sample);
};

//----------------------------------------------------------------------------------------------------------------------------------

class Module
{
public:
    vector<Node> data;
    void append(Node sample);
    void print();
    Node& operator [](const int pos);
};

//----------------------------------------------------------------------------------------------------------------------------------

class Parser
{
    enum type_of_lex
    {
        LEX_NONE,
        LEX_H,
        LEX_LINE,
        LEX_LEFT,
        LEX_RIGHT_HEADER,
        LEX_RIGHT_VAR,
        LEX_COMMENT,
        LEX_VAR,
        LEX_END
    } state, prev_state;
    int var_header, var_left_bracket;
    string var_key;

    int debug;
    
    Module mod;
    map<string,string> dict;
    string buffer, var_buffer;
    vector<int> collected, waiting;
    
    int c;
    FILE* f;
    int pos,
        line,
        column;
    string node_to_run;
    
    void get_c();
    
    int get_mod_index(string& vertex);
    void assemble(int current);
    int is_file(string& filename);
    int get_index(vector<int> sample, int key);

    string strip(string sample);
    vector<string> split(string sample);

    string itoa(int number);
public:
    Parser(int argc, char** argv);
    void load(); //
    void print();
    void collect();
    ~Parser();
};

#endif
