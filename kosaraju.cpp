#include "kosaraju.hpp"

#include "dfs.hpp"

#include <vector>
#include <stack>

namespace graph
{
namespace algo
{

KosarajuMaker::KosarajuMaker(const Graph& gr) : g(gr), rev(reverse(gr)) {}

const std::list<std::list<unsigned>>& KosarajuMaker::make()
{
	result.clear();
	auto revOut = DFSMaker(rev).make();
	DFSMaker dfser(g);
	while(!revOut.empty())
	{
		unsigned v = revOut.top();
		revOut.pop();
		if(dfser.dfs(v) == COLOR_WHITE)
		{
			auto reached = dfser.getOutStack();
			std::list<unsigned> component;
			while(!reached.empty())
			{
				component.push_back(reached.top());
				reached.pop();
			}
			result.push_back(component);
			dfser.clearStack();
		}
	}
	return result;
}

}//namespace algo
}//namespace graph
