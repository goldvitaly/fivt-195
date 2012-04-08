#pragma once

#include "graph.hpp"
#include "base.hpp"

#include <stack>
#include <vector>
#include <set>

namespace graph
{
namespace algo
{
	class KosarajuMaker
	{
		public:
			KosarajuMaker(const Graph& gr) : g(gr), rev(reverse(gr)) {}
			
			std::set<std::set<unsigned>> make()
			{
				std::set<std::set<unsigned>> ret;
				auto revOut = DFSMaker(rev).make();
				DFSMaker dfser(g);
				while(!revOut.empty())
				{
					unsigned v = revOut.top();
					revOut.pop();
					dfser.dfs(v);
					auto reached = dfser.getOutStack();
					std::set<unsigned> component;
					while(!reached.empty())
					{
						component.insert(reached.top());
						reached.pop();
					}
					ret.insert(component);
				}
				return ret;
			}
			
		private:
			const Graph& g;
			Graph rev;
	};
}
}
