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

size_t Graph::vertexNum() const {
	return IncidencePtrs.size();
}
const IIncidence& Graph::operator[] (size_t vertexId) const {
	return *IncidencePtrs[vertexId];
}
} // namespace graph
