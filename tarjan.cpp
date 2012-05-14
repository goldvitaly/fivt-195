#include "tarjan.hpp"

namespace graph
{
namespace algo
{

TarjanMaker::TarjanMaker(const Graph& gr) : g(gr) {}

const std::list<std::list<unsigned>>& TarjanMaker::make()
{
	result.clear();
	info.resize(g.size());
	st.clear();
	time = 0;
	for(unsigned v = 0; v < g.size(); ++v)
		if(info[v].tin == 0)
			dfs(v);
	return result;
}

void TarjanMaker::dfs(unsigned v)
{
	info[v].tin = info[v].lowlink = ++time;
	info[v].processing = true;
	st.push_back(v);
	for(unsigned u : g.getNode(v))
	{
		if(info[u].tin == 0)
		{
			dfs(u);
			if(info[u].lowlink < info[v].lowlink)
				info[v].lowlink = info[u].lowlink;
		}
		else if(info[u].processing)
			if(info[u].tin < info[v].lowlink)
				info[v].lowlink = info[u].tin;
	}
	if(info[v].lowlink == info[v].tin)
	{
		std::list<unsigned> comp;
		unsigned next;
		do
		{
			next = st.back();
			comp.push_back(next);
			info[next].processing = false;
			st.pop_back();
		}
		while(next != v);
		result.push_back(comp);
	}
}

}
}