#ifndef __MAP_H__
#define __MAP_H__

class map
{
    char** names;
    int number;
public:
    map();
    ~map();
    map& operator = (const map& sample);
    int add_vertice(char* name);
    char* get_vertice(int number);
    int get_number(char* name);
};

#endif /* __MAP_H__ */

