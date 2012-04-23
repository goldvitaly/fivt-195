#ifndef TARJAN_HPP
#define TARJAN_HPP

#include <vector>
#include <stack>
#include <iostream>
#include <cassert>

#include "../../Graph/Graph.hpp"

namespace __tarjan_impl
{
class TarjanRunner
{
private:
	class Info
	{
	public:
		bool visited, inStack;
		size_t col, tin, tup;
		
		Info() {
			visited = inStack = false;
			col = tin = tup = 0;
		}
	};
	const graph::Graph& graph;
	
	std::vector<Info> vertexInfo;
	std::stack<size_t> visitedStack;
	size_t dfsTime;
	size_t compsNumber;
	
	void dfs(size_t vertex)
	{
		Info& curInfo = vertexInfo[vertex];
		curInfo.tin = curInfo.tup = dfsTime++;
		curInfo.visited = curInfo.inStack = true;
		visitedStack.push(vertex);
		
		for (auto child: graph[vertex])
		{
			const Info& childInfo = vertexInfo[child];
			if (!childInfo.visited)
			{
				dfs(child);
				curInfo.tup = std::min(curInfo.tup, childInfo.tup);
			}
			else if (childInfo.inStack)
			{
				curInfo.tup = std::min(curInfo.tup, childInfo.tin);
			}
		}
		if (curInfo.tup == curInfo.tin)
		{
			while (!visitedStack.empty() && vertexInfo[visitedStack.top()].tin >= curInfo.tin)
			{
				vertexInfo[visitedStack.top()].col = compsNumber;
				vertexInfo[visitedStack.top()].inStack = false;
				visitedStack.pop();
			}
			++compsNumber;
		}
	}
	

public:
	TarjanRunner (const graph::Graph& graph_): graph(graph_) {}
	
	std::vector<size_t> getStronglyConnectedComponents()
	{
		vertexInfo.assign(graph.size(), Info());
		visitedStack = std::stack<size_t>();
		dfsTime = 0;
		compsNumber = 0;
		
		for (size_t i = 0; i < graph.size(); i++)
			if (!vertexInfo[i].visited)
				dfs(i);
		
		std::vector<size_t> col(graph.size());
		for (size_t i = 0; i < graph.size(); i++)
			col[i] = vertexInfo[i].col;
		return std::move(col);
	}
	
};
} // namespace __tarjan_impl

inline std::vector<size_t> tarjanStronglyConnectedComponents(const graph::Graph& graph)
{
	__tarjan_impl::TarjanRunner runner(graph);
	return runner.getStronglyConnectedComponents();
}

#endif
