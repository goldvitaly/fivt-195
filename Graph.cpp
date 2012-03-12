#include "Graph.hpp"

int main ()
{	
	Graph G;
	G.AddVertex<MyIncident>(0);
	G.AddVertex<MyIncident>(1);
	G.AddIncident(1, 0);
	
	std::vector <size_t> ord;
	TopSort(G, 0, ord);
	for (size_t i = 0; i < ord.size(); ++i)
		std::cout << ord[i] << " ";
	
	return 0;
}
