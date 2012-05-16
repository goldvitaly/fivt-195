#include <iostream>
#include <ctime>
#include <stdlib.h>

#include "MaxFlow.h"
#include "VertexMap.h"
#include "VertexVector.h"

void make_random_graph(Graph<Vertex<int>, int>& graph, std::vector<std::vector<int> >& table)
{
    int numVer = rand()%10 + 4;
    int maxCapacity = 10;
    for(int i = 0; i < numVer; i++)
    {
        graph.add_vertex(i, new VertexVector<int>());
    }
    table.clear();
    table.resize(numVer, std::vector<int>(numVer));
    for(int i = 0; i < 4*numVer; i++)
    {
        int inVer = rand() % numVer;
        int outVer = rand() % numVer;
        int capacity = rand() % maxCapacity;
        if(inVer != outVer)
        {
            graph.add_edge(inVer, outVer, capacity);
            table[inVer][outVer] += capacity;
        }
    }
}


bool dfs(int root, int vertex, std::vector<std::vector<int>>& table, std::vector<int>& mark, std::vector<int>& from)
{
    mark[vertex] = 1;
    from[vertex] = root;
    for(int i = 0; i < table[vertex].size(); i++)
    {
        if(mark[i] == 0 && table[vertex][i] > 0)
        {
            dfs(vertex, i, table, mark, from);
        }
    }
}


int slowSearchMaxFlow(std::vector<std::vector<int> > table, int source, int target)
{
    std::vector<int> mark;
    std::vector<int> from;
    int maxFlow = 0;
    while(1)
    {
        mark.clear();
        from.clear();
        mark.resize(table.size());
        from.resize(table.size());
        dfs(source, source, table, mark, from);

        if(mark[target] == 0)
            break;
        int minEdge = table[from[target]][target];
        int vertex = from[target];
        while(from[vertex] != vertex)
        {
            minEdge = std::min(minEdge, table[from[vertex]][vertex]);
            vertex = from[vertex];
        }
        vertex = target;
        while(from[vertex] != vertex)
        {
            table[from[vertex]][vertex] -= minEdge;
            table[vertex][from[vertex]] += minEdge;
            vertex = from[vertex];
        }
        maxFlow += minEdge;
    }
    return maxFlow;
}


void test()
{
    Graph<Vertex<int>, int> graph;
    std::vector<std::vector<int> > table;
    make_random_graph(graph, table);
    MaxFlow<Vertex<int>, int, std::less<int> > maxFlow(graph);
    int ans;
    int ans2;
    if((ans = maxFlow.calculate(0, 3)) != (ans2 = slowSearchMaxFlow(table, 0, 3)))
    {
        std::cout << ans << " " << ans2 << std::endl;
        for(int i = 0; i < table.size(); i++)
        {
            for(int j = 0; j < table.size(); j++)
            {
                std::cout << table[i][j] << " " ;
            }
            std::cout << std::endl;
        }
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
