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

class GraphSolver
{
	public:
		explicit GraphSolver(const Graph& gr) : g(gr)
		{
			components.assign(g.size(), 0);
			colors.assign(g.size(), COLOR_WHITE);
		}
		
		int dfsSimple(unsigned v)
		{
			if(colors[v] != COLOR_WHITE)
				return colors[v];
			colors[v] = COLOR_GREY;
			for(unsigned u : g.getNode(v))
				dfsSimple(u);
			colors[v] = COLOR_BLACK;
			tout.push(v);
			return COLOR_WHITE;
		}
		
		int dfsKosaraju(unsigned v)
		{
			if(colors[v] != COLOR_WHITE)
				return colors[v];
			components[v] = currentComponent;
			colors[v] = COLOR_GREY;
			for(unsigned u : g.getNode(v))
				dfsKosaraju(u);
			colors[v] = COLOR_BLACK;
			return COLOR_WHITE;
		}
		
		void dfs()
		{
			componentsCount = 0;
			colors.assign(g.size(), COLOR_WHITE);
			while(!tout.empty())
				tout.pop();
			size_t sz = g.size();
			for(unsigned v = 0; v < sz; v++)
				if(dfsSimple(v) == COLOR_WHITE)
					componentsCount++;
		}
		
		int getComponentsCount() const
		{
			return componentsCount;
		}
		
		std::stack<unsigned>& getOutStack()
		{
			return tout;
		}
		
		void makeKosarajuAlgo()
		{
			currentComponent = 0;
			components.assign(g.size(), 0);
			colors.assign(g.size(), COLOR_WHITE);
			Graph rev = reverse(g);
			GraphSolver revSolver(rev);
			revSolver.dfs();
			auto& toutRev = revSolver.getOutStack();
			while(!toutRev.empty())
			{
				currentComponent++;
				unsigned v = toutRev.top();
				dfsKosaraju(v);
				toutRev.pop();
			}
		}
		
		void makeTarjanAlgo()
		{
			components.assign(g.size(), 0);
		}
		
		const std::vector<int>& getComponents() const
		{
			return components;
		}
		
	private:
		const Graph& g;
		std::vector<int> colors;
		std::vector<int> components;
		std::stack<unsigned> tout;
		int componentsCount;
		int currentComponent;
		
};

}
}
