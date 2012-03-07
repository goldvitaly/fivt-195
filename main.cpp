#include "graph.hpp"

#include <iostream>

int main()
{
	Graph g;
	g.add_vertex<IncidenceVector> ();
	g.add_vertex<IncidenceVector> ();
	g.add_vertex<IncidenceVector> ();
	//g.add_edge(0, 1);
	//g.add_edge(0, 2);
	auto beg = g[0].begin();
	auto end = g[0].end();
	std::cerr << (beg != end) << std::endl;
	
	return 0;
}
