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
						std::set<unsigned> component;
						while(!reached.empty())
						{
							component.insert(reached.top());
							reached.pop();
						}
						result.insert(component);
						dfser.clearStack();
					}
				}
				return result;
			}
			
		private:
			const Graph& g;
			Graph rev;
			std::set<std::set<unsigned>> result;
	};
}
}
