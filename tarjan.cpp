#include "tarjan.hpp"

#include "graph.hpp"
#include <vector>
#include <algorithm>
#include <stack>

std::vector<std::vector<size_t> > tarjan_scc (const graph& g)
{
	struct vertex
	{
		size_t v;
		incidence::iterator it;
		bool in;
		vertex(size_t v_, const incidence::iterator& it_)
		{
			v = v_;
			it = it_;
			in = false;
		}
	};
	enum state_t {not_visited, in_stack, scc_assigned};
	
	std::vector<size_t> tin(g.size(), 0), tout(g.size(), 0);
	std::vector<state_t> state(g.size(), not_visited);
	std::vector<std::vector<size_t> > scc(0);
	std::stack<size_t> way_stack;
	
	std::stack<vertex> call_stack;
	
	size_t timer = 0;
	
	for (size_t i = 0; i < g.size(); i++) if (state[i] == not_visited)
	{
		call_stack.push(vertex(i, g[i].begin()));
		while (!call_stack.empty())
		{
			size_t v = call_stack.top().v;
			auto it = call_stack.top().it;
			bool in = call_stack.top().in;
			
			if (!in) // position in loop before recursion called
			{
				if (it == g[v].begin())
				{
					state[v] = in_stack;
					tin[v] = tout[v] = timer++;
					way_stack.push(v);
				}
				if (it == g[v].end())
				{
					if (tin[v] == tout[v]) // found new component
					{
						scc.push_back(std::vector<size_t>(0));
						while (!way_stack.empty() && tin[way_stack.top()] >= tin[v])
						{
							scc.back().push_back(way_stack.top());
							state[way_stack.top()] = scc_assigned;
							way_stack.pop();
						}
					}
					call_stack.pop();
					continue;
				}
				switch(state[*it])
				{
					case scc_assigned:
						++call_stack.top().it;
						break;
					case in_stack:
						tout[v] = std::min(tout[v], tin[*it]);
						++call_stack.top().it;
						break;
					case not_visited:
						call_stack.top().in = true;
						call_stack.push(vertex(*it, g[*it].begin()));
						break;
				}
			}
			else // position in loop after returned from child
			{
				call_stack.top().in = false;
				tout[v] = std::min(tout[v], tout[*it]);
				++call_stack.top().it;
			}
		}
	}
	
	return scc;
}
