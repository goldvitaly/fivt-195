#pragma once

#include "graph.hpp"

#include <memory>

namespace graph
{
	Graph reverse(const Graph& g)
	{
		Graph ret;
		for(unsigned v = 0; v < g.size(); ++v)
			ret.add(std::unique_ptr<Node>(new ListNode()));
		for(unsigned v = 0; v < g.size(); ++v)
		{
			for(unsigned u : g.getNode(v))
				ret.connect(v, u);
		}
		return ret;
	}
}
