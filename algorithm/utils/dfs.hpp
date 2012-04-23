#ifndef DFS_HPP
#define DFS_HPP

#include "../../Graph/Graph.hpp"
#include <vector>

namespace __dfs_impl
{
class Wrapper
{
private:
	const graph::Graph& graph;
	std::vector<char> visited;
	
	void dfs(size_t vertex)
	{
		visited[vertex] = true;
		for (auto child: graph[vertex])
			if (!visited[child])
				dfs(child);
	}
	
public:
	Wrapper(const graph::Graph& graph_): graph(graph_) {}
	bool reachable(size_t from, size_t to)
	{
		visited.assign(graph.size(), false);
		dfs(from);
		return visited[to];
	}
};
} // namespace __dfs_impl

bool reachable (size_t from, size_t to, const graph::Graph& graph)
{
	__dfs_impl::Wrapper wrapper(graph);
	return wrapper.reachable(from, to);
}

#endif
