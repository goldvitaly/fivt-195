#pragma once

#include "graph.hpp"

#include <list>
#include <vector>
#include <algorithm>

namespace graph
{
namespace algo
{

class TarjanMaker
{
	public:
		explicit TarjanMaker(const Graph& gr) : g(gr) {}

		const std::list<std::list<unsigned>>& make()
		{
			result.clear();
			lowlink.assign(g.size(), 0);
			tin.assign(g.size(), 0);
			processing.assign(g.size(), false);
			st.clear();
			time = 0;
			for(unsigned v = 0; v < g.size(); ++v)
				if(tin[v] == 0)
					dfs(v);
			return result;
		}

	private:
		const Graph& g;
		std::list<std::list<unsigned>> result;
		std::vector<unsigned> lowlink;
		std::vector<unsigned> tin;
		std::list<unsigned> st;
		std::vector<bool> processing;
		unsigned time;

		void dfs(unsigned v)
		{
			tin[v] = lowlink[v] = ++time;
			processing[v] = true;
			st.push_back(v);
			for(unsigned u : g.getNode(v))
			{
				if(tin[u] == 0)
				{
					dfs(u);
					if(lowlink[u] < lowlink[v])
						lowlink[v] = lowlink[u];
				}
				else if(processing[u])
					if(tin[u] < lowlink[v])
						lowlink[v] = tin[u];
			}
			if(lowlink[v] == tin[v])
			{
				std::list<unsigned> comp;
				unsigned next;
				do
				{
					next = st.back();
					comp.push_back(next);
					processing[next] = false;
					st.pop_back();
				}
				while(next != v);
				result.push_back(comp);
			}
		}
};

}
}