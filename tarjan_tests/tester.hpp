#ifndef TARJAN_TESTER_HPP
#define TARJAN_TESTER_HPP

#include "../Graph/Graph.hpp"
#include "../algorithm/tarjan/tarjan.hpp"
#include "../algorithm/utils/dfs.hpp"
#include "../incidences/VectorIncidence.hpp"

#include <iostream>


namespace tarjan_tester
{
bool test (const graph::Graph& graph)
{
	std::cerr << "Testing, V = " << graph.vertexNum() << std::endl;
	std::vector<size_t> res = tarjanStronglyConnectedComponents(graph);
	for (size_t i = 0; i < graph.vertexNum(); i++)
		for (size_t j = 0; j < i; j++)
		{
			bool reachIJ = reachable(i, j, graph);
			bool reachJI = reachable(j, i, graph);
			if ( (reachIJ && reachJI && res[i] != res[j]) ||
				 (!(reachIJ && reachJI) && res[i] == res[j]) )
			{
				std::cerr << "Result: WA" << std::endl;
				return false;
			}
		}
		
	std::cerr << "Result: OK" << std::endl;
	return true;
	
}
} // namespace tarjan_tester

#endif
