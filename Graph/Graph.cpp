#include "Graph.hpp"
#include "IIncidence.hpp"

namespace graph
{
Graph::Graph() {}	
void Graph::addVertex(IncidencePtr list) {
	IncidencePtrs.push_back(std::move(list));
}
void Graph::clear() {
	IncidencePtrs.clear();
}

void Graph::addEdge(size_t from, size_t to) {
	IncidencePtrs[from]->addEdge(to);
}
void Graph::delEdge(size_t from, size_t to) {
	IncidencePtrs[from]->delEdge(to);
}

size_t Graph::size() const {
	return IncidencePtrs.size();
}
const IIncidence& Graph::operator[] (int i) const {
	return *IncidencePtrs[i];
}
} // namespace graph
