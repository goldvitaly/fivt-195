#pragma once

#include "graph.hpp"

#include <stack>
#include <vector>

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
	explicit DFSMaker(const Graph& gr);
	void uncolor();
	void clearStack();
	int dfs(unsigned v);
	const std::stack<unsigned>& make();
	const std::stack<unsigned>& getOutStack();
	
private:
	const Graph& g;
	std::vector<int> color;
	std::stack<unsigned> out;
};

}
}
