#include "graph.hpp"
#include "useful.hpp"
#include "graphalgo.hpp"

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <cstdlib>
#include <ctime>

class Timer
{
	private:
		unsigned int start_time;
	public:
		Timer() { start_time = clock(); };
		void start() { start_time = clock(); };
		double get_time() { return (clock() - start_time) / double(CLOCKS_PER_SEC); };
};

template <class VertexRealization>
char test_graph_realization(int vertex_number, int requests)
{	
	Timer t;
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
		char has_in_model = model_realization[v].find(u) != model_realization[v].end();
		char has_in_testing = testing_realization.has_edge(v,u);
		if (has_in_model != has_in_testing)
		{
			std::cerr << "Test failed on step " << i;
			if (has_in_model) 
				std::cerr << ": existing edge (" << v << ", " << u << ") not found" << std::endl;
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
	std::cerr << "Test OK" << " within " << t.get_time() << " seconds" << std::endl;
	return 1;
};

char strongly_connection_test(int vertex_number, int requests)
{
	Timer t;
	class NaiveSolver
	{
		public:
			static void dfs(unsigned int current_vertex, std::vector<char>& used, const Graph& g)
			{
				if (used[current_vertex]) return;
				used[current_vertex] = 1;
				for (unsigned int next_vertex: g[current_vertex])
					if (!used[next_vertex])
						dfs(next_vertex, used, g);
			}
			static std::vector<unsigned int> solve(const Graph& g)
			{
				int vertex_number = g.size();
				std::vector<unsigned int> coloring(vertex_number, 0);
				std::vector<std::vector<char>> connectivity_table(vertex_number, std::vector<char>(vertex_number, false));
				for (int i = 0; i < vertex_number; i ++)
					dfs(i, connectivity_table[i], g);
				std::vector<char> used(vertex_number, 0);
				int colors = 0;
				for (int i = 0; i < vertex_number; i ++)
					if (!used[i])
					{
						for (int j = 0; j < vertex_number; j ++)
							if (connectivity_table[i][j] && connectivity_table[j][i])
							{
								used[j] = 1;
								coloring[j] = colors;
							}
						colors ++;
					}
				return coloring;
			};
	};
	class SmartVectorComparator
	{
		private:
			std::vector <unsigned int> renumerate(const std::vector<unsigned int>& coloring)
			{
				std::vector<unsigned int> result;
				result.reserve(coloring.size());
				int colors = 0;
				std::map<unsigned int, unsigned int> compressor;
				for (unsigned int c: coloring)
				{
					if (compressor.count(c) > 0)
					{
						result.push_back(colors);
						compressor[c] = colors ++;
					}
					else
						result.push_back(compressor[c]);
				}
				return result;
			};
		public:
			bool operator () (const std::vector<unsigned int>& a, const std::vector<unsigned int>& b)
			{
				return renumerate(a) == renumerate(b);
			};
	};
	Graph g(vertex_number);
	for (int step = 0; step < requests; step ++)
	{
		int u = rand() % vertex_number;
		int v = rand() % vertex_number;
		if (!g.has_edge(v,u))
			g.add_edge(v, u);
		else
			g.del_edge(v, u);
		std::vector<unsigned int> model_coloring = NaiveSolver::solve(g);
		std::vector<unsigned int> testing_coloring = graph::algorithm::calculate_strongly_connected_components_coloring(g);
		if (!SmartVectorComparator()(model_coloring, testing_coloring))
		{
			for (int i = 0; i < vertex_number; i ++)
				std::cerr << std::vector<unsigned int>(g[i].begin(), g[i].end()) << std::endl;
			std::cerr << "Test failed on step " << step << std::endl << "Expected: " << model_coloring << std::endl << "Got: " << testing_coloring << std::endl;
			return 0;
		}
	}
	std::cerr.precision(2);
	std::cerr << "Test OK" << " within " << t.get_time() << " seconds" << std::endl;
	return 1;
};


int main()
{
	int CODE = 0;
	std::cerr << "Testing graph realization" << std::endl;
	if (!(CODE = test_graph_realization<graph::VertexWithUnsortedVector>(100,1e5))) return CODE;
	if (!(CODE = test_graph_realization<graph::VertexWithSortedVector>(100,1e5))) return CODE;
	if (!(CODE = test_graph_realization<graph::VertexWithSet>(100,1e5))) return CODE;
	if (!(CODE = test_graph_realization<graph::VertexWithMultiset>(100,1e5))) return CODE;
	std::cerr << "Graph realization seemes to be OK" << std::endl << "Testing Tarjan algorithm realization" << std::endl;
	if (!(CODE = strongly_connection_test(5,   1e3))) return CODE;
	if (!(CODE = strongly_connection_test(10,  1e3))) return CODE;
	if (!(CODE = strongly_connection_test(50,  1e3))) return CODE;
	if (!(CODE = strongly_connection_test(100, 1e3))) return CODE;
	std::cerr << "Tajan algorithm seemes to be OK" << std::endl;
	return 0;
};
