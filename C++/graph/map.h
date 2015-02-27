#ifndef __MAP_H__
#define __MAP_H__

class map
{
    char** names;
    int number;
public:
    map();
    ~map();
    void operator = (const map& sample);
    int add_vertice(char* name);
    const char* get_vertice(int number) const;
    int get_number(char* name) const;
};

#endif /* __MAP_H__ */

