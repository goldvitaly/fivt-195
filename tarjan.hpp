#pragma once

#include "graph.hpp"

#include <list>
#include <vector>

namespace graph
{
namespace algo
{

class TarjanMaker
{
	public:
		explicit TarjanMaker(const Graph& gr);

		const std::list<std::list<unsigned>>& make();

	private:
		const Graph& g;
		std::list<std::list<unsigned>> result;
		std::vector<unsigned> lowlink;
		std::vector<unsigned> tin;
		std::list<unsigned> st;
		std::vector<bool> processing;
		unsigned time;

		void dfs(unsigned v);
};

}
}