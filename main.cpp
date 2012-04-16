#include "Graph/Graph.hpp"
#include "algorithm/generators/generators.hpp"
#include "incidences/VectorIncidence.hpp"
#include <iostream>

using namespace graph;
using namespace generators;

int main()
{
	Random random = Random();
	Graph graph;
	
	for (int i = 0; i < 5; i++)
	{
		graph = random.gen(4, 5, [](){return std::unique_ptr<IIncidence>(new VectorIncidence());});
		for(size_t j = 0; j < graph.size(); j++)
		{
			std::cout << j << ": ";
			for (auto k: graph[j])
				std::cout << k << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	
	return 0;
}
