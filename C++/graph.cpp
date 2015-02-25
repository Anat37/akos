#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255

class map
{
    char* names;
    int* values;
public:
    map()
    {
        names = NULL;
        values = NULL;
    }
//To-do: дописать класс карты
};

class City_graph
{
    int** edges;
    int* edges_len;
    int* Vertices;
    int Vertices_len;

    void add_vertice(int v);
    void add_edge(int first,int second);
    int is_empty(char* str);
public:
    City_graph();
    void load_from_file(char* filename);
    void print_vertices();
    void print_edges();
};

City_graph::City_graph()
{
    Vertices_len = 0;
    Vertices = NULL;
    edges = NULL;
    edges_len = NULL;
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

void City_graph::load_from_file(char* filename)
{
    FILE* f = fopen(filename,"r");
    char buff[SIZE];
    char *city_1,
        *city_2;
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
            city_1 = strtok (buff,"-\n");
            city_2 = strtok (NULL, "-\n");   
            add_edge(atoi(city_1),atoi(city_2));
            add_edge(atoi(city_2),atoi(city_1));
        }else
        {
            buff[strlen(buff)-1]='\0';
            add_vertice(atoi(buff));
        }
    }
    fclose(f);
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
        printf("%i ",Vertices[i]);
    }
    printf("\n");
}

void City_graph::print_edges()
{
    for (int i = 0; i<Vertices_len; i++)
    {
        printf("Neighbors of vertice - \'%i\' :\n", Vertices[i]);
        for(int j = 0; j<edges_len[i]; j++)
        {
            printf("%i ",edges[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    City_graph g;
    g.load_from_file((char*)"edges");
    g.print_vertices();
    g.print_edges();
    return 0;

}







