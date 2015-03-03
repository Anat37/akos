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
    void operator=(const map& sample);
    int add_vertice(char* name);
    char* get_vertice(int number);
    int get_number(char* name);
};

map::map()
{
    names = NULL;
    number = 0;
}

void map::operator=(const map& sample)
{
    try{
        number = sample.number;
        names = (char**)malloc(sizeof(char*)*number);
        if (names == NULL)
            throw "Memory error\n";
        for(int i = 0; i<number; i++)
        {
            names[i] = (char*)malloc(sizeof(char)*(strlen(sample.names[i])+1));
            if (names[i] == NULL)
                throw "Memory error\n";
            strcpy(names[i],sample.names[i]);
        }
    }
}

int map::add_vertice(char* name)
{
    number++;
    names = (char**)realloc(names,sizeof(char*)*number);
    names[number-1] = (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(names[number-1],name);
    return number-1;
}

char* map::get_vertice(int number)
{
    if (number != -1)
        return names[number];
    return NULL;
}

int map::get_number(char* name)
{
    for(int i = 0; i<number; i++)
        if (!strcmp(names[i],name))
            return i;
    printf("There is no vertice '%s'\n",name);
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
    City_graph(const City_graph& sample);
    ~City_graph();
    void print_vertices();
    void print_edges();
    void print_neighbour(char* name);
};


City_graph::City_graph(const City_graph& sample)
{
    int i,j;
    Vertices_len = sample.Vertices_len;
    edges = (int**)malloc(sizeof(int*)*Vertices_len);
    edges_len = (int*)malloc(sizeof(int)*Vertices_len);
    Vertices = (int*)malloc(sizeof(int)*Vertices_len);
    for(i = 0; i<Vertices_len; i++)
    {
        edges_len[i] = sample.edges_len[i];
        edges[i] = (int*)malloc(sizeof(int)*edges_len[i]);
        for(j = 0; j< edges_len[i]; j++)
        {
            edges[i][j] = sample.edges[i][j];
        }
        Vertices[i] = sample.Vertices[i];
    }
    cities = sample.cities;
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
            tmp = strtok(buff,"-\n");
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
            //add_edge(city_2,city_1); //раскоментить для не ориентированного графа
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
    free(edges_len);
    free(Vertices);
}

int City_graph::is_empty(char* str)
{
    while((*str == ' ')||(*str == '\n'))
        str++;
    
    if (*str=='\0')
        return 1;
    else
        return 0;
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
        print_neighbour(cities.get_vertice(Vertices[i]));
    }
}

void City_graph::print_neighbour(char* name)
{
    int v = cities.get_number(name);
    if (v == -1)
        return;
    printf("Neighbors of %s is:\n",name);
    for(int i = 0;i<edges_len[v];i++)
        printf("%s\n",cities.get_vertice(edges[v][i]));
}

int main()
{
    City_graph g((char*)"edges");
    g.print_vertices();
    g.print_edges();
    printf("Neighbors\n");
    g.print_neighbour((char*)"moscow");
    g.print_neighbour((char*)"rim");
    g.print_neighbour((char*)"prague");
    City_graph q(g);
    q.print_vertices();
    q.print_edges();
    
    return 0;
}







