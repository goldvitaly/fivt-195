#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include "graph.h"
#include "VertexVec.h"
#include "VertexSet.h"
#include "AlgTarStrConComp.h"

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
    for(int i = 0; i < 2*numVer; i++)
    {
        int in_ver = rand() % numVer;
        int out_ver = rand() % numVer;
        graph.add_edge(out_ver, in_ver);
    }
}
int main()
{
    Graph<int, Vertex<int> > graph;
    const int numVer = 10;
    make_random_graph(graph, numVer);

    AlgTarStrConComp<int> algTar(graph);
    std::vector<std::vector<int> > strConCom = algTar.str_con_com();

    for(int i = 0; i < (int)strConCom.size(); i++)
    {
        std::cout << "component " << i + 1 << " : ";
        for(int j = 0; j < (int)strConCom[i].size(); j++)
            std::cout << strConCom[i][j] << " ";
        std::cout << std::endl;
    }
    return 0;
}
