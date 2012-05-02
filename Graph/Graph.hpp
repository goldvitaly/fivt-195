#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>

#include "IIncidence.hpp"

namespace graph
{
class Graph
{
public: // declaration of types
	typedef std::unique_ptr<IIncidence> IncidencePtr;
	
public: // declaration of methods
	Graph();
	Graph(const Graph&) = delete;
	Graph(Graph&&) = default;

	void addVertex(IncidencePtr);
	void clear();
	
	void addEdge(size_t, size_t);
	void delEdge(size_t, size_t);
	
	size_t vertexNum() const;
	const IIncidence& operator[] (size_t) const;
	
private:
	std::vector<IncidencePtr> IncidencePtrs;
};
} // namespace graph

#endif
