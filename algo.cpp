#include "graph.hpp"

using namespace std;

namespace graph
{

namespace algo
{

const int COLOR_WHITE = 0;
const int COLOR_GREY = 1;
const int COLOR_BLACK = 2;

//return initial color of v
int dfs(const Graph& g, unsigned v, int* colors)
{
	if(colors[v] != COLOR_WHITE)
		return colors[v];
	colors[v] = COLOR_GREY;
	for(auto it : g.getNode(v))
		dfs(g, it, colors);
	colors[v] = COLOR_BLACK;
	return COLOR_WHITE;
}

//returns number of connected components
unsigned countComponents(const Graph& g)
{
	int* colors = new int[g.getSize()];
	for(unsigned i = 0; i < g.getSize(); ++i)
		colors[i] = 0;
	unsigned count = 0;
	for(unsigned v = 0; v < g.getSize(); ++v)
		if(dfs(g, v, colors) == COLOR_WHITE)
			count++;
	delete colors;
	return count;
}

}

}
