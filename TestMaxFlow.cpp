#include <iostream>

#include "MaxFlow.h"
#include "VertexMap.h"
#include "VertexVector.h"

void make_graph(Graph<Vertex<int>, int>& graph, int number = 0)
{
    int numVer = 5;
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector<int>());
        else
            graph.add_vertex(i, new VertexMap<int>());
    }
    if(number == 0) // ans 2
    {
        graph.add_edge(0, 1, 1);
        graph.add_edge(0, 2, 2);
        graph.add_edge(1, 3, 1);
        graph.add_edge(2, 3, 1);
    }
    else if(number == 1) // ans 1
    {
        graph.add_edge(0, 1, 1);
        graph.add_edge(1, 2, 2);
        graph.add_edge(0, 2 , 1);
        graph.add_edge(2, 3, 1);
    }
    else if(number == 2) //ans 2
    {
        graph.add_edge(0, 1, 1);
        graph.add_edge(1, 2, 2);
        graph.add_edge(0, 2, 1);
        graph.add_edge(2, 3, 2);
    }
}

std::vector<int> ans;

void test(int number)
{
    Graph<Vertex<int>, int> graph;
    make_graph(graph, number);
    MaxFlow<Vertex<int>, int, std::less<int> > maxFlow(graph);
   ans.push_back(maxFlow.calculate(0, 3));
}

int main()
{
    for(int i = 0; i < 3; i++)
    {
        test(i);
    }
    if(ans != (std::vector<int>() = {2, 1, 2}))
        exit(1);
    return 0;
}
