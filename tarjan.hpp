#pragma once

#include "graph.hpp"

#include <list>
#include <vector>

namespace graph
{
namespace algo
{

struct VertexInfo
{
	unsigned lowlink;
	unsigned tin;
	bool processing;
};

class TarjanMaker
{
public:
	explicit TarjanMaker(const Graph& gr);

	const std::list<std::list<unsigned>>& make();

private:
	const Graph& g;
	std::list<std::list<unsigned>> result;
	std::list<unsigned> st;
	std::vector<VertexInfo> info;
	unsigned time;

	void dfs(unsigned v);
};

}
}