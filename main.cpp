#include "Graph/Graph.hpp"
#include "incidences/VectorIncidence.hpp"
#include <iostream>

using namespace graph;

int main()
{
	Graph<size_t> graph;
	for (int i = 0; i < 10; i++)
		graph.addVertex(Graph<size_t>::IncidencePtr(new VectorIncidence<size_t>));
		
	srand(100500);
	for(int i = 0; i < 20; i++)
		graph.addEdge(rand()%graph.vertexNum(), rand()%graph.vertexNum());
		
	for (size_t i = 0; i < graph.vertexNum(); i++)
	{
		std::cout << i << ": ";
		for (auto j: graph[i])
			std::cout << j << " ";
		std::cout << std::endl;
	}
	return 0;
}
