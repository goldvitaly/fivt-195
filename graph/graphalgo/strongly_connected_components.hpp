#ifndef __STRONGLY_CONNECTED_COMPONENTS_H__
#define __STRONGLY_CONNECTED_COMPONENTS_H__

#include <vector>
#include <stack>

namespace graph
{
namespace algorithm
{
namespace impl
{
	struct StronglyConnectedComponentsColorer
	{
		StronglyConnectedComponentsColorer(unsigned int vertex_number): 
			state(vertex_number, 0), minlink(vertex_number, 0), index(vertex_number, 0) {};
		unsigned int current_index;
		unsigned int current_component;
		std::vector<char> state;
		std::stack <unsigned int> stack;
		std::vector<unsigned int> minlink;
		std::vector<unsigned int> index;
	};

	void process_strongly_connected_compontent(unsigned int vertex, const Graph& g, 
			StronglyConnectedComponentsColorer& colorer, std::vector<unsigned int>& coloring)
	{
		if (colorer.state[vertex]) return;
		colorer.state[vertex] = 1;
		colorer.minlink[vertex] = colorer.index[vertex] = colorer.current_index ++;
		colorer.stack.push(vertex);
		for (graph::Graph::edge_type neighbor: g[vertex])
		{
			if (!colorer.state[neighbor.to])
			{
				process_strongly_connected_compontent(neighbor.to, g, colorer, coloring);
				colorer.minlink[vertex] = std::min(colorer.minlink[vertex], colorer.minlink[neighbor.to]);			
			}
			else
			{
				if (colorer.state[neighbor.to] == 1)
				{
					colorer.minlink[vertex] = std::min(colorer.minlink[vertex], colorer.index[neighbor.to]);
				}
			}
		}
		if (colorer.index[vertex] == colorer.minlink[vertex])
		{
			unsigned int current_vertex;
			do
			{
				current_vertex = colorer.stack.top();
				colorer.stack.pop();
				colorer.state[current_vertex] = 2;
				coloring[current_vertex] = colorer.current_component;
			}
			while (current_vertex != vertex);
			colorer.current_component ++;
		}
	}
}
	inline std::vector<unsigned int> get_strongly_connected_components_coloring(const Graph& g)
	{
		std::vector<unsigned int> coloring(g.size(), 0);
		impl::StronglyConnectedComponentsColorer colorer(g.size());
		for (unsigned vertex = 0; vertex < g.size(); ++ vertex)
			process_strongly_connected_compontent(vertex, g, colorer, coloring);
		return coloring;
	}
}
}

#endif // __STRONGLY_CONNECTED_COMPONENTS_H__
