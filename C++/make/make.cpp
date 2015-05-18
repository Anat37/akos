#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "Types.h"

using namespace std;

class Lexer
{
enum type_of_lex
    {
        LEX_NONE,
        LEX_H,
        LEX_LINE,
        LEX_LEFT,
        LEX_RIGHT_HEADER,
        LEX_RIGHT_VAR,
        LEX_VAR,
        LEX_END
    } state, prev_state;
    int var_header, var_left_bracket;
    T_String var_key;
    Module mod;
    Map map;
    Base<char> *buffer, *var_buffer;
    Base<int> collected, waiting;
    int c;
    FILE* f;
    int pos,line;
    T_String* node_to_run;
    
    void get_c();
    int get_mod_index(T_String& vertex);
    void assemble(int current);
    int is_file(T_String& filename);
public:
    Lexer(int argc, char** argv);
    void load();
    void print();
    void collect();
    ~Lexer();
};

void Lexer::get_c()
{
    c = fgetc(f);
}

int Lexer::is_file(T_String& filename)
{
    struct stat buf;
    if ((stat(filename, &buf)>=0) && (S_ISREG(buf.st_mode)))
        return 1;
    return 0;
}

int Lexer::get_mod_index(T_String& vertex)
{
    for(int i=0; i<mod.len(); i++)
        if (!strcmp(mod[i].left_header, vertex))
            return i;
    throw "index error";
    return -1;
}

void Lexer::assemble(int current)
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
        system(mod[current][i]);
}

Lexer::Lexer(int argc, char** argv)
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
        node_to_run = new T_String;
        *node_to_run = T_String(argv[optind]);
    }
}

void Lexer::load()
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

                    case '\t':
                        buffer = new Base<char>;
                        state = LEX_LINE;
                        get_c();
                        break;

                    default:
                        buffer = new Base<char>;
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
                        mod[pos-1].append_left_header(T_String(*buffer));
                        delete buffer;
                        buffer = new Base<char>;
                        state = LEX_RIGHT_HEADER;
                        break;

                    case '=':
                        var_key = T_String(*buffer);
                        delete buffer;
                        buffer = new Base<char>;
                        state = LEX_RIGHT_VAR;
                        break;

                    case '$':
                        prev_state = LEX_LEFT;
                        state = LEX_VAR;
                        break;

                    case '\n':
                    case -1:
                        throw T_String("Line type error on line: ")+T_String(line);
                        break;

                    default:
                        if (isprint(c))
                            buffer->append(c);                
                        break;
                }
                get_c();
                break;

            case LEX_RIGHT_HEADER:
                switch(c)
                {
                    case ':':
                        throw T_String("Header error on line: ")+T_String(line);
                        break;

                    case '\n':
                    case -1:
                        mod[pos-1].append_right_header(T_String(*buffer));
                        delete buffer;
                        state = LEX_H;
                        ++line;
                        break;

                    case '$':
                        prev_state = LEX_RIGHT_HEADER;
                        state = LEX_VAR;
                        break;

                    default:
                        if (isprint(c))
                            buffer->append(c);                
                        break;
                }
                get_c();
                break;

            case LEX_RIGHT_VAR:
                switch(c)
                {
                    case '=':
                    throw T_String("Variable error on line: ")+T_String(line);
                        break;

                    case '\n':
                    case -1:
                        map.append(var_key.strip(), T_String(*buffer).strip());
                        delete buffer;
                        state = LEX_H;
                        ++line;
                        break;

                    default:
                        if (isprint(c))
                            buffer->append(c);                
                        break;
                }
                get_c();
                break;

            case LEX_LINE:
                if (!var_header)
                    throw T_String("Header error on line: ")+T_String(line);
                switch(c)
                {
                    case '\n':
                    case -1:
                        state = LEX_H;
                        ++line;
                        mod[pos-1].append(T_String(*buffer));
                        delete buffer;
                        break;
                    
                    case '$':
                        prev_state = LEX_LINE;
                        state = LEX_VAR;
                        break;

                    default:
                        if (isprint(c))
                            buffer->append(c);
                        break;
                }
                get_c();
                break;
            
            case LEX_VAR:
                if ((c=='(')&&(!var_left_bracket))
                {
                    var_left_bracket = 1;
                    var_buffer = new Base<char>;
                    get_c();
                    break;
                }

                switch(c)
                {
                    case '(':
                        throw T_String("Brackets error on line: ")+T_String(line);
                        break;

                    case ')':
                    {
                        T_String ans = map[T_String(*var_buffer).strip()];
                        
                        int len = strlen(ans);
                        for(int i = 0; i<len; i++)
                            buffer->append(ans[i]);
                        
                        delete var_buffer;
                        var_left_bracket = 0;
                        state = prev_state;
                        break;
                    }

                    default:
                        if (isprint(c))
                            var_buffer->append(c);
                        break;
                }
                get_c();
                break;

            case LEX_END:
                exit(1); // в теории сюда дойти нельзя
        }
    }while(state != LEX_END);
}

void Lexer::print()
{
    cout<<"--Start--"<<endl;
    cout<<"--Modules--"<<endl;
    mod.print();

    cout<<"--Dictionary--"<<endl;
    map.print();
    cout<<"--End--"<<endl;
}

void Lexer::collect()
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

Lexer::~Lexer()
{
    fclose(f);
    delete node_to_run;
}

int main(int argc, char** argv)
{
    Lexer lex(argc, argv);
    try
    {
        lex.load();    
        lex.print();
        cout<<endl;
        lex.collect();
    }
    catch( T_String e)
    {
        cout<< e << endl;
    }
    return 0;
}
