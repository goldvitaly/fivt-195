#pragma once

#include "graph.hpp"
#include "base.hpp"

#include <stack>
#include <vector>
#include <list>

namespace graph
{
namespace algo
{
	class KosarajuMaker
	{
		public:
			explicit KosarajuMaker(const Graph& gr) : g(gr), rev(reverse(gr)) {}
			
			const std::list<std::list<unsigned>>& make()
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
			
		private:
			const Graph& g;
			Graph rev;
			std::list<std::list<unsigned>> result;
	};
}
}
