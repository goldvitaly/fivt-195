#include "graph.hpp"
#include "graph_algorithm.hpp"
#include <iostream>
#include <algorithm>

std::vector<bool> used;

void dfs (int v, const Graph &G)
{
    used[v] = true;
    for (auto u : G.GetIncident(v))
    {
        if (!used[u])
            dfs(u, G);
    }
}

template <typename IncidentType>
bool test (size_t vertexNum, size_t edgeNum)
{
    Graph G;
    for (size_t i = 0; i < vertexNum; ++i)
    {
        G.AddVertex<IncidentType>();
    }
    
    for (size_t i = 0; i < edgeNum; ++i)
    {
        size_t from, to;
        from = rand() % vertexNum;
        to = rand() % vertexNum;
        G.AddIncident(from, to);
    }

    GraphAlgorithm ga;
	std::vector<size_t> ord = ga.FindStronglyConnectedComponents(G);
	
	for (size_t v = 0; v < vertexNum; ++v)
	{
	    for (size_t u = v + 1; u < vertexNum; ++u)
	    {
	        bool check = true;
	        used.assign(vertexNum, 0);
	        dfs(v, G);
	        if (!used[u]) check = false;
	        
	        used.assign(vertexNum, 0);
	        dfs(u, G);
	        if (!used[v]) check = false;
	        
            if ((ord[v] == ord[u]) != (check))
                return false;
	    }
	}
	
	return true;
}

int main ()
{	
    const size_t testNum = 1000000;
    const size_t testN = 30;
    const size_t testM = 100;
    const size_t testSrand = 333;
    
    srand(testSrand);
    
    for (size_t t = 0; t < testNum; ++t)
    {
        //Vector of Int
        std::cerr << "VIntIncident ";
        if (test<VIntIncident>(testN, testM))
            std::cerr << "test #" << t + 1 << " ok" << std::endl;
        else
            std::cerr << "test #" << t + 1 << " failed" << std::endl;
            
        //Vector of Bool
        std::cerr << "VBoolIncident ";
        if (test<VBoolIncident>(testN, testM))
            std::cerr << "test #" << t + 1 << " ok" << std::endl;
        else
            std::cerr << "test #" << t + 1 << " failed" << std::endl;
    }
    
	return 0;
}
