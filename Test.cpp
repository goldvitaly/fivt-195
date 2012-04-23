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
#include "StronglyConnectedComp.h"
#include "SlowAlg.h"
#include "utility"
#include "ShortestPath.h"

typedef unsigned int TypeNameVer;
void make_random_graph(Graph<Vertex<int> , int >& graph, int numVer)
{
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector<int>());
        else
            graph.add_vertex(i, new VertexMap<int>());
    }
    for(int i = 0; i < 2*numVer; i++)
    {
        int in_ver = rand() % numVer;
        int out_ver = rand() % numVer;
        graph.add_edge(out_ver, in_ver, 1);
    }
}

void make_graph(Graph<Vertex<int>, int>& graph)
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
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 10);
    graph.add_edge(1, 3, 1);
    graph.add_edge(3, 4, 1);
    graph.add_edge(4, 2, 1);
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
        std::cerr << "component " << i + 1 << " : ";
        for(int j = 0; j < (int)vector[i].size(); j++)
            std::cerr << vector[i][j] << " ";
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}


void test(Graph<Vertex<int> , int >& graph)
{
    StronglyConnectedComp<Vertex<int> , int> algTar(graph);
    std::vector<std::vector<TypeNameVer> > ansAlgTar = algTar.listComponents();
    SlowAlg<Vertex<int> , int> slowAlg(graph);
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
        Graph<Vertex<int>, int > graph;
        for(int j = 0; j < numVer; j++)
            graph.add_vertex(j, new VertexVector<int>);
        int pos = 0;
        while(mask != 0)
        {
            if(mask&1)
                graph.add_edge(listEdge[pos].first, listEdge[pos].second, 1);
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
        Graph<Vertex<int>, int > graph;
        make_random_graph(graph, numVer);
        test(graph);
    }
}



class PlusPairInt
{
public:
     std::pair<int, int> operator()(std::pair<int, int> a, int b)
     {
         return std::pair<int,int>(a.first + b, a.second + b);
     }
};

class CmpPair
{
public:
    bool operator()(std::pair<int,int> a, std::pair<int,int> b) const
    {
        return a < b;
    }
};

int main()
{
    testRandomGraphs(10);
    std::cerr << "testRandomGraphs 10 Ok" << std::endl;
    for(int i = 1; i < 5; i++)
    {
        testAllGraphs(i);
        std::cerr << "testAllgraphs " << i << " Ok" << std::endl;
    }
    Graph<Vertex<int>, int> graph;
    make_graph(graph);
    ShortestPath<Vertex<int>, int, std::pair<int, int>, PlusPairInt, CmpPair> shortestPath(graph);
    std::vector<std::pair<int, int> > ans = shortestPath.count(0);
    for(int i = 0; i < ans.size(); i++)
    {
        std::cout << ans[i].first << " " << ans[i].second << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
