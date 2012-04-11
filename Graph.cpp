#include "Graph.hpp"
#include "iIncidence.hpp"

Graph::Graph() {}
Graph::~Graph() {}

void Graph::addVertex(incList list)
{
	incLists.push_back(std::move(list));
}

void Graph::addEdge(size_t from, size_t to)
{
	incLists[from]->addEdge(to);
}
void Graph::delEdge(size_t from, size_t to)
{
	incLists[from]->delEdge(to);
}

size_t Graph::size() const
{
	return incLists.size();
}
const Graph::iIncidence& Graph::operator[] (int i) const
{
	return *incLists[i];
}
