#include "Graph.hpp"
#include "IIncidence.hpp"

namespace graph
{
Graph::Graph() {}
Graph::~Graph() {}

void Graph::addVertex(incList list) {
	incLists.push_back(std::move(list));
}
void Graph::clear() {
	incLists.clear();
}

void Graph::addEdge(size_t from, size_t to) {
	incLists[from]->addEdge(to);
}
void Graph::delEdge(size_t from, size_t to) {
	incLists[from]->delEdge(to);
}

size_t Graph::size() const {
	return incLists.size();
}
const IIncidence& Graph::operator[] (int i) const {
	return *incLists[i];
}
} // namespace graph
