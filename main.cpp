#include "graph.hpp"
#include "graph_algorithm.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>

bool dijkstraTest (size_t vertexNum, size_t edgeNum)
{
    size_t g[vertexNum][vertexNum];
    for (size_t i = 0; i < vertexNum; ++i)
    {
        for (size_t j = 0; j < vertexNum; ++j)
        {
            g[i][j] = 1e9;
        }
    }
        
    Graph<Edge> G;
    for (size_t i = 0; i < vertexNum; ++i)  
    {
        G.AddVertex<VIntIncident<Edge>>();
    }
    
    for (size_t i = 0; i < edgeNum; ++i)  
    {
        size_t f, t, v;
        f = rand() % vertexNum;
        t = rand() % vertexNum;
        v = rand() % 100;
        
        g[f][t] = std::min(g[f][t], v);
        G.AddIncident(f, t, Edge(v));
    }

    G.AddIncident(0, vertexNum - 1, Edge(1e9));
    
    for (size_t i = 0; i < vertexNum; ++i)
    {
        g[i][i] = 0;
    }
    
    for (size_t k = 0; k < vertexNum; ++k)
    {
        for (size_t i = 0; i < vertexNum; ++i)
        {
            for (size_t j = 0; j < vertexNum; ++j)
            {
                g[i][j] = std::min(g[i][j], g[i][k] + g[k][j]);
            }
        }
    }
    
    DijkstraAlgorithm<Edge, Path<Edge>, PathComp<Path<Edge>>, Relax<Edge, Path<Edge>>> da;
    
    if (da(G, 0, vertexNum - 1)() != g[0][vertexNum - 1])
    {
        std::cerr << da(G, 0, vertexNum - 1)() << std::endl;
        std::cerr << g[0][vertexNum - 1] << std::endl;
    }
    
    return da(G, 0, vertexNum - 1)() == g[0][vertexNum - 1];
}

int main ()
{    
    const size_t Tests = 1000;
    for (size_t test = 1; test <= Tests; ++test)
    {
        std::cout << "Test# " << test << ": ";
        
        if (dijkstraTest(100, 2000))
        {
            std::cout << " ok." << std::endl;
        }
        else
        {
            std::cout << " failed!" << std::endl;
            return 0;
        }
    }

    return 0;
}
