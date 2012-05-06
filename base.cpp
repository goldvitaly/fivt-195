#include "base.hpp"

#include <memory>

namespace graph
{
	Graph reverse(const Graph& g)
	{
		Graph result;
		for(unsigned v = 0; v < g.size(); ++v)
			result.add(std::unique_ptr<Node>(new ListNode()));
		for(unsigned v = 0; v < g.size(); ++v)
		{
			for(unsigned u : g.getNode(v))
				result.connect(u, v);
		}
		return result;
	}
}
