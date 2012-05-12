#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include "ctime"
#include "graph.h"
#include "VertexVector.h"
#include "VertexMap.h"
#include "utility"
#include "ShortestPath.h"

typedef unsigned int TypeNameVer;

std::vector<int> make_graph1(Graph<Vertex<int>, int>& graph)
{
    graph.clear();
    int numVer = 5;
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector<int>());
        else
            graph.add_vertex(i, new VertexMap<int>());
    }
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 10);
    graph.add_edge(1, 3, 1);
    graph.add_edge(3, 4, 1);
    graph.add_edge(4, 2, 1);
    return std::vector<int>() = {0, 1, 4, 2, 3};
}

std::vector<int> make_graph2(Graph<Vertex<int>, int>& graph)
{
    graph.clear();
    int numVer = 5;
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector<int>());
        else
            graph.add_vertex(i, new VertexMap<int>());
    }
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 4, 5);
    graph.add_edge(2, 4, 1);
    graph.add_edge(0, 2, 3);
    return std::vector<int>() = {0, 1, 2, -1, 3};
}

std::vector<int> make_graph3(Graph<Vertex<int>, int>& graph)
{
    graph.clear();
    int numVer = 5;
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector<int>());
        else
            graph.add_vertex(i, new VertexMap<int>());
    }
    graph.add_edge(0, 3, 1);
    graph.add_edge(3, 2, 2);
    graph.add_edge(0, 2, 10);
    graph.add_edge(2, 1, 1);
    graph.add_edge(1, 0, 1);
    return std::vector<int>() = {0, 4, 3, 1, -1};
}

class PlusPairInt
{
public:
     std::pair<int, int> operator()(AccessPath<int, std::pair<int, int> > accessPath)
     {
         std::pair<int, int> pair = accessPath.path();
         int integer = accessPath.weight();
         return std::pair<int,int>(pair.first + integer, pair.second + integer);
     }
};

void test(Graph<Vertex<int>, int>& graph, std::vector<int> ans)
{
    //pair<int, int> взят просто так. В нем всегда first == second.
    ShortestPath<Vertex<int>, int, std::pair<int, int>, PlusPairInt> shortestPath(graph);
    std::vector<std::pair<int, int> > ansShortestPath = shortestPath.count(0, std::make_pair(-1, -1));
    for(int i = 0; i < graph.size(); i++)
    {
        if(ans[i] != ansShortestPath[i].first || ansShortestPath[i].first != ansShortestPath[i].second)
        {
            std::cerr << ans[i] << " " << ansShortestPath[i].first << std::endl;
            exit(1);
        }
    }
}

int main()
{
    Graph<Vertex<int>, int> graph;
    std::vector<int> ans(5);
    ans = make_graph1(graph);
    test(graph, ans);

    ans = make_graph2(graph);
    test(graph, ans);

    ans = make_graph3(graph);
    test(graph, ans);

    return 0;
}
