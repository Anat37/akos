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
            printf("Edge between %s and %s\n",city_1,city_2);
            add_edge(atoi(city_1),atoi(city_2));
            add_edge(atoi(city_2),atoi(city_1));
        }else
        {
            buff[strlen(buff)-1]='\0';
            printf("Vertice %s\n",buff);
            add_vertice(atoi(buff));
        }
    }
    fclose(f);
}

void City_graph::add_vertice(int v)
{
    if (Vertices_len==0)
    {
        Vertices = (int*)malloc(sizeof(int));
        edges = (int**)malloc(sizeof(int*));
        edges_len = (int*)malloc(sizeof(int));
    }else
    {
        //To-do: вкрутить защиту от повторений!
        Vertices = (int*)realloc(Vertices, sizeof(int)*(Vertices_len+1));
        edges = (int**)realloc(edges, sizeof(int*)*(Vertices_len+1));
        edges_len = (int*)realloc(edges_len, sizeof(int)*(Vertices_len+1));
    }

    edges_len[Vertices_len] = 0;
    Vertices[Vertices_len] = v;
    Vertices_len++;
}

void City_graph::print_vertices()
{
    int i;
    for(i = 0; i<Vertices_len; i++)
    {
        printf("vertice %i is %i\n",i,Vertices[i]);
    }
}

void City_graph::add_edge(int first,int second)
{
    for (int i=0; i<Vertices_len; i++)
    {
        if (Vertices[i] == first)
        {
            printf("found vertice!\n");
            if (edges_len[i] == 0)
            {
                edges[i] = (int*)malloc(sizeof(int));
            }else
            {
                edges[i] = (int*)realloc(edges[i],sizeof(int)*(edges_len[i]+1));
            }
            edges[i][edges_len[i]] = second;
            edges_len[i]++;
            return;
        }
    }
    printf("There is no such edge!\n");
}

void City_graph::print_edges()
{
    int i,j;
    for (i = 0; i<Vertices_len; i++)
    {
        printf("Vertice %i\n", Vertices[i]);
        printf("Neighbors:\n");
        for(j = 0; j<edges_len[i]; j++)
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












