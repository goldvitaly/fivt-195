#include "graph.hpp"
#include "graph_algorithm.hpp"
#include <iostream>
#include <algorithm>

std::vector<bool> used;

void dfs (int v, const Graph &G)
{
    used[v] = true;
    for (auto u : *G.GetIncident(v))
    {
        if (!used[u])
            dfs(u, G);
    }
}

bool test (size_t n, size_t m)
{
    Graph G;
    G.AddVertex<VIntIncident>(n);
    
    for (size_t i = 0; i < m; ++i)
    {
        size_t from, to;
        from = rand() % n;
        to = rand() % n;
        G.AddIncident(from, to);
    }
    
    GraphAlgorithm ga;
	std::vector<size_t> ord = ga.FindStronglyConnectedComponents(G);
	
	for (size_t v = 0; v < n; ++v)
	{
	    for (size_t u = v + 1; u < n; ++u)
	    {
	        bool check = true;
	        used.assign(n, 0);
	        dfs(v, G);
	        if (!used[u]) check = false;
	        
	        used.assign(n, 0);
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
    
    #define PRINT_TEST_RESULTS
    
    srand(testSrand);
    
    for (size_t t = 0; t < testNum; ++t)
    {
        #ifdef PRINT_TEST_RESULTS
        if (test(testN, testM))
            std::cerr << "test #" << t + 1 << " ok" << std::endl;
        else
            std::cerr << "test #" << t + 1 << " failed" << std::endl;
        #endif /* PRINT_TEST_RESULTS */
    }
    
    #undef PRINT_TEST_RESULTS
    
	return 0;
}
