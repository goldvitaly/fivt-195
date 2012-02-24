#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "graph.h"

using namespace std;


void dfs(int vertex, Graph<int, Vertex<int> >& graph, vector<int>& mark)
{
    mark[vertex] = 1;
    vector<int>* neighbours;
    neighbours = graph.list_neighbour(vertex);
    for(size_t i = 0; i < neighbours->size(); i++)
    {
        if(mark[(*neighbours)[i]] != 1)
        {
            dfs((*neighbours)[i], graph, mark);
        }
    }
}

int main()
{
    Graph<int, Vertex<int> > graph;

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);

    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 1);

    vector<int> mark;
    mark.resize(graph.size());
    dfs(2, graph, mark);
    cout << "component of 2:" << endl;
    for(int i = 1; i <= 4; i++)
    {
        if(mark[i])
        {
            cout << i << " ";
        }
    }
    cout << endl;
    return 0;
}
