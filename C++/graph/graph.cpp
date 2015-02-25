#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255

class map
{
    char** names;
    int number;
public:
    map();
    ~map();
    int add_vertice(char* name);
    char* get_vertice(int number);
    int get_number(char* name);
};

map::map()
{
    names = NULL;
    number = 0;
}

int map::add_vertice(char* name)
{
    number++;
    names = (char**)realloc(names,sizeof(char*)*number);
    names[number-1] = (char*)malloc(sizeof(char)*strlen(name));
    strcpy(names[number-1],name);
    return number-1;
}

char* map::get_vertice(int number)
{
    return names[number];
}

int map::get_number(char* name)
{
    for(int i = 0; i<number; i++)
        if (!strcmp(names[i],name))
            return i;
    return -1;
}

map::~map()
{
    for(int i = 0; i<number; i++)
        free(names[i]);
    free(names);
}

class City_graph
{
    int** edges;
    int* edges_len;
    int* Vertices;
    int Vertices_len;
    map cities;

    void add_vertice(int v);
    void add_edge(int first,int second);
    int is_empty(char* str);
public:
    City_graph(char* filename);
    ~City_graph();
    void print_vertices();
    void print_edges();
};

int City_graph::is_empty(char* str)
{
    while((*str == ' ')||(*str == '\n'))
        str++;
    
    if (*str=='\0')
        return 1;
    else
        return 0;
}

City_graph::City_graph(char* filename)
{
    Vertices_len = 0;
    Vertices = NULL;
    edges = NULL;
    edges_len = NULL;

    FILE* f = fopen(filename,"r");
    char buff[SIZE];
    char *tmp;
    int city_1,
        city_2;
    bool flag = 0; //0 - Vertices, 1 - Edges 

    while(!feof(f))
    {
        fgets(buff,SIZE,f);

        if (is_empty(buff))
            continue;

        if (!strcmp(buff,"Vertices\n"))
        {
            continue;
        }

        if (!strcmp(buff,"Edges\n"))
        {
            flag = 1;
            continue;
        }

        if (flag)
        {
            tmp = strtok (buff,"-\n");
            city_1 = cities.get_number(tmp);
            if (city_1 == -1)
            {
                printf("There is no vertice \'%s\'\n",tmp);
                continue;
            }
            tmp = strtok (NULL,"-\n");
            city_2 = cities.get_number(tmp);
            if (city_2 == -1)
            {
                printf("There is no vertice \'%s\'\n",tmp);
                continue;
            }   
            add_edge(city_1,city_2);
            add_edge(city_2,city_1);
        }else
        {
            buff[strlen(buff)-1]='\0';
            add_vertice(cities.add_vertice(buff));
        }
    }
    fclose(f);
}

City_graph::~City_graph()
{
    for(int i = 0; i<Vertices_len; i++)
        free(edges[i]);
    free(edges);
    free(Vertices);
}

void City_graph::add_vertice(int v)
{
   //To-do: вкрутить защиту от повторений!
    Vertices = (int*)realloc(Vertices, sizeof(int)*(Vertices_len+1));
    edges = (int**)realloc(edges, sizeof(int*)*(Vertices_len+1));
    edges_len = (int*)realloc(edges_len, sizeof(int)*(Vertices_len+1));

    Vertices[Vertices_len] = v;
    edges[Vertices_len] = NULL;
    edges_len[Vertices_len] = 0;

    Vertices_len++;
}

void City_graph::add_edge(int first,int second)
{
    for (int i=0; i<Vertices_len; i++)
    {
        if (Vertices[i] == first)
        {
            edges[i] = (int*)realloc(edges[i],sizeof(int)*(edges_len[i]+1));
            edges[i][edges_len[i]] = second;
            edges_len[i]++;
            return;
        }
    }
    printf("There is no such edge!\n");
}

void City_graph::print_vertices()
{
    printf("Vertices:\n");
    for(int i = 0; i<Vertices_len; i++)
    {
        printf("%s\n",cities.get_vertice(Vertices[i]));
    }
}

void City_graph::print_edges()
{
    for (int i = 0; i<Vertices_len; i++)
    {
        printf("Neighbors of vertice - \'%s\' :\n", cities.get_vertice(Vertices[i]));
        for(int j = 0; j<edges_len[i]; j++)
        {
            printf("%s ",cities.get_vertice(edges[i][j]));
        }
        printf("\n");
    }
}

int main()
{
    City_graph g((char*)"edges");
    g.print_vertices();
    g.print_edges();
    return 0;
}







