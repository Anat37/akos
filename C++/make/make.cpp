#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
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

int is_file(T_String& filename)
{
    struct stat buf;
    if ((stat(filename, &buf)>=0) && (S_ISREG(buf.st_mode)))
        return 1;
    return 0;
}

void run(Node& node)
{
    for(int i = 0; i < node.len(); i++)
        system(node[i]);
}

int get_mod_index(Module& mod, T_String& vertex)
{
    for(int i=0; i<mod.len(); i++)
        if (!strcmp(mod[i].left_header, vertex))
            return i;
    throw "index error";
    return -1;
}

void assemble(int current, Module& mod, Base<int>& collected, Base<int>& waiting)
{
    int index;
    for(int i = 0; i<mod[current].right_header.len(); i++)
    {
        if (waiting.index(current) != -1)
            throw "queue error";

        if(!is_file(mod[current].right_header[i]))
        {
            index = get_mod_index(mod,mod[current].right_header[i]);
            if (collected.index(index) == -1)
            {
                waiting.append(current);
                assemble(index, mod, collected, waiting);
                waiting.pop();
            }
        }
    }

    collected.append(current);
    run(mod[current]);
}

int main(int argc, char** argv)
{
    FILE* f = get_file(argc,argv);

    Module mod;
    int pos = 0;

    while(!feof(f))
    {
        T_String tmp = read_long_line(f);

        if (!tmp.empty())
            continue;
        
        if (tmp[0] != '\t')
        {
            Node node;
            mod.append(node);
            ++pos;
            mod[pos-1].append_header(tmp.strip());
        }
        else
        {
            mod[pos-1].append(tmp.strip());
        }
    }
    
    fclose(f);

    Base<int> collected, waiting;
    try
    {
        assemble(0, mod, collected, waiting);
    }
    catch (const char* e)
    {
        cout << e << endl;
    }
    return 0;
}
