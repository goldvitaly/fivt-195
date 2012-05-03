#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>

#include "IIncidence.hpp"

namespace graph
{
template<typename TEdge>
class Graph
{
public: // declaration of types
	typedef std::unique_ptr<IIncidence<TEdge>> IncidencePtr;
	
public: // declaration and definition of methods
	Graph() {}
	Graph(const Graph&) = delete;
	Graph(Graph&&) = default;

	void addVertex(IncidencePtr list) {
		IncidencePtrs.push_back(std::move(list));
	}
	void clear() {
		IncidencePtrs.clear();
	}
	
	void addEdge(size_t from, TEdge edge) {
		IncidencePtrs[from]->addEdge(edge);
	}
	void delEdge(size_t from, TEdge edge) {
		IncidencePtrs[from]->delEdge(edge);
	}
	
	size_t vertexNum() const {
		return IncidencePtrs.size();
	}
	const IIncidence<TEdge>& operator[] (size_t vertexId) const {
		return *IncidencePtrs[vertexId];
	}
	
private:
	std::vector<IncidencePtr> IncidencePtrs;
};
} // namespace graph

#endif
