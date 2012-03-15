#ifndef GRAPHALGO_H
#define GRAPHALGO_H

#include "graph.hpp"
#include <stack>
#include <vector>

namespace graph
{
namespace algorithm
{
	std::vector<unsigned int> calculate_strongly_connected_components_coloring(const Graph& g)
	{
		typedef unsigned int uint;
		std::vector<uint> coloring(g.size(), 0);
		class colorer
		{
			const Graph& g;
			uint current_index;
			uint current_component;			
			std::vector<char> state;
			std::stack <uint> stack;
			std::vector<uint> minlink;	
			std::vector<uint> index;
			std::vector<uint>& coloring;
	      public:
			colorer(const Graph& g, std::vector<uint>& coloring):
				g(g), state(g.size(), 0), minlink(g.size(), 0), index(g.size(), 0), coloring(coloring)
			{
				current_index = current_component = 0;
			};
			void connect(uint vertex)
			{
				if (state[vertex]) return;
				state[vertex] = 1;
				minlink[vertex] = index[vertex] = current_index ++;
				stack.push(vertex);
				for (uint neighbor: g[vertex])
					if (!state[neighbor])
					{
						connect(neighbor);
						minlink[vertex] = std::min(minlink[vertex], minlink[neighbor]);														
					}
					else
						if (state[neighbor] == 1)
							minlink[vertex] = std::min(minlink[vertex], index[neighbor]);
				if (index[vertex] == minlink[vertex])
				{
					unsigned int current_vertex;
					do
					{
						current_vertex = stack.top();
						stack.pop();
						state[current_vertex] = 2;
						coloring[current_vertex] = current_component;
					}
					while (current_vertex != vertex);
					current_component ++;
				}
			};
		};
		colorer c(g, coloring);
		for (size_t vertex = 0; vertex < g.size(); ++ vertex)
			c.connect(vertex);
		return coloring;
	};
};
};
#endif // GRAPHALGO_H
