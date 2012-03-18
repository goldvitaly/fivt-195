#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include "ctime"
#include "graph.h"
#include "VertexVec.h"
#include "VertexSet.h"
#include "AlgTarStrConComp.h"
#include "SlowAlg.h"

void make_random_graph(Graph<Vertex>& graph, int numVer)
{
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector());
        else
            graph.add_vertex(i, new VertexSet());
    }
    for(int i = 0; i < 2*numVer; i++)
    {
        int in_ver = rand() % numVer;
        int out_ver = rand() % numVer;
        graph.add_edge(out_ver, in_ver);
    }
}

void sort(std::vector<std::vector<unsigned int> >& Vec)
{
    for(size_t i = 0; i < Vec.size(); i++)
    {
        sort(Vec[i].begin(), Vec[i].end());
    }
    sort(Vec.begin(), Vec.end());
}

void print(std::vector<std::vector<unsigned int> >& vector)
{
    for(int i = 0; i < (int)vector.size(); i++)
    {
        std::cout << "component " << i + 1 << " : ";
        for(int j = 0; j < (int)vector[i].size(); j++)
            std::cout << vector[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    for(int trial = 0; trial < 1000; trial++)
    {
        Graph<Vertex> graph;
        const int numVer = 5;
        make_random_graph(graph, numVer);

        StronglyConnectedComp algTar(graph);
        std::vector<std::vector<unsigned int> > ansAlgTar = algTar.listComponents();
        SlowAlg slowAlg(graph);
        std::vector<std::vector<unsigned int> > ansSlowAlg = slowAlg.listComponents();

        sort(ansAlgTar);
        sort(ansSlowAlg);

        if(ansAlgTar != ansSlowAlg)
        {
            print(ansAlgTar);
            print(ansSlowAlg);
            return 0;
        }
    }
    std::cout << "Ok" << std::endl;
    return 0;
}
