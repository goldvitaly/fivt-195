#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include "graph.h"
#include "VertexVec.h"
#include "VertexSet.h"


void make_random_graph(Graph<int, Vertex<int> >& graph, int numVer)
{
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVec<int>());
        else
            graph.add_vertex(i, new VertexSet<int>());
    }
    for(int i = 0; i < numVer; i++)
    {
        int in_ver = rand() % numVer;
        int out_ver = rand() % numVer;
        graph.add_edge(out_ver, in_ver);
    }
}

template<class TypeNameVer>
class IsWhite: public UnaryFunc<TypeNameVer>
{
    std::vector<int>* mark;
    Graph<int, Vertex<int> >* graph;
public:
    IsWhite(Graph<int, Vertex<int> >* graph_, std::vector<int>* mark_)
    {
        graph = graph_;
        mark = mark_;
    }
    void operator()(TypeNameVer ver)
    {
        if((*mark)[ver] == 0)
        {
            dfs(ver, *graph, * mark);
        }
    }
};

void dfs(int vertex, Graph<int, Vertex<int> >& graph, std::vector<int>& mark)
{
    mark[vertex] = 1;
    IsWhite<int> isWhite(&graph, &mark);
    graph.for_each_neighbour(vertex, isWhite);
}

int main()
{
    Graph<int, Vertex<int> > graph;
    const int numVer = 100;
    make_random_graph(graph, numVer);

    std::vector<int> mark;
    mark.resize(numVer);
    dfs(0, graph, mark);
    std::cout << "component of 0:" << std::endl;
    for(int i = 0; i < numVer; i++)
    {
        if(mark[i])
        {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
