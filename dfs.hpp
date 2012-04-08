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
			DFSMaker(const Graph& gr) : g(gr)
			{ uncolor(); }
			
			void uncolor()
			{
				color.assign(g.size(), COLOR_WHITE);
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
			
			std::stack<unsigned>& make()
			{
				uncolor();
				for(unsigned v = 0; v < g.size(); ++v)
					dfs(v);
				return out;
			}
			
			std::stack<unsigned>& getOutStack()
			{ return out; }
			
		private:
			const Graph& g;
			std::vector<int> color;
			std::stack<unsigned> out;
	};
}
}
