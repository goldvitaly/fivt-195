// Любая проблема решается введением дополнительного уровня абстракции. Кроме проблемы избыточного количества уровней абстракции.

#include "graph.h"
#include "useful.h"
#include "dfs.h"

int main()
{
	Graph g(5);
	g.add_edge(0,2);
	g.add_edge(0,3);
	g.add_edge(2,3);
	g.add_edge(2,0);
	std::cout << graph::algorithm::calculate_strongly_connected_components_coloring(g) << std::endl;
	return 0;
};
