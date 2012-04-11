#include <iostream>

#include "Graph.hpp"
#include "iIncidence.hpp"
#include "Iterator.hpp"
#include "IteratorWrapper.hpp"
#include "VectorIncidence.hpp"
// как можно избавиться от подключениях всех библиотек в main?

typedef Graph::iIncidence::IteratorWrapper iterator;

int main()
{
	Graph g;
	
	g.addVertex(Graph::incList(new VectorIncidence()));
	g.addVertex(Graph::incList(new VectorIncidence()));
	g.addVertex(Graph::incList(new VectorIncidence()));
	g.addVertex(Graph::incList(new VectorIncidence()));
	
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(2, 1);
		
	return 0;
}
