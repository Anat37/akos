#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "Types.h"

using namespace std;

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
    Module mod;
    Map map;
    string *buffer, *var_buffer;
    Base<int> collected, waiting;
    int c;
    FILE* f;
    int pos,line;
    string* node_to_run;
    
    void get_c();
    
    int get_mod_index(string& vertex); //
    void assemble(int current); //
    int is_file(string& filename); //

    string strip(string sample);
    Base<string> split(string sample);

    string itoa(int number);

public:
    Parser(int argc, char** argv);
    void load(); //
    void print();
    void collect();
    ~Parser();
};

Base<string> Parser::split(string sample)
{
    Base<string> ans;
    
    int i, last_pos = 0;
    for(i = 0; sample[i] ;i++)
    {
        if (sample[i] == ' ')
        {
            ans.append(sample.substr(last_pos, i-last_pos));
            last_pos = i+1;
        }
    }

    if (i!= last_pos)
        ans.append(sample.substr(last_pos, i-last_pos));

    return ans;
}

string Parser::strip(string sample)
{
    string tmp;
    int i = 0, 
    space = 0;

    while((sample[i] == ' ')||(sample[i] == '\t'))
        ++i;
    
    while(sample[i])
    {
        if (sample[i] == ' ')
        {
            space = 1;
        }else
        {
            if(space)
            {
                tmp.push_back(' ');
                space = 0;
            }
            tmp.push_back(sample[i]);
        }
        ++i;
    }
    return tmp;
}

void Parser::get_c()
{
    c = fgetc(f);
}

int Parser::is_file(string& filename)
{
    struct stat buf;
    if ((stat(filename.c_str(), &buf)>=0) && (S_ISREG(buf.st_mode)))
        return 1;
    return 0;
}

int Parser::get_mod_index(string& vertex)
{
    for(int i=0; i<mod.len(); i++)
    {
        if (!mod[i].left_header.compare(vertex))
            return i;
    }
    cout<<"there no vertex: "<<vertex<<endl;
    throw "index error";
    return -1;
}

void Parser::assemble(int current)
{
    int index;
    for(int i = 0; i<mod[current].right_header.len(); i++)
    {
        if (waiting.index(current) != -1)
            throw "queue error";

        if(!is_file(mod[current].right_header[i]))
        {
            index = get_mod_index(mod[current].right_header[i]);
            if (collected.index(index) == -1)
            {
                waiting.append(current);
                assemble(index);
                waiting.pop();
            }
        }
    }

    collected.append(current);
    
    for(int i = 0; i < mod[current].len(); i++)
        system(mod[current][i].c_str());
}

Parser::Parser(int argc, char** argv)
{
    node_to_run = NULL;
    line = 1;
    pos = 0;
    prev_state = LEX_NONE;
    state = LEX_NONE;
    var_left_bracket = 0;
    var_header = 0;

    switch(getopt(argc,argv,"f:"))
    {
        case 'f':
            f = fopen(optarg, "r");
            break;
        default:
            f = fopen("tmp_makefile","r");
    }
    
    if (optind<argc)
    {
        node_to_run = new string;
        *node_to_run = string(argv[optind]);
    }
}

void Parser::load()
{
    do
    {
        switch(state)
        {
            case LEX_NONE:
                get_c();
                state = LEX_H;

            case LEX_H:
                switch(c)
                {
                    case -1:
                        state = LEX_END;
                        break;

                    case '\n':
                        var_header = 0;
                        ++line;
                        get_c();
                        break;

                    case '#':
                        state = LEX_COMMENT;
                        get_c();
                        break;

                    case '\t':
                        buffer = new string;
                        state = LEX_LINE;
                        get_c();
                        break;

                    default:
                        buffer = new string;
                        state = LEX_LEFT;
                        break;
                }
                break;

            case LEX_LEFT:
                switch(c)
                {
                    case ':':
                        mod.append(Node());
                        ++pos;
                        var_header = 1;
                        mod[pos-1].append_left_header(strip(*buffer));
                        delete buffer;
                        buffer = new string;
                        state = LEX_RIGHT_HEADER;
                        break;

                    case '#':
                        delete buffer;
                        throw string("Line type error on line: ")+string(itoa(line));
                        break;

                    case '=':
                        var_key = strip(*buffer);
                        delete buffer;
                        buffer = new string;
                        state = LEX_RIGHT_VAR;
                        break;

                    case '$':
                        prev_state = LEX_LEFT;
                        state = LEX_VAR;
                        break;

                    case '\n':
                    case -1:
                        delete buffer;
                        throw string("Line type error on line: ")+string(itoa(line));
                        break;

                    default:
                        if (isprint(c))
                            buffer->push_back(c);                
                        break;
                }
                get_c();
                break;

            case LEX_RIGHT_HEADER:
                switch(c)
                {
                    case ':':
                        delete buffer;
                        throw string("Header error on line: ")+string(itoa(line));
                        break;

                    case '\n':
                    case -1:
                        mod[pos-1].append_right_header(split(strip(*buffer)));
                        delete buffer;
                        state = LEX_H;
                        ++line;
                        break;

                    case '#':
                        mod[pos-1].append_right_header(split(strip(*buffer)));
                        delete buffer;
                        state = LEX_COMMENT;
                        break;

                    case '=':
                        delete buffer;
                        throw string("Line type error on line: ")+string(itoa(line));
                        break;

                    case '$':
                        prev_state = LEX_RIGHT_HEADER;
                        state = LEX_VAR;
                        break;

                    default:
                        if (isprint(c))
                            buffer->push_back(c);                
                        break;
                }
                get_c();
                break;

            case LEX_RIGHT_VAR:
                switch(c)
                {
                    case '=':
                        delete buffer;
                        throw string("Variable error on line: ")+string(itoa(line));
                        break;

                    case ':':
                        delete buffer;
                        throw string("Line type error on line: ")+string(itoa(line));
                        break;
                        
                    case '\n':
                    case -1:
                        map.append(var_key, strip(*buffer));
                        delete buffer;
                        state = LEX_H;
                        ++line;
                        break;

                    case '#':
                        map.append(var_key, strip(*buffer));
                        delete buffer;
                        state = LEX_COMMENT;
                        break;

                    default:
                        if (isprint(c))
                            buffer->push_back(c);                
                        break;
                }
                get_c();
                break;

            case LEX_LINE:
                if (!var_header)
                {
                    delete buffer;
                    throw string("Header error on line: ")+string(itoa(line));
                }

                switch(c)
                {
                    case '\n':
                    case -1:
                        mod[pos-1].append(strip(*buffer));
                        delete buffer;
                        state = LEX_H;
                        ++line;
                        break;
                    
                    case '$':
                        prev_state = LEX_LINE;
                        state = LEX_VAR;
                        break;

                    case '#':
                        mod[pos-1].append(strip(*buffer));
                        delete buffer;
                        state = LEX_COMMENT;
                        break;

                    default:
                        if (isprint(c))
                            buffer->push_back(c);
                        break;
                }
                get_c();
                break;
            
            case LEX_VAR:
                if ((c=='(')&&(!var_left_bracket))
                {
                    var_left_bracket = 1;
                    var_buffer = new string;
                    get_c();
                    break;
                }

                switch(c)
                {
                    case '(':
                        delete buffer;
                        delete var_buffer;
                        throw string("Brackets error on line: ")+string(itoa(line));
                        break;

                    case ')':
                    {
                        string ans;
                        try
                        {
                            ans = map[ string(*var_buffer) ];
                        }
                        catch(string e)
                        {
                            delete buffer;
                            delete var_buffer;
                            throw e+string("on line: ")+string(itoa(line));
                        }
                        
                        int len = ans.length();
                        for(int i = 0; i<len; i++)
                            buffer->push_back(ans[i]);
                        
                        delete var_buffer;
                        var_left_bracket = 0;
                        state = prev_state;
                        break;
                    }

                    case '\n':
                    case -1:
                        delete buffer;
                        delete var_buffer;
                        throw string("Brackets error on line: ")+string(itoa(line));

                    case '#':
                        delete buffer;
                        delete var_buffer;
                        throw string("Brackets error on line: ")+string(itoa(line));

                    default:
                        if (isprint(c))
                            var_buffer->push_back(c);
                        break;
                }
                get_c();
                break;

            case LEX_COMMENT:
                switch(c)
                {
                    case '\n':
                        state = LEX_H;
                        ++line;
                        break;
                }
                get_c();
                break;

            case LEX_END:
                exit(1); // в теории сюда дойти нельзя
        }
    }while(state != LEX_END);
}

void Parser::print()
{
    cout<<"--Start--"<<endl;
    cout<<"--Modules--"<<endl;
    mod.print();

    cout<<"--Dictionary--"<<endl;
    map.print();
    cout<<"--End--"<<endl;
}

void Parser::collect()
{
    try
    {
        if (node_to_run)
            assemble(get_mod_index(*node_to_run));
        else
            assemble(0);
    }
    catch (const char* e)
    {
        cout << e << endl;
    }
}

Parser::~Parser()
{
    fclose(f);
    delete node_to_run;
}

string Parser::itoa(int number)
{
    char buff[21]; // enought for int
    sprintf(buff, "%i", number);
    return string(buff);
}

int main(int argc, char** argv)
{ 
    Parser lex(argc, argv);
    try
    {
        lex.load();
        lex.print();
        cout<<endl;
        lex.collect();
    }
    catch(string e)
    {
        cout<< e << endl;
    }

    return 0;
}
