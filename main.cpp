#include "graph.hpp"
#include "graph_algorithm.hpp"
#include <iostream>

int main ()
{	
    Graph G;
	G.AddVertex<VIntIncident>(4);
	
	G.AddIncident(0, 1);
	G.AddIncident(0, 2);
	G.AddIncident(1, 3);
	G.AddIncident(3, 1);
	G.AddIncident(2, 4);
	G.AddIncident(4, 2);

    	GraphAlgorithm ga;
	std::vector<size_t> ord = ga.FindStronglyConnectedComponents(G);
	
	for (auto i : ord)
		std::cout << i << " ";

	return 0;
}