#include "graph.hpp"
#include <iostream>

int main ()
{	
    Graph G;
	G.AddVertex<MyIncident>(4);
	/*G.AddIncident(0, 1);
	G.AddIncident(0, 2);
	G.AddIncident(1, 3);
	G.AddIncident(3, 1);
	G.AddIncident(2, 4);
	G.AddIncident(4, 2);*/

    //	GraphAlgorithm ga;
	//std::vector<size_t> ord = ga.FindStronglyConnectedComponents(G);
	
	//for (size_t i = 0; i < ord.size(); ++i)
	//	std::cout << ord[i] << " ";

	return 0;
}