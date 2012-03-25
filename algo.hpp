#pragma once

#include "graph.hpp"
#include <vector>
#include <stack>
#include <map>

namespace graph
{
namespace algo
{
	
enum
{
	COLOR_WHITE, COLOR_GREY, COLOR_BLACK
};

class DFSMaker
{
	public:
		explicit DFSMaker(const Graph& gr) : g(gr) 
		{
			colors.resize(gr.size(), COLOR_WHITE);
		}
		
		int dfs(unsigned v)
		{
			if(colors[v] != COLOR_WHITE)
				return colors[v];
			colors[v] = COLOR_GREY;
			for(unsigned u : g.getNode(v))
				dfs(u);
			tout.push(v);
			colors[v] = COLOR_BLACK;
			return COLOR_WHITE;
		}
		
		void dfs()
		{
			components = 0;
			while(!tout.empty())
				tout.pop();
			size_t sz = g.size();
			for(unsigned v = 0; v < sz; v++)
				if(dfs(v) == COLOR_WHITE)
					components++;
		}
		
		int componentsCount() const
		{
			return components;
		}
		
		std::stack<unsigned> getOutStack() const
		{
			return tout;
		}
		
	private:
		const Graph& g;
		std::vector<int> colors;
		std::stack<unsigned> tout;
		int components;
};

Graph reverse(const Graph& g)
{
	Graph rev;
	size_t sz = g.size();
	for(unsigned v = 0; v < sz; ++v)
		rev.add(new ListNode());//no ideas how to do it correctly
	for(unsigned v = 0; v < sz; ++v)
		for(unsigned u : g.getNode(v))
			rev.connect(u, v);
	return rev;
}

}
}
