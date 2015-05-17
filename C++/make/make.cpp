#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "Types.h"

using namespace std;

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

class Lexer
{
enum type_of_lex
    {
        LEX_NONE,
        LEX_H,
        LEX_LINE,
        LEX_HEADER,
        LEX_END
    } state;
    int var_colon,
    var_header;
    Module mod;
    Base<char> *buffer;
    Base<int> collected, waiting;
    int c;
    FILE* f;
    int pos,line;
    
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
    line = 1;
    pos = 0;
    state = LEX_NONE;
    var_colon = 0;
    var_header = 0;

    switch(getopt(argc,argv,"f:"))
    {
        case 'f':
            f = fopen(optarg, "r");
            break;
        default:
            f = fopen("tmp_makefile","r");
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
                        mod.append(Node());
                        ++pos;
                        buffer = new Base<char>;
                        state = LEX_HEADER;
                        var_colon = 0;
                        var_header = 1;
                        break;
                }
                break;

            case LEX_HEADER:
                switch(c)
                {
                    case ':':
                        if (var_colon)
                            throw T_String("Header error on line: ")+T_String(line);
                        var_colon = 1;
                        mod[pos-1].append_left_header(T_String(*buffer));
                        delete buffer;
                        buffer = new Base<char>;
                        break;

                    case '\n':
                    case -1:
                        if (!var_colon)
                        {
                            throw T_String("Header error on line: ")+T_String(line);
                        }
                        else
                        {
                            mod[pos-1].append_right_header(T_String(*buffer));
                            delete buffer;
                            state = LEX_H;
                            ++line;
                        }
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
                    
                    default:
                        if (isprint(c))
                            buffer->append(c);
                        break;
                }
                get_c();
                break;
            
            case LEX_END:
                exit(1);
        }
    }while(state != LEX_END);
}

void Lexer::print()
{
    mod.print();
}

void Lexer::collect()
{
    try
    {
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
}

int main(int argc, char** argv)
{
    Lexer lex(argc, argv);
    try
    {
        lex.load();
        lex.collect();
    //    lex.print();
    }
    catch( T_String e)
    {
        cout<< e << endl;
    }
    return 0;
}
