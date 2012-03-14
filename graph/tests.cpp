#include "graph.hpp"
#include "useful.hpp"

#include <vector>
#include <set>
#include <iostream>
#include <cstdlib>

template <class VertexRealization>
bool test(int vertex_number, int requests)
{	
	class VertexChooser
	{
	  public:
		Graph::Vertex* operator() (unsigned int degree, unsigned int vertices)
		{
			return new VertexRealization();
		}		
	};
	Graph testing_realization(vertex_number, VertexChooser());
	std::vector < std::set <unsigned int> > model_realization(vertex_number);
	for (int i = 0; i < requests; i ++)
	{
		int v = rand() % vertex_number;
		int u = rand() % vertex_number;
		bool has_in_model = model_realization[v].find(u) != model_realization[v].end();
		bool has_in_testing = testing_realization.has_edge(v,u);
		if (has_in_model != has_in_testing)
		{
			std::cerr << "Test failed on step " << i;
			if (has_in_model) 
				std::cerr << " existing edge (" << v << ", " << u << ") not found" << std::endl;
			else
				std::cerr << " non-existing edge (" << v << ", " << u << ") found" << std::endl;
			return 0;
		}
		if (has_in_model)
		{
			model_realization[v].erase(model_realization[v].find(u));
			testing_realization.del_edge(v,u);
		}			
		else
		{
			model_realization[v].insert(u);
			testing_realization.add_edge(v,u);
		}
		std::set <unsigned int> adjacency_list(testing_realization[v].begin(), testing_realization[v].end());
		if (adjacency_list != model_realization[v])
		{

			std::cerr << "Test failed on step " << i << std::endl << "Expected adjacency list: " << model_realization[v] << ", got: " << adjacency_list << std::endl; 
			for (Graph::iterator it = testing_realization[v].begin(); it != testing_realization[v].end(); it ++)
				std::cerr << v << " ";
			std::cerr << std::endl;
			return 0;
		}
	}
	std::cerr << "Test passed" << std::endl;
	return 1;
};


int main()
{
	int CODE = 0;
	if (!(CODE = test<graph::VertexWithUnsortedVector>(100,1e5))) return CODE;
	if (!(CODE = test<graph::VertexWithSortedVector>(100,1e5))) return CODE;
	if (!(CODE = test<graph::VertexWithSet>(100,1e5))) return CODE;
	if (!(CODE = test<graph::VertexWithMultiset>(100,1e5))) return CODE;
	return 0;
};
