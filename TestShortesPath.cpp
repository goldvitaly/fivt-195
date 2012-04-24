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
    if(number == 0) // ans from 0 : 0 1 4 2 3
    {
        graph.add_edge(0, 1, 1);
        graph.add_edge(1, 2, 10);
        graph.add_edge(1, 3, 1);
        graph.add_edge(3, 4, 1);
        graph.add_edge(4, 2, 1);
    }
    else if(number == 1) // ans from 0 : 0 1 2 not 3
    {
        graph.add_edge(0, 1, 1);
        graph.add_edge(1, 2, 1);
        graph.add_edge(1, 4, 5);
        graph.add_edge(2, 4, 1);
        graph.add_edge(0, 2, 3);
    }
    else if(number == 2) //ans from 0 : 0 4 3 1 not
    {
        graph.add_edge(0, 3, 1);
        graph.add_edge(3, 2, 2);
        graph.add_edge(0, 2, 10);
        graph.add_edge(2, 1, 1);
        graph.add_edge(1, 0, 1);
    }
}

class PlusPairInt
{
public:
     std::pair<int, int> operator()(AccessPath<int, std::pair<int, int> > accessPath)
     {
         std::pair<int, int> pair = accessPath.path();
         int integer = accessPath.weight();

         // просто проверка доступа к пути. Выводит путь в обратном порядке.
         std:: cout << "path: ";
         while(accessPath.prev() != NULL)
         {
             std::cout << accessPath.lastVertex() << " ";
             accessPath = *(accessPath.prev());
         }
         std::cout << accessPath.lastVertex() << " ";
         std::cout << std::endl;

         return std::pair<int,int>(pair.first + integer, pair.second + integer);
     }
};

void test(int number)
{
    Graph<Vertex<int>, int> graph;
    make_graph(graph, number);
    //pair<int, int> взят просто так. В нем всегда first == second.
    ShortestPath<Vertex<int>, int, std::pair<int, int>, PlusPairInt> shortestPath(graph);
    std::vector<std::pair<int, int> > ans = shortestPath.count(0, std::make_pair(-1, -1));
    for(int i = 0; i < ans.size(); i++)
    {
        std::cout << ans[i].first << " " << ans[i].second << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    for(int i = 0; i < 3; i++)
    {
        test(i);
    }
    return 0;
}
