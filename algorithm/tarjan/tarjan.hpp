#ifndef TARJAN_HPP
#define TARJAN_HPP

#include <vector>
#include <stack>
#include <iostream>
#include <cassert>

#include "../../Graph/Graph.hpp"

namespace __tarjan_impl
{
class Wrapper
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
	
	std::vector<Info> info;
	std::stack<size_t> path;
	size_t dfsTime;
	size_t compsNumber;
	
	void dfs(size_t vertex)
	{
		Info& curInfo = info[vertex];
		curInfo.tin = curInfo.tup = dfsTime++;
		curInfo.visited = curInfo.inStack = true;
		path.push(vertex);
		
		for (auto child: graph[vertex])
		{
			const Info& childInfo = info[child];
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
			while (!path.empty() && info[path.top()].tin >= curInfo.tin)
			{
				info[path.top()].col = compsNumber;
				info[path.top()].inStack = false;
				path.pop();
			}
			++compsNumber;
		}
	}
	

public:
	Wrapper (const graph::Graph& graph_): graph(graph_) {}
	
	std::vector<size_t> getStronglyConnectedComponents()
	{
		info.assign(graph.size(), Info());
		path = std::stack<size_t>();
		dfsTime = compsNumber = 0; // стоит ли разнести эти присваивания, потому что они разные по смыслу?
		
		for (size_t i = 0; i < graph.size(); i++)
			if (!info[i].visited)
				dfs(i);
		
		std::vector<size_t> col(graph.size());
		for (size_t i = 0; i < graph.size(); i++)
			col[i] = info[i].col;
		return std::move(col);
	}
	
};
} // namespace __tarjan_impl

inline std::vector<size_t> tarjanStronglyConnectedComponents(const graph::Graph& graph)
{
	__tarjan_impl::Wrapper wrapper(graph);
	return wrapper.getStronglyConnectedComponents();
}

#endif
