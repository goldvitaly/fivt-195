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
			explicit DFSMaker(const Graph& gr) : g(gr)
			{ uncolor(); }
			
			void uncolor()
			{
				color.assign(g.size(), COLOR_WHITE);
			}

			void clearStack()
			{
				while(!out.empty())
					out.pop();
			}
			
			int dfs(unsigned v)
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
			
			const std::stack<unsigned>& make()
			{
				uncolor();
				clearStack();
				for(unsigned v = 0; v < g.size(); ++v)
					dfs(v);
				return out;
			}
			
			const std::stack<unsigned>& getOutStack()
			{ return out; }
			
		private:
			const Graph& g;
			std::vector<int> color;
			std::stack<unsigned> out;
	};
}
}
