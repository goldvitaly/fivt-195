#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <vector>

#include "graph.h"
#include "MaxFlow.h"
#include "VertexMap.h"
#include "VertexVector.h"

void make_random_graph(Graph<Vertex<int>, int>& graph, std::vector<std::vector<int> >& tableEdge)
{
    graph.clear();
    tableEdge.clear();
    int numVer = rand() % 10 + 4;
    int maxCapacity = 10;
    for(int i = 0; i < numVer; i++)
    {
        graph.add_vertex(i, new VertexVector<int>());
    }
    tableEdge.resize(numVer, std::vector<int>(numVer));
    for(int i = 0; i < 4*numVer; i++)
    {
        int inVer = rand() % numVer;
        int outVer = rand() % numVer;
        int capacity = rand() % maxCapacity;
        if(inVer != outVer)
        {
            graph.add_edge(inVer, outVer, capacity);
            tableEdge[inVer][outVer] += capacity;
        }
    }
}


bool dfs(int prevVertex, int vertex, std::vector<std::vector<int>>& tableEdge,
         std::vector<int>& mark, std::vector<int>& from)
{
    mark[vertex] = 1;
    from[vertex] = prevVertex;
    for(int nextVertex = 0; nextVertex < tableEdge[vertex].size(); nextVertex++)
    {
        if(mark[nextVertex] == 0 && tableEdge[vertex][nextVertex] > 0)
        {
            dfs(vertex, nextVertex, tableEdge, mark, from);
        }
    }
}


int slowSearchMaxFlow(const std::vector<std::vector<int> >& tableEdge_, int source, int target)
{
    std::vector<std::vector<int> > tableEdge = tableEdge_;
    std::vector<int> mark;
    std::vector<int> from;
    int maxFlow = 0;
    while(1)
    {
        mark.assign(tableEdge.size(), 0);
        from.assign(tableEdge.size(), 0);
        dfs(source, source, tableEdge, mark, from);

        if(mark[target] == 0)
            break;

        int minEdge = tableEdge[from[target]][target];
        int vertex = from[target];
        while(from[vertex] != vertex)
        {
            minEdge = std::min(minEdge, tableEdge[from[vertex]][vertex]);
            vertex = from[vertex];
        }

        vertex = target;
        while(from[vertex] != vertex)
        {
            tableEdge[from[vertex]][vertex] -= minEdge;
            tableEdge[vertex][from[vertex]] += minEdge;
            vertex = from[vertex];
        }
        maxFlow += minEdge;
    }
    return maxFlow;
}


void test()
{
    Graph<Vertex<int>, int> graph;
    std::vector<std::vector<int> > tableEdge;
    make_random_graph(graph, tableEdge);
    MaxFlow<Vertex<int>, int, std::less<int> > maxFlow(graph);
    if(maxFlow.calculate(0, 3) != slowSearchMaxFlow(tableEdge, 0, 3))
    {
        exit(1);
    }
}


int main()
{
    srand(time(NULL));
    for(int i = 0; i < 1000; i++)
    {
        test();
    }
    return 0;
}
