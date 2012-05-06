#include "dfs.hpp"

namespace graph
{
namespace algo
{
	DFSMaker::DFSMaker(const Graph& gr) : g(gr)
	{ uncolor(); }
	
	void DFSMaker::uncolor()
	{
		color.assign(g.size(), COLOR_WHITE);
	}

	void DFSMaker::clearStack()
	{
		while(!out.empty())
			out.pop();
	}
	
	int DFSMaker::dfs(unsigned v)
	{
		if(color[v] != COLOR_WHITE)
			return color[v];
		color[v] = COLOR_GREY;
		for(unsigned u : g.getNode(v))
			dfs(u);
		color[v] = COLOR_BLACK;
		out.push(v);
		return COLOR_WHITE;
	}
	
	const std::stack<unsigned>& DFSMaker::make()
	{
		uncolor();
		clearStack();
		for(unsigned v = 0; v < g.size(); ++v)
			dfs(v);
		return out;
	}
	
	const std::stack<unsigned>& DFSMaker::getOutStack()
	{ return out; }
}
}
