// Любая проблема решается введением дополнительного уровня абстракции. Кроме проблемы избыточного количества уровней абстракции.

#include "graph.hpp"
#include "useful.hpp"
#include "dfs.hpp"

int main()
{
	Graph g(5);
	g.add_edge(0,2);
	g.add_edge(0,3);
	g.add_edge(2,3);
	g.add_edge(2,0);
	return 0;
};
