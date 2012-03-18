#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include "ctime"
#include "graph.h"
#include "VertexVector.h"
#include "VertexSet.h"
#include "StronglyConnectedComp.h"
#include "SlowAlg.h"
#include "utility"

typedef unsigned int TypeNameVer;

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

void sort(std::vector<std::vector<TypeNameVer> >& Vec)
{
    for(size_t i = 0; i < Vec.size(); i++)
    {
        sort(Vec[i].begin(), Vec[i].end());
    }
    sort(Vec.begin(), Vec.end());
}

void print(std::vector<std::vector<TypeNameVer> >& vector)
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


void test(Graph<Vertex>& graph)
{
    StronglyConnectedComp algTar(graph);
    std::vector<std::vector<TypeNameVer> > ansAlgTar = algTar.listComponents();
    SlowAlg slowAlg(graph);
    std::vector<std::vector<TypeNameVer> > ansSlowAlg = slowAlg.listComponents();
    sort(ansAlgTar);
    sort(ansSlowAlg);

    if(ansAlgTar != ansSlowAlg)
    {
        std::cerr << "Wrong" << std::endl;
        print(ansAlgTar);
        print(ansSlowAlg);
        exit(1);
    }
}

void testAllGraphs(int numVer = 3)
{
    std::vector<std::pair<TypeNameVer, TypeNameVer> > listEdge;
    for(int i = 0; i < numVer; i++)
    {
        for(int j = 0; j < numVer; j++)
        {
            if(i != j)
                listEdge.push_back(std::make_pair(i, j));
        }
    }
    for(long long i = 0; i < (1LL<<listEdge.size()); i++)
    {
        long long mask = i;
        Graph<Vertex> graph;
        for(int j = 0; j < numVer; j++)
            graph.add_vertex(j, new VertexVector);
        int pos = 0;
        while(mask != 0)
        {
            if(mask&1)
                graph.add_edge(listEdge[pos].first, listEdge[pos].second);
            pos++;
            mask >>=1;
        }
        test(graph);
    }
}

void testRandomGraphs(int numVer = 3)
{
    for(int trial = 0; trial < 1000; trial++)
    {
        Graph<Vertex> graph;
        make_random_graph(graph, numVer);
        test(graph);
    }
}

int main()
{
    testRandomGraphs();
    std::cout << "testRandomGraphs Ok" << std::endl;
    for(int i = 1; i < 5; i++)
    {
        testAllGraphs(i);
        std::cout << "testAllgraphs " << i << " Ok" << std::endl;
    }
    return 0;
}
