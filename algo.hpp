//Deprecated.

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

const unsigned HUGE_TIME = 2 << 31;

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
			outStack.push(v);
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
		
		int dfsTarjan(unsigned v)
		{
			if(colors[v] != COLOR_WHITE)
				return colors[v];
			tin[v] = time++;
			stackTarjan.push(v);
			colors[v] = COLOR_GREY;
			bool root = true;
			for(unsigned u : g.getNode(v))
			{
				dfsTarjan(u);
				if(tin[v] > tin[u])
				{
					tin[v] = tin[u];
					root = false;
				}
			}
			if(root)
			{
				++currentComponent;
				unsigned u;
				do
				{
					u = stackTarjan.top();
					components[u] = currentComponent;
					tin[u] = HUGE_TIME;
					stackTarjan.pop();
				}
				while(u != v);
			}
			colors[v] = COLOR_BLACK;
			return COLOR_WHITE;
		}
		
		void dfs()
		{
			componentsCount = 0;
			colors.assign(g.size(), COLOR_WHITE);
			while(!outStack.empty())
				outStack.pop();
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
			return outStack;
		}
		
		void makeKosarajuAlgo()
		{
			currentComponent = 0;
			components.assign(g.size(), 0);
			colors.assign(g.size(), COLOR_WHITE);
			Graph rev = reverse(g);
			GraphSolver revSolver(rev);
			revSolver.dfs();
			auto& outStackRev = revSolver.getOutStack();
			while(!outStackRev.empty())
			{
				currentComponent++;
				unsigned v = outStackRev.top();
				dfsKosaraju(v);
				outStackRev.pop();
			}
		}
		
		void makeTarjanAlgo()
		{
			components.assign(g.size(), 0);
			colors.assign(g.size(), COLOR_WHITE);
			tin.assign(g.size(), 0);
			time = 0;
			currentComponent = 0;
			while(!stackTarjan.empty())
				stackTarjan.pop();
			size_t sz = g.size();
			for(unsigned v = 0; v < sz; v++)
				dfsTarjan(v);
		}
		
		const std::vector<int>& getComponents() const
		{
			return components;
		}
		
	private:
		const Graph& g;
		std::vector<int> colors;
		std::vector<int> components;
		std::vector<unsigned> tin;
		std::stack<unsigned> outStack;
		std::stack<unsigned> stackTarjan;
		int componentsCount;
		int currentComponent;
		unsigned time;
};

}
}
