#include "Graph/Graph.hpp"
#include "algorithm/generators/generators.hpp"
#include "incidences/VectorIncidence.hpp"
#include "tarjan_tests/tester.hpp"
#include <iostream>

using namespace graph;
using namespace generators;

int main()
{
	Random random = Random();
	auto VecIncGen = [](){return std::unique_ptr<IIncidence>(new VectorIncidence());};
	
	for (int i = 0; i < 10; i++)
	{
		Graph graph = random.gen(20, 400, VecIncGen);
		if (!tarjan_tester::test(graph))
		{
			std::cout << graph.size() << std::endl;
			for (size_t j = 0; j < graph.size(); j++)
			{
				std::cout << j << ": ";
				for (auto k: graph[j])
					std::cout << k << " ";
				std::cout << std::endl;
			}
			std::cout << std::endl;
			break;
		}
	}
	
	return 0;
}
